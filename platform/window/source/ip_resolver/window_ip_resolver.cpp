#include "ip_resolver/window_ip_resolver.hpp"
#include <WinSock2.h>
#include <iphlpapi.h>
#include <conio.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <cstring>
#include <vector>

using namespace std;


/**
 * @brief 현재 어떤 모드로 들어갈지 확인하는 함수
 */
bool WindowIpResolver::start()
{
    int choice;

    while (true) {
        printf("1. open room, 2. enter room\n");
        cin >> choice;
        if (choice == 1) {
            if(open_room())
                return true;
        }
        else if (choice == 2) {
            if(enter_room())
                return false;
        }
    }
}

/**
 * @brief (서버)방을 열고 다른 사용자들의 ip 주소를 저장하게 하는 함수
 */
bool WindowIpResolver::open_room()
{
    WSADATA wsa_data;
    char buffer[1024];
    char broadcast_ip[16];
    std::chrono::steady_clock::time_point base_time;
    int index = 0;
    int room_user_index = 0;
    bool is_game_start = false;
    string s;
    char ip[16];

    memset(my_id, 0, sizeof(my_id));
    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();
    ip_resolver_network->find_broadcast_ip(broadcast_ip);

    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "\e[?25l";
    cout << "Please enter USER ID(8): ";
    cout << flush;
    cin >> my_id;
    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "Press Any key + enter for start game or Press q + enter for get out from room" << '\n';
    
    base_time = std::chrono::steady_clock::now();
    while (true) {
        if (_kbhit() != 0) {
            cin >> s;
            if (s == "q") {
                cout << "\033[2J\033[1;1H";
                cout << flush;
                ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0, 0, 0, 1, broadcast_ip);
                is_game_start = false;
                break;
            }
            else {
                ip_resolver_network->send_multi_udp(my_id, client_ip_address,
                                                    client_ip_address.size(), 0, 1, 0, 0, 0, client_ip_address);
                ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0, 0, 0, 1, broadcast_ip);
                is_game_start = true;
                break;
            }
        }

        if (std::chrono::steady_clock::now() - base_time >= std::chrono::milliseconds(5000)) {
            base_time = std::chrono::steady_clock::now();
            ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 0, 0, 1, 0, 0, broadcast_ip);
        }

        memset(ip, 16, sizeof(ip));
        user_data received_data;
        if (ip_resolver_network->recv_udp(received_data, ip) == false) continue;

        if (index >= 4 || (received_data.is_enter == true && client_ip_address.find(
                               std::string(received_data.id)) != client_ip_address.end()))
            ip_resolver_network->send_udp(my_id, client_ip_address, client_ip_address.size(), 1, 0, 0, 0, 0, broadcast_ip);
        else {
            if (received_data.is_enter == true)
                client_ip_address[std::string(received_data.id)] = std::string(ip);
            else
                client_ip_address.erase(std::string(received_data.id));
            index = client_ip_address.size();
            cout << "\033[2J\033[1;1H";
            cout << flush;
            cout << my_id << '\n';
            for (const auto& [key, value] : client_ip_address)
                cout << key << '\n';
            cout << "Press Any key + enter for start game or Press q + enter for get out from room" << '\n';
            ip_resolver_network->send_multi_udp(my_id, client_ip_address, client_ip_address.size(),
                                                0, 0, 0, 1, 0, client_ip_address);
        }
    }

    return is_game_start;
}

/**
 * @brief (클라이언트)방에 입장하는 함수
 */
bool WindowIpResolver::enter_room()
{
    int room_user_index = 0;
    bool is_in_room = false;
    string server_ip;
    bool is_game_start = false;
    char room_ip[16];

    memset(my_id, 0, sizeof(my_id));
    memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
    client_ip_address.clear();
    server_ip_address.clear();

    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "\e[?25l";
    cout << "Please enter USER ID(8): ";
    cin >> my_id;
    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "Get Room constantly...(exit q + enter)" << '\n';
    
    while (true) {
        if (_kbhit() != 0) {
            cin >> server_ip;
            if (is_in_room == false && server_ip == "q")
            {
                cout << "\033[2J\033[1;1H";
                cout << flush;
                is_game_start = false;
                break;
            }
            else if (is_in_room)
            {
                ip_resolver_network->send_udp(my_id, false, selected_server_ip_address);
                is_in_room = false;
                cout << "\033[2J\033[1;1H";
                cout << flush;
                for (const auto& [key, value] : server_ip_address)
                    cout << value << " : " << key << '\n';
                cout << "Get Room constantly...(exit q + enter)" << '\n';
            }
            else
            {
                if (server_ip_address.find(server_ip) == server_ip_address.end()) continue;
                ip_resolver_network->send_udp(my_id, true, server_ip.c_str());
                is_in_room = true;
            }
        }

        memset(room_ip, 16, sizeof(room_ip));
        room_data received_data;
        if (ip_resolver_network->recv_udp(received_data, room_ip) == false) continue;

        if (received_data.is_broadcast &&
            server_ip_address.find(std::string(room_ip)) ==
            server_ip_address.end()) {
            server_ip_address[std::string(room_ip)] = std::string(received_data.room_master_id);
            if (is_in_room == false)
            {
                cout << "\033[2J\033[1;1H";
                cout << flush;
                for (const auto& [key, value] : server_ip_address)
                    cout << value << " : " << key << '\n';
                cout << "Get Room constantly...(exit q + enter)" << '\n';
            }
        }
        else if (received_data.is_broadcast_delete)
        {
            server_ip_address.erase(std::string(room_ip));
            if (is_in_room == false ||
                (is_in_room == true && strcmp(selected_server_ip_address, room_ip) == 0)) {
                cout << "\033[2J\033[1;1H";
                cout << flush;
                for (const auto& [key, value] : server_ip_address)
                    cout << value << " : " << key << '\n';
                cout << "Get Room constantly...(exit q + enter)" << '\n';
            }
        }
        else if (received_data.is_update) {
            memset(selected_server_ip_address, 0, sizeof(selected_server_ip_address));
            snprintf(selected_server_ip_address, sizeof(selected_server_ip_address), "%s", room_ip);
            client_ip_address.clear();
            for (int i = 0; i < received_data.id_len; ++i)
                client_ip_address[std::string(received_data.room_master_id)] =
                    std::string(received_data.id[i]);
            cout << "\033[2J\033[1;1H";
            cout << flush;
            cout << "ROOM HOST: " << received_data.room_master_id << '\n';
            for (const auto& [key, value] : client_ip_address)
                cout << value << '\n';
            cout << "Press Any key + enter for get out room" << '\n';
            is_in_room = true;
        }
        else if (received_data.is_enter_not_success) {
            server_ip_address.erase(std::string(room_ip));
            cout << "\033[2J\033[1;1H";
            cout << flush;
            for (const auto& [key, value] : server_ip_address)
                cout << value << " : " << key << '\n';
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

/**
 * @brief 저장된 클라이언트 ip 주소를 키에 따라 반환하는 함수
 */
const char* WindowIpResolver::get_client_ip_address(string key)
{
    return client_ip_address[key].c_str();
}

/**
 * @brief 저장된 클라이언트 id들을 반환하는 함수
 */
std::vector<std::pair<std::string, std::string>> WindowIpResolver::get_client_ids_ips()
{ 
    std::vector<std::pair<std::string, std::string>> v;
    for (const auto& [key, value] : client_ip_address)
        v.push_back(std::make_pair(key, value));

    return v;
}

/**
 * @brief 저장된 서버 ip 주소를 반환하는 함수
 */
const char* WindowIpResolver::get_server_ip_address()
{ return selected_server_ip_address; }

/**
 * @brief 저장된 내 id를 반환하는 함수
 */
const char* WindowIpResolver::get_my_id()
{ return my_id; }