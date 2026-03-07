#include "lobby.hpp"

#include <chrono>
#include <cstring>

using namespace std;

Lobby::Lobby(Setting* setting, ILobbyNetwork* network, ILobbyRenderer* render,
             ILobbyInputHandler* input)
    : setting(setting), network(network), render(render), input(input)
{
}

bool Lobby::start()
{
    set_nickname();
    int choice;

    while (true) {
        render->render_clear();
        render->render_select();
        input->scan(&choice, 1);
        if (choice == 1) {
            if (open_room()) return true;
        }
        else if (choice == 2) {
            if (enter_room()) return false;
        }
        render->render_clear();
    }
}

void Lobby::set_nickname()
{
    render->render_clear();
    render->render_big_text(20, 7, "SET NICKNAME");
    render->render_input_window(27, 20, "type your nickname.[length : 1 ~ 8]");

    char nickname[9];

    while (true) {
        input->scan(nickname, 8, true);

        if (nickname[0] != '\0') {
            setting->nick_name = nickname;
            break;
        }
    }
}

void Lobby::reload() {}

bool Lobby::open_room()
{
    char buffer[BUF_SIZE];
    char broadcast_ip[16];
    std::chrono::steady_clock::time_point base_time;
    std::unordered_map<std::string, std::string> client_ip_address_for_send;
    int index = 0;
    int room_user_index = 0;
    bool is_game_start = false;
    char s[BUF_SIZE];
    char ip[16];

    memset(my_id, 0, sizeof(my_id));
    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();
    network->find_broadcast_ip(broadcast_ip);

    render->render_user_id_input();
    input->scan(my_id, sizeof(my_id), 1);
    render->render_server_view_room(my_id, client_ip_address);

    base_time = std::chrono::steady_clock::now();
    while (true) {
        if (input->scan(s, sizeof(s), 0) == 1) {
            if (strcmp(s, "q") == 0) {
                render->render_clear();
                network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0, 0, 0, 1,
                                  broadcast_ip);
                is_game_start = false;
                break;
            }
            else {
                network->send_multi_udp(my_id, client_ip_address, client_ip_address.size(), 0, 1, 0,
                                        0, 0, client_ip_address);
                network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0, 0, 0, 1,
                                  broadcast_ip);
                is_game_start = true;
                break;
            }
        }

        if (std::chrono::steady_clock::now() - base_time >= std::chrono::milliseconds(5000)) {
            base_time = std::chrono::steady_clock::now();
            network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0, 1, 0, 0,
                              broadcast_ip);
        }

        memset(ip, 16, sizeof(ip));
        user_data received_data;
        if (network->recv_udp(received_data, ip) == false) continue;

        if (index >= 4 ||
            (received_data.is_enter == true &&
             client_ip_address.find(std::string(received_data.id)) != client_ip_address.end()))
            network->send_udp(my_id, client_ip_address, client_ip_address.size(), 1, 0, 0, 0, 0,
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
            index = client_ip_address.size();
            render->render_server_view_room(my_id, client_ip_address);
            network->send_multi_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0, 0, 1,
                                    0, client_ip_address_for_send);
        }
    }

    return is_game_start;
}

bool Lobby::enter_room()
{
    char buffer[BUF_SIZE];
    int room_user_index = 0;
    bool is_in_room = false;
    bool is_game_start = false;
    char s[BUF_SIZE];
    char room_ip[16];

    memset(my_id, 0, sizeof(my_id));
    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();

    render->render_user_id_input();
    input->scan(my_id, sizeof(my_id), 1);
    render->render_view_enter_room(server_ip_address);

    while (true) {
        if (input->scan(s, sizeof(s), 0) == 1) {
            if (is_in_room == false && strcmp(s, "q") == 0) {
                render->render_clear();
                is_game_start = false;
                break;
            }
            else if (is_in_room)
                network->send_udp(my_id, false, selected_server_ip_address);
            else {
                if (server_ip_address.find(std::string(s)) == server_ip_address.end()) continue;
                network->send_udp(my_id, true, s);
            }
        }

        memset(room_ip, 16, sizeof(room_ip));
        room_data received_data;
        if (network->recv_udp(received_data, room_ip) == false) continue;

        if (received_data.is_broadcast &&
            server_ip_address.find(std::string(room_ip)) == server_ip_address.end()) {
            server_ip_address[std::string(room_ip)] = std::string(received_data.room_master_id);
            if (is_in_room == false) render->render_view_enter_room(server_ip_address);
        }
        else if (received_data.is_broadcast_delete) {
            server_ip_address.erase(std::string(room_ip));
            if (is_in_room == false ||
                (is_in_room == true && strcmp(selected_server_ip_address, room_ip) == 0))
                render->render_view_enter_room(server_ip_address);
            is_in_room = false;
        }
        else if (received_data.is_update) {
            memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
            snprintf(selected_server_ip_address, sizeof(selected_server_ip_address), "%s", room_ip);
            client_ip_address.clear();
            is_in_room = false;
            for (int i = 0; i < received_data.id_len; ++i) {
                if (strcmp(received_data.id[i], my_id) == 0) is_in_room = true;
                client_ip_address[std::string(received_data.id[i])] =
                    std::string(received_data.room_master_id);
            }
            if (is_in_room == true)
                render->render_client_view_room(received_data.room_master_id, client_ip_address);
            else
                render->render_view_enter_room(server_ip_address);
        }
        else if (received_data.is_enter_not_success) {
            server_ip_address.erase(std::string(room_ip));
            render->render_view_enter_room(server_ip_address);
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
    if (is_server) m[std::string(my_id)] = "";
    return m;
}

const char* Lobby::get_server_ip_address() { return selected_server_ip_address; }

const char* Lobby::get_my_id() { return my_id; }
