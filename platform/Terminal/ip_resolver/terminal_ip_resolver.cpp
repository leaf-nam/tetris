#include "terminal_ip_resolver.hpp"
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

using namespace std;

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

/**
 * @brief (서버)브로드캐스트 주소를 찾는 함수
 */
void TerminalIpResolver::find_broadcast_ip(char* broadcast_ip)
{
    ULONG size = 0;
    GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &size);

    IP_ADAPTER_ADDRESSES* adapters = (IP_ADAPTER_ADDRESSES*) malloc(size);

    if (GetAdaptersAddresses(AF_INET, 0, nullptr, adapters, &size) != NO_ERROR) return;

    for (auto a = adapters; a; a = a->Next) {
        if (a->OperStatus != IfOperStatusUp) continue;

        if (a->IfType == IF_TYPE_SOFTWARE_LOOPBACK) continue;

        if (a->PhysicalAddressLength == 0) continue;

        for (auto u = a->FirstUnicastAddress; u; u = u->Next) {
            SOCKADDR_IN* sa = (SOCKADDR_IN*) u->Address.lpSockaddr;

            uint32_t ip = ntohl(sa->sin_addr.s_addr);
            uint32_t mask = (
                u->OnLinkPrefixLength == 0 ? 0 : (0xFFFFFFFF << (32 - u->OnLinkPrefixLength)));
            uint32_t broadcast = ip | ~mask;
            broadcast = htonl(broadcast);

            IN_ADDR b;
            b.s_addr = broadcast;

            inet_ntop(AF_INET, &b, broadcast_ip, 16);
            return;
        }
    }

    free(adapters);
}

/**
 * @brief (서버)방을 열고 다른 사용자들의 ip 주소를 저장하게 하는 함수
 */
void TerminalIpResolver::open_room()
{
    WSADATA wsa_data;
    char buffer[1024];
    SOCKADDR_IN broadcast_addr;
    SOCKADDR_IN other_user_addr;
    SOCKADDR_IN room_addr;
    int other_user_addr_len;
    SOCKET room_sock;
    char open_room_id[9];
    std::chrono::steady_clock::time_point base_time;
    int index = 0;
    int room_user_index = 0;
    BOOL enable = TRUE;

    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "\e[?25l";
    cout << "Please enter USER ID(8): ";
    cout << flush;
    cin >> open_room_id;

    // 1. 윈도우 소켓 초기화 (필수)
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        exit(0);
    }

    // 2. Room Socket 생성 (수신용)
    room_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (room_sock == INVALID_SOCKET) {
        cerr << "socket failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        exit(0);
    }

    // 3. Room Socket을 Non-blocking 모드로 설정
    u_long mode = 1; // 1: Non-blocking, 0: Blocking
    if (ioctlsocket(room_sock, FIONBIO, &mode) == SOCKET_ERROR) {
        cerr << "ioctlsocket failed: " << WSAGetLastError() << "\n";
        exit(0);
    }
    // 5. Bind data setting
    ZeroMemory(&room_addr, sizeof(room_addr));
    room_addr.sin_family = AF_INET;
    room_addr.sin_port = htons(ROOM_PORT);
    room_addr.sin_addr.s_addr = INADDR_ANY;

    // 6. BroadCast settting
    enable = TRUE;
    setsockopt(room_sock, SOL_SOCKET, SO_BROADCAST, (char*) &enable, sizeof(enable));
    ZeroMemory(&broadcast_addr, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(ROOM_PORT);
    char broadcast_ip[16];
    find_broadcast_ip(broadcast_ip);
    inet_pton(AF_INET, broadcast_ip, &broadcast_addr.sin_addr);

    // 7. Bind
    if (bind(room_sock, (SOCKADDR*) &room_addr, sizeof(room_addr)) == SOCKET_ERROR) {
        cerr << "bind failed: " << WSAGetLastError() << "\n";
        exit(0);
    }

    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "Press Key for start game" << '\n';
    base_time = std::chrono::steady_clock::now();
    while (true) {
        if (_kbhit() != 0) {
            for (const auto& [key, value] : client_ip_address) {
                ZeroMemory(&other_user_addr, sizeof(other_user_addr));
                other_user_addr_len = sizeof(other_user_addr);
                other_user_addr.sin_family = AF_INET;
                other_user_addr.sin_port = htons(ROOM_PORT);
                inet_pton(AF_INET, value.c_str(), &other_user_addr.sin_addr);
                room_data send_room_data{};
                send_room_data.is_game_start = true;
                snprintf(send_room_data.room_master_id, sizeof(send_room_data.room_master_id), "%s",
                         open_room_id);
                room_user_index = 0;
                for (const auto& [key2, value2] : client_ip_address) {
                    if (room_user_index >= 4) break;
                    snprintf(send_room_data.id[room_user_index++], sizeof(send_room_data.id[0]),
                             "%s", key2.c_str());
                }
                send_room_data.id_len = room_user_index;
                sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                       (SOCKADDR*) &other_user_addr, sizeof(other_user_addr));
            }
            room_data send_room_data{};
            snprintf(send_room_data.room_master_id, sizeof(send_room_data.room_master_id), "%s",
                     open_room_id);
            send_room_data.is_broadcast_delete = true;
            sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                   (SOCKADDR*) &broadcast_addr, sizeof(broadcast_addr));
            break;
        }

        ZeroMemory(&other_user_addr, sizeof(other_user_addr));
        other_user_addr_len = sizeof(other_user_addr);
        memset(buffer, 0, sizeof(buffer));
        int r = recvfrom(room_sock, buffer, sizeof(buffer) - 1, 0, (SOCKADDR*) &other_user_addr,
                         &other_user_addr_len);

        int err = (r == SOCKET_ERROR) ? WSAGetLastError() : 0;

        if (std::chrono::steady_clock::now() - base_time >= std::chrono::milliseconds(5000)) {
            base_time = std::chrono::steady_clock::now();
            room_data send_room_data{};
            snprintf(send_room_data.room_master_id, sizeof(send_room_data.room_master_id), "%s",
                     open_room_id);
            send_room_data.is_broadcast = true;
            sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                   (SOCKADDR*) &broadcast_addr,
                   sizeof(broadcast_addr));
        }

        if (r == SOCKET_ERROR) {
            if (err == WSAEWOULDBLOCK) {
                // 더 이상 읽을 데이터가 없음 (버퍼 비워짐)
                continue;
            }
            else {
                // 진짜 에러 발생
                cerr << "recvfrom failed: " << err << "\n";
                return;
            }
        }
        else if (r != sizeof(user_data))
            continue;

        // 데이터 수신 성공
        char ip[16];
        inet_ntop(AF_INET, &other_user_addr.sin_addr, ip, sizeof(ip));
        user_data received_data;
        memcpy(&received_data, buffer, sizeof(received_data));
        received_data.id[sizeof(received_data.id) - 1] = '\0';
        if (index >= 4 || (received_data.is_enter == true && client_ip_address.find(
                               std::string(received_data.id)) != client_ip_address.end())) {
            ZeroMemory(&other_user_addr, sizeof(other_user_addr));
            other_user_addr_len = sizeof(other_user_addr);
            other_user_addr.sin_family = AF_INET;
            other_user_addr.sin_port = htons(ROOM_PORT);
            inet_pton(AF_INET, ip, &other_user_addr.sin_addr);
            room_data send_room_data{};
            snprintf(send_room_data.room_master_id, sizeof(send_room_data.room_master_id), "%s",
                     open_room_id);
            send_room_data.is_enter_not_success = true;
            sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                   (SOCKADDR*) &other_user_addr,
                   sizeof(other_user_addr));
        }
        else {
            if (received_data.is_enter == true)
                client_ip_address[std::string(received_data.id)] = std::string(ip);
            else
                client_ip_address.erase(std::string(received_data.id));
            index = client_ip_address.size();
            cout << "\033[2J\033[1;1H";
            cout << flush;
            cout << open_room_id << '\n';
            for (const auto& [key, value] : client_ip_address)
                cout << key << '\n';
            cout << "Press Key for start game" << '\n';
            for (const auto& [key, value] : client_ip_address) {
                ZeroMemory(&other_user_addr, sizeof(other_user_addr));
                other_user_addr_len = sizeof(other_user_addr);
                other_user_addr.sin_family = AF_INET;
                other_user_addr.sin_port = htons(ROOM_PORT);
                inet_pton(AF_INET, value.c_str(), &other_user_addr.sin_addr);
                room_data send_room_data{};
                send_room_data.is_update = true;
                snprintf(send_room_data.room_master_id, sizeof(send_room_data.room_master_id), "%s",
                         open_room_id);
                room_user_index = 0;
                for (const auto& [key2, value2] : client_ip_address) {
                    if (room_user_index >= 4) break;
                    snprintf(send_room_data.id[room_user_index++], sizeof(send_room_data.id[0]),
                             "%s", key2.c_str());
                }
                send_room_data.id_len = room_user_index;
                sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                       (SOCKADDR*) &other_user_addr, sizeof(other_user_addr));
            }
        }
    }

    closesocket(room_sock);
}

/**
 * @brief (클라이언트)방에 입장하는 함수
 */
void TerminalIpResolver::enter_room()
{
    WSADATA wsa_data;
    char buffer[1024];
    SOCKADDR_IN room_send_addr;
    SOCKADDR_IN room_recv_addr;
    SOCKADDR_IN enter_addr;
    int room_recv_addr_len;
    int room_send_addr_len;
    SOCKET enter_sock;
    char my_id[9];
    int room_user_index = 0;
    BOOL enable = TRUE;
    bool is_in_room = false;
    string server_ip;

    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "\e[?25l";
    cout << "Please enter USER ID(8): ";
    cout << flush;
    cin >> my_id;

    // 1. 윈도우 소켓 초기화 (필수)
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        exit(0);
    }

    // 2. Enter Socket 생성 (수신용)
    enter_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (enter_sock == INVALID_SOCKET) {
        cerr << "socket failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        exit(0);
    }

    // 3. Enter Socket을 Non-blocking 모드로 설정
    u_long mode = 1; // 1: Non-blocking, 0: Blocking
    if (ioctlsocket(enter_sock, FIONBIO, &mode) == SOCKET_ERROR) {
        cerr << "ioctlsocket failed: " << WSAGetLastError() << "\n";
        exit(0);
    }
    // 5. Bind data setting
    ZeroMemory(&enter_addr, sizeof(enter_addr));
    enter_addr.sin_family = AF_INET;
    enter_addr.sin_port = htons(ROOM_PORT);
    enter_addr.sin_addr.s_addr = INADDR_ANY;

    // 6. BroadCast settting
    enable = TRUE;
    setsockopt(enter_sock, SOL_SOCKET, SO_BROADCAST, (char*) &enable, sizeof(enable));

    // 7. Bind
    if (bind(enter_sock, (SOCKADDR*) &enter_addr, sizeof(enter_addr)) == SOCKET_ERROR) {
        cerr << "bind failed: " << WSAGetLastError() << "\n";
        exit(0);
    }

    cout << "\033[2J\033[1;1H";
    cout << flush;
    cout << "Get Room constantly..." << '\n';
    while (true) {
        if (_kbhit() != 0) {
            if (is_in_room)
            {
                while (_kbhit())
                    _getch();
                ZeroMemory(&room_send_addr, sizeof(room_send_addr));
                room_send_addr_len = sizeof(room_send_addr);
                room_send_addr.sin_family = AF_INET;
                room_send_addr.sin_port = htons(ROOM_PORT);
                inet_pton(AF_INET, selected_server_ip_address, &room_send_addr.sin_addr);
                user_data send_user_data{};
                snprintf(send_user_data.id, sizeof(send_user_data.id), "%s", my_id);
                send_user_data.is_enter = false;
                sendto(enter_sock, (char*) &send_user_data, sizeof(send_user_data), 0,
                       (SOCKADDR*) &room_send_addr, sizeof(room_send_addr));
                is_in_room = false;
                cout << "\033[2J\033[1;1H";
                cout << flush;
                for (const auto& [key, value] : server_ip_address)
                    cout << value << " : " << key << '\n';
                cout << "Get Room constantly..." << '\n';
            }
            else
            {
                cin >> server_ip;
                while (_kbhit())
                    _getch();
                if (server_ip_address.find(server_ip) == server_ip_address.end()) continue;
                ZeroMemory(&room_send_addr, sizeof(room_send_addr));
                room_send_addr_len = sizeof(room_send_addr);
                room_send_addr.sin_family = AF_INET;
                room_send_addr.sin_port = htons(ROOM_PORT);
                inet_pton(AF_INET, server_ip.c_str(), &room_send_addr.sin_addr);
                user_data send_user_data{};
                snprintf(send_user_data.id, sizeof(send_user_data.id), "%s", my_id);
                send_user_data.is_enter = true;
                sendto(enter_sock, (char*) &send_user_data, sizeof(send_user_data), 0,
                       (SOCKADDR*) &room_send_addr, sizeof(room_send_addr));
                is_in_room = true;
            }
        }

        ZeroMemory(&room_recv_addr, sizeof(room_recv_addr));
        room_recv_addr_len = sizeof(room_recv_addr);
        memset(buffer, 0, sizeof(buffer));
        int r = recvfrom(enter_sock, buffer, sizeof(buffer) - 1, 0, (SOCKADDR*) &room_recv_addr,
                         &room_recv_addr_len);

        int err = (r == SOCKET_ERROR) ? WSAGetLastError() : 0;

        if (r == SOCKET_ERROR) {
            if (err == WSAEWOULDBLOCK) {
                // 더 이상 읽을 데이터가 없음 (버퍼 비워짐)
                continue;
            }
            else {
                // 진짜 에러 발생
                cerr << "recvfrom failed: " << err << "\n";
                return;
            }
        }
        else if (r != sizeof(room_data))
            continue;

        // 데이터 수신 성공
        char room_ip[16];
        inet_ntop(AF_INET, &room_recv_addr.sin_addr, room_ip, sizeof(room_ip));
        room_data received_data;
        memcpy(&received_data, buffer, sizeof(received_data));
        received_data.room_master_id[sizeof(received_data.room_master_id) - 1] = '\0';
        for (int i = 0; i < received_data.id_len; ++i)
            received_data.id[i][sizeof(received_data.id[0]) - 1] = '\0';
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
                cout << "Get Room constantly..." << '\n';
            }
        }
        else if (received_data.is_broadcast_delete)
        {
            server_ip_address.erase(std::string(room_ip));
            if (is_in_room == false) {
                cout << "\033[2J\033[1;1H";
                cout << flush;
                for (const auto& [key, value] : server_ip_address)
                    cout << value << " : " << key << '\n';
                cout << "Get Room constantly..." << '\n';
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
            cout << "Press Any key for get out room" << '\n';
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
            break;
        }
    }

    closesocket(enter_sock);
}

/**
 * @brief 저장된 클라이언트 ip 주소들을 반환하는 함수
 */
char (*TerminalIpResolver::get_client_ip_address())[16]
{
    return nullptr; // client_ip_address;
}

/**
 * @brief 저장된 서버 ip 주소를 반환하는 함수
 */
char (*TerminalIpResolver::get_server_ip_address())[16]
{
    return nullptr; // server_ip_address;
}