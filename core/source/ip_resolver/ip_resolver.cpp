#include "ip_resolver/ip_resolver.hpp"

#include <chrono>

using namespace std;

IpResolver::IpResolver(IIpResolverNetwork* ip_resolver_network,
                       IIpResolverRenderer* ip_resolver_renderer,
                       IIpResolverInputHandler* ip_resolver_input_handler)
    : ip_resolver_network(ip_resolver_network), ip_resolver_renderer(ip_resolver_renderer),
      ip_resolver_input_handler(ip_resolver_input_handler)
{
}

/**
 * @brief 현재 어떤 모드로 들어갈지 확인하는 함수
 */
bool IpResolver::start()
{
    int choice;

    while (true) {
        ip_resolver_renderer->render_select();
        ip_resolver_input_handler->scan(&choice, 1);
        if (choice == 1) {
            if (open_room()) return true;
        }
        else if (choice == 2) {
            if (enter_room()) return false;
        }
    }
}

/**
 * @brief (서버)방을 열고 다른 사용자들의 ip 주소를 저장하게 하는 함수
 */
bool IpResolver::open_room()
{
    char buffer[BUF_SIZE];
    char broadcast_ip[16];
    std::chrono::steady_clock::time_point base_time;
    int index = 0;
    int room_user_index = 0;
    bool is_game_start = false;
    char s[BUF_SIZE];
    char ip[16];

    memset(my_id, 0, sizeof(my_id));
    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();
    ip_resolver_network->find_broadcast_ip(broadcast_ip);

    ip_resolver_renderer->render_user_id_input();
    ip_resolver_input_handler->scan(my_id, sizeof(my_id), 1);
    ip_resolver_renderer->render_server_view_room(my_id, client_ip_address);

    base_time = std::chrono::steady_clock::now();
    while (true) {
        if (ip_resolver_input_handler->scan(s, sizeof(s), 0) == 1) {
            if (strcmp(s, "q") == 0) {
                ip_resolver_renderer->render_clear();
                ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0,
                                              0, 0, 0, 1, broadcast_ip);
                is_game_start = false;
                break;
            }
            else {
                ip_resolver_network->send_multi_udp(my_id, client_ip_address,
                                                    client_ip_address.size(), 0, 1, 0, 0, 0,
                                                    client_ip_address);
                ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0,
                                              0, 0, 0, 1, broadcast_ip);
                is_game_start = true;
                break;
            }
        }

        if (std::chrono::steady_clock::now() - base_time >= std::chrono::milliseconds(5000)) {
            base_time = std::chrono::steady_clock::now();
            ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0,
                                          1, 0, 0, broadcast_ip);
        }

        memset(ip, 16, sizeof(ip));
        user_data received_data;
        if (ip_resolver_network->recv_udp(received_data, ip) == false) continue;

        if (index >= 4 ||
            (received_data.is_enter == true &&
             client_ip_address.find(std::string(received_data.id)) != client_ip_address.end()))
            ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 1, 0,
                                          0, 0, 0, broadcast_ip);
        else {
            if (received_data.is_enter == true)
                client_ip_address[std::string(received_data.id)] = std::string(ip);
            else
                client_ip_address.erase(std::string(received_data.id));
            index = client_ip_address.size();
            ip_resolver_renderer->render_server_view_room(my_id, client_ip_address);
            ip_resolver_network->send_multi_udp(my_id, client_ip_address, client_ip_address.size(),
                                                0, 0, 0, 1, 0, client_ip_address);
        }
    }

    return is_game_start;
}

/**
 * @brief (클라이언트)방에 입장하는 함수
 */
bool IpResolver::enter_room()
{
    int room_user_index = 0;
    bool is_in_room = false;
    bool is_game_start = false;
    char s[BUF_SIZE];
    char room_ip[16];

    memset(my_id, 0, sizeof(my_id));
    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();

    ip_resolver_renderer->render_user_id_input();
    ip_resolver_input_handler->scan(my_id, sizeof(my_id), 1);
    ip_resolver_renderer->render_view_enter_room(server_ip_address);

    while (true) {
        if (ip_resolver_input_handler->scan(s, sizeof(s), 0) == 1) {
            if (is_in_room == false && strcmp(s, "q") == 0) {
                ip_resolver_renderer->render_clear();
                is_game_start = false;
                break;
            }
            else if (is_in_room) {
                ip_resolver_network->send_udp(my_id, false, selected_server_ip_address);
                is_in_room = false;
                ip_resolver_renderer->render_view_enter_room(server_ip_address);
            }
            else {
                if (server_ip_address.find(std::string(s)) == server_ip_address.end()) continue;
                ip_resolver_network->send_udp(my_id, true, s);
                is_in_room = true;
            }
        }

        memset(room_ip, 16, sizeof(room_ip));
        room_data received_data;
        if (ip_resolver_network->recv_udp(received_data, room_ip) == false) continue;

        if (received_data.is_broadcast &&
            server_ip_address.find(std::string(room_ip)) == server_ip_address.end()) {
            server_ip_address[std::string(room_ip)] = std::string(received_data.room_master_id);
            if (is_in_room == false)
                ip_resolver_renderer->render_view_enter_room(server_ip_address);
        }
        else if (received_data.is_broadcast_delete) {
            server_ip_address.erase(std::string(room_ip));
            if (is_in_room == false ||
                (is_in_room == true && strcmp(selected_server_ip_address, room_ip) == 0))
                ip_resolver_renderer->render_view_enter_room(server_ip_address);
        }
        else if (received_data.is_update) {
            memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
            snprintf(selected_server_ip_address, sizeof(selected_server_ip_address), "%s", room_ip);
            client_ip_address.clear();
            for (int i = 0; i < received_data.id_len; ++i)
                client_ip_address[std::string(received_data.room_master_id)] =
                    std::string(received_data.id[i]);
            ip_resolver_renderer->render_client_view_room(received_data.room_master_id,
                                                          client_ip_address);
            is_in_room = true;
        }
        else if (received_data.is_enter_not_success) {
            server_ip_address.erase(std::string(room_ip));
            ip_resolver_renderer->render_view_enter_room(server_ip_address);
            is_in_room = false;
        }
        else if (received_data.is_game_start) {
            memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
            snprintf(selected_server_ip_address, sizeof(selected_server_ip_address), "%s", room_ip);
            client_ip_address.clear();
            for (int i = 0; i < received_data.id_len; ++i)
                client_ip_address[std::string(received_data.room_master_id)] =
                    std::string(received_data.id[i]);
            is_game_start = true;
            break;
        }
    }

    return is_game_start;
}

void IpResolver::finish()
{
    delete ip_resolver_renderer;
    delete ip_resolver_network;
    delete ip_resolver_input_handler;
}

/**
 * @brief 저장된 클라이언트 ip 주소를 키에 따라 반환하는 함수
 */
const char* IpResolver::get_client_ip_address(string key)
{
    return client_ip_address[key].c_str();
}

/**
 * @brief 저장된 클라이언트 id들을 반환하는 함수
 */
std::vector<std::pair<std::string, std::string>> IpResolver::get_client_ids_ips()
{
    std::vector<std::pair<std::string, std::string>> v;
    for (const auto& [key, value] : client_ip_address)
        v.push_back(std::make_pair(key, value));

    return v;
}

/**
 * @brief 저장된 서버 ip 주소를 반환하는 함수
 */
const char* IpResolver::get_server_ip_address() { return selected_server_ip_address; }

/**
 * @brief 저장된 내 id를 반환하는 함수
 */
const char* IpResolver::get_my_id() { return my_id; }