#include "terminal_ip_resolver.hpp"
#include <WinSock2.h>
#include <conio.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

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
    SOCKADDR_IN my_addr;
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
    cout << "사용자 ID를 입력해주세요(최대 8자): ";
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
    inet_pton(AF_INET, "255.255.255.255", &broadcast_addr.sin_addr);

    // 7. Bind
    if (bind(room_sock, (SOCKADDR*) &room_addr, sizeof(room_addr)) == SOCKET_ERROR) {
        cerr << "bind failed: " << WSAGetLastError() << "\n";
        exit(0);
    }

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
                send_room_data.is_enter_success = false;
                send_room_data.is_game_start = true;
                room_user_index = 0;
                for (const auto& [key, value] : client_ip_address) {
                    if (room_user_index >= 4) break;
                    snprintf(send_room_data.id[room_user_index++], sizeof(send_room_data.id[0]),
                             "%s", key.c_str());
                }
                sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                       (SOCKADDR*) &other_user_addr, sizeof(other_user_addr));
            }
            break;
        }
        else
        {
            cout << "\033[2J\033[1;1H";
            cout << flush;
            cout << "\e[?25l";
            cout << open_room_id << '\n';
            for (const auto& [key, value] : client_ip_address)
                cout << key << '\n';
            cout << "게임 시작을 원하시면 아무키나 눌러주세요" << '\n';
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
        if (index >= 4) {
            ZeroMemory(&other_user_addr, sizeof(other_user_addr));
            other_user_addr_len = sizeof(other_user_addr);
            other_user_addr.sin_family = AF_INET;
            other_user_addr.sin_port = htons(ROOM_PORT);
            inet_pton(AF_INET, ip, &other_user_addr.sin_addr);
            room_data send_room_data{};
            send_room_data.is_enter_success = false;
            send_room_data.is_game_start = false;
            sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                   (SOCKADDR*) &other_user_addr,
                   sizeof(other_user_addr));
        }
        else {
            if (received_data.is_enter == true)
                client_ip_address[std::string(received_data.id)] = std::string(ip);
            else
                client_ip_address.erase(std::string(received_data.id));
            ZeroMemory(&other_user_addr, sizeof(other_user_addr));
            other_user_addr_len = sizeof(other_user_addr);
            other_user_addr.sin_family = AF_INET;
            other_user_addr.sin_port = htons(ROOM_PORT);
            inet_pton(AF_INET, ip, &other_user_addr.sin_addr);
            room_data send_room_data{};
            snprintf(send_room_data.room_master_id, sizeof(send_room_data.room_master_id),
                          "%s", open_room_id);
            send_room_data.is_enter_success = true;
            send_room_data.is_game_start = false;
            room_user_index = 0;
            for (const auto& [key, value] : client_ip_address) {
                if (room_user_index >= 4) break;
                snprintf(send_room_data.id[room_user_index++], sizeof(send_room_data.id[0]), "%s",
                         key.c_str());
            }
            sendto(room_sock, (char*) &send_room_data, sizeof(send_room_data), 0,
                   (SOCKADDR*) &other_user_addr, sizeof(other_user_addr));
            index = client_ip_address.size();
        }
    }

    closesocket(room_sock);
}

/**
 * @brief (클라이언트)방에 입장하는 함수
 */
void TerminalIpResolver::enter_room()
{
    //char buf[1024];
    //SOCKADDR_IN another_user;
    //ZeroMemory(&another_user, sizeof(another_user));
    //another_user.sin_family = AF_INET;
    //another_user.sin_port = htons(ROOM_PORT);

    //// 1. Client Socket 생성 (송신용)
    //SOCKET client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    //if (client_sock == INVALID_SOCKET) {
    //    cerr << "client socket creation failed: " << WSAGetLastError() << "\n";
    //    WSACleanup();
    //    exit(0);
    //}

    //// Terminal에서는 inet_pton 사용 시 <WS2tcpip.h> 필요
    //inet_pton(AF_INET, server_ip_address[selected_server_ip_address], &another_user.sin_addr);

    //int send_result = sendto(client_sock, (char*) buf, 1024, 0, (SOCKADDR*) &another_user,
    //                         sizeof(another_user));

    //if (send_result == SOCKET_ERROR) {
    //    cerr << "sendto failed: " << WSAGetLastError() << "\n";
    //}
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