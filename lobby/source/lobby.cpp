#include "app_state.hpp"
#include "entrance.hpp"
#include "i_input_handler.hpp"
#include "lobby.hpp"
#include "setting_storage.hpp"

#include <chrono>
#include <cstring>

using namespace std;

Lobby::Lobby(Setting* setting, ILobbyNetwork* network, ILobbyRenderer* render,
             ILobbyInputHandler* input)
    : setting(setting), network(network), render(render), input(input)
{
}

AppState Lobby::start()
{
    bool status;
    set_nickname();
    choose_entrance();

    switch (entrance) {
    case Entrance::CREATE_ROOM:
        create_room();
        status = waiting_client();
        if (status == false)
            break;
        return AppState::MULTI_SERVER;

    case Entrance::ENTER_ROOM:
        status = enter_lobby();
        if (status == false)
            break;
        return AppState::MULTI_CLIENT;

    case Entrance::EXIT:
        return AppState::MENU;
    }

    return AppState::MENU;
}

void Lobby::set_nickname()
{
    char nickname[9];

    SettingStorage& setting_storage = SettingStorage::getInstance();
    render->render_set_nickname(setting->nick_name);

    while (true) {
        input->scan(nickname, 8, true);

        if (nickname[0] != '\0') {
            setting->nick_name = nickname;
            setting_storage.save(*setting);
            break;
        }
    }
}

void Lobby::choose_entrance()
{
    int in;
    render->render_entrance();

    while (true) {
        in = input->scan();

        if (in == Arrow::KEY_UP || in == Arrow::KEY_LEFT) {
            entrance--;
        }

        else if (in == Arrow::KEY_DOWN || in == Arrow::KEY_RIGHT) {
            entrance++;
        }

        if (in == '\n' || in == ' ') {
            return;
        }

        render->render_entrance_choice(entrance);
    }
}

void Lobby::create_room()
{
    render->render_create_room();

    while (true) {
        input->scan(room_name, 8, true);

        if (room_name[0] != '\0') {
            break;
        }
    }
}

bool Lobby::waiting_client()
{
    char buffer[BUF_SIZE];
    char broadcast_ip[16];
    std::chrono::steady_clock::time_point base_time;
    std::unordered_map<std::string, std::string> client_ip_address_for_send;
    int index = 0;
    int in;
    int room_user_index = 0;
    bool is_game_start = false;
    char s[BUF_SIZE];
    char ip[16];

    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();
    network->find_broadcast_ip(broadcast_ip);
    render->render_room(room_name, setting->nick_name, true);

    base_time = std::chrono::steady_clock::now();
    while (true) {
        in = input->scan();

        if (in == Key::ESC) {
            render->render_clear();
            network->send_udp(setting->nick_name.c_str(), client_ip_address, room_name, client_ip_address.size(), 0, 0, 0, 0, 1,
                              broadcast_ip);
            return false;
        }

        else if (in == Key::SPACE) {
            network->send_multi_udp(setting->nick_name.c_str(), client_ip_address, room_name, client_ip_address.size(), 0, 1, 0, 0,
                                    0, client_ip_address);
            network->send_udp(setting->nick_name.c_str(), client_ip_address, room_name, client_ip_address.size(), 0, 0, 0, 0, 1,
                              broadcast_ip);
            return true;
        }

        if (std::chrono::steady_clock::now() - base_time >= std::chrono::milliseconds(5000)) {
            base_time = std::chrono::steady_clock::now();
            network->send_udp(setting->nick_name.c_str(), client_ip_address, room_name, client_ip_address.size(), 0, 0, 1, 0, 0,
                              broadcast_ip);
        }

        // TODO 사용자 입장 받아서 clients에 추가
        memset(ip, 16, sizeof(ip));
        user_data received_data;
        if (network->recv_udp(received_data, ip) == false) continue;

        if (index >= 4 ||
            (received_data.is_enter == true &&
             client_ip_address.find(std::string(received_data.id)) != client_ip_address.end()))
            network->send_udp(setting->nick_name.c_str(), client_ip_address, room_name, client_ip_address.size(), 1, 0, 0, 0, 0,
                              broadcast_ip);
        else {
            if (received_data.is_enter == true) {
                client_ip_address[std::string(received_data.id)] = std::string(ip);
                client_ip_address_for_send.clear();
                client_ip_address_for_send = client_ip_address;
            }
            else {
                client_ip_address_for_send.clear();
                client_ip_address_for_send = client_ip_address;
                client_ip_address.erase(std::string(received_data.id));
            }
            render->render_clear();
            render->render_room(room_name, setting->nick_name, true);
            render->render_room_clients(client_ip_address);
            index = client_ip_address.size();
            network->send_multi_udp(setting->nick_name.c_str(), client_ip_address, room_name, client_ip_address.size(), 0, 0, 0, 1,
                                    0, client_ip_address_for_send);
        }
    }
}

bool Lobby::enter_lobby()
{
    int in;
    char buffer[BUF_SIZE];
    int room_user_index = 0;
    bool is_in_room = false;
    bool is_game_start = false;
    char s[BUF_SIZE];
    char room_ip[16];
    int selecting_idx = 0;
    std::vector<std::pair<std::string, std::string>> rooms;

    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();

    render->render_user_id_input();
    render->render_lobby_rooms(rooms, selecting_idx);
    render->render_lobby();

    while (true) {
        in = input->scan();

        if (in == Arrow::KEY_RIGHT || in == Arrow::KEY_DOWN) {
            selecting_idx = (selecting_idx + 1) % rooms.size();
            render->render_clear();
            render->render_lobby();
            render->render_lobby_rooms(rooms, selecting_idx);
        }

        if (in == Arrow::KEY_LEFT || in == Arrow::KEY_UP) {
            selecting_idx = (selecting_idx - 1 + rooms.size()) % rooms.size();
            render->render_clear();
            render->render_lobby();
            render->render_lobby_rooms(rooms, selecting_idx);
        }

        // TODO 사용자 입력 받아서 선택한 방 입장
        if (in == Key::ESC) {
            if (is_in_room)
                network->send_udp(setting->nick_name.c_str(), false, selected_server_ip_address);
            else
                return false;
        } else if (in == Key::SPACE) {
            network->send_udp(setting->nick_name.c_str(), true, rooms[selecting_idx].second.c_str());
        }

        memset(room_ip, 16, sizeof(room_ip));
        room_data received_data;
        if (network->recv_udp(received_data, room_ip) == false) continue;

        if (received_data.is_broadcast &&
            server_ip_address.find(std::string(room_ip)) == server_ip_address.end()) {
            server_ip_address[std::string(room_ip)] = std::string(received_data.room_master_id);
            rooms.push_back(std::make_pair(std::string(received_data.room_name), std::string(room_ip)));
            if (is_in_room == false) {
                render->render_clear();
                render->render_lobby();
                render->render_lobby_rooms(rooms, selecting_idx);
            }
        }
        else if (received_data.is_broadcast_delete) {
            server_ip_address.erase(std::string(room_ip));
            for (int i = 0; i < rooms.size(); ++i) {
                if(strcmp(rooms[i].first.c_str(), received_data.room_name) == 0 && 
                    strcmp(rooms[i].second.c_str(), room_ip) == 0) {
                    rooms.erase(rooms.begin() + i);
                    selecting_idx = (selecting_idx >= rooms.size() ? rooms.size()-1 : selecting_idx);
                    break;
                }
            }
            if (is_in_room == false ||
                (is_in_room == true && strcmp(selected_server_ip_address, room_ip) == 0)) {
                render->render_clear();
                render->render_lobby();
                render->render_lobby_rooms(rooms, selecting_idx);
                is_in_room = false;
            }
        }
        else if (received_data.is_update) {
            memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
            snprintf(selected_server_ip_address, sizeof(selected_server_ip_address), "%s",room_ip);
            client_ip_address.clear(); 
            is_in_room = false; 
            for (int i = 0; i < received_data.id_len; ++i) {
                if (strcmp(received_data.id[i], setting->nick_name.c_str()) == 0) is_in_room = true;
                client_ip_address[std::string(received_data.id[i])] =
                    std::string(received_data.room_master_id);
            }
            if (is_in_room == true) {
                render->render_clear();
                render->render_room(received_data.room_name, std::string(received_data.room_master_id), false);
                render->render_room_clients(client_ip_address);
            } else {
                render->render_clear();
                render->render_lobby();
                render->render_lobby_rooms(rooms, selecting_idx);
            }
        }
        else if (received_data.is_enter_not_success) {
            render->render_clear();
            render->render_lobby();
            render->render_lobby_rooms(rooms, selecting_idx);
            is_in_room = false;
        }
        else if (received_data.is_game_start) {
            memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
            snprintf(selected_server_ip_address, sizeof(selected_server_ip_address), "%s", room_ip);
            client_ip_address.clear();
            for (int i = 0; i < received_data.id_len; ++i)
                client_ip_address[std::string(received_data.id[i])] =
                    std::string(received_data.room_master_id);
            is_game_start = true;
            break;
        }
    }

    return is_game_start;
}

void Lobby::finish()
{
    delete render;
    delete network;
    delete input;
}

const char* Lobby::get_client_ip_address(string key) { return client_ip_address[key].c_str(); }

std::vector<std::pair<std::string, std::string>> Lobby::get_client_ids_ips()
{
    std::vector<std::pair<std::string, std::string>> v;
    for (const auto& [key, value] : client_ip_address)
        v.push_back(std::make_pair(key, value));

    return v;
}

std::unordered_map<std::string, std::string> Lobby::get_ids(bool is_server)
{
    std::unordered_map<std::string, std::string> m;
    for (const auto& [key, value] : client_ip_address) {
        m[key] = "";
        if (is_server == false) m[value] = "";
    }
    if (is_server) m[setting->nick_name] = "";
    return m;
}

const char* Lobby::get_server_ip_address() { return selected_server_ip_address; }

const char* Lobby::get_my_id() { return setting->nick_name.c_str(); }
