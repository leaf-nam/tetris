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
void TerminalIpResolver::open_server(bool is_open_server)
{
    WSADATA wsa_data;
    char buf[1024];
    SOCKADDR_IN client_addr;
    SOCKADDR_IN another_user;
    SOCKET client_sock;
    int addr_len = sizeof(client_addr);
    int index = 0;
    BOOL enable = TRUE;

    // 1. 윈도우 소켓 초기화 (필수)
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        exit(0);
    }

    // 2. Client Socket 생성 (송신용)
    if (is_open_server) {
        ZeroMemory(&another_user, sizeof(another_user));
        SOCKADDR_IN caddr{};
        another_user.sin_family = AF_INET;
        another_user.sin_port = htons(ROOM_PORT);
        client_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (client_sock == INVALID_SOCKET) {
            cerr << "client socket creation failed: " << WSAGetLastError() << "\n";
            WSACleanup();
            exit(0);
        }
        setsockopt(client_sock, SOL_SOCKET, SO_BROADCAST, (char*) &enable, sizeof(enable));
        caddr.sin_family = AF_INET;
        caddr.sin_port = 0; // 자동 포트
        caddr.sin_addr.s_addr = INADDR_ANY;
        bind(client_sock, (SOCKADDR*) &caddr, sizeof(caddr));
        inet_pton(AF_INET, "255.255.255.255", &another_user.sin_addr);
    }

    // 3. Server Socket 생성 (수신용)
    SOCKET server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock == INVALID_SOCKET) {
        WSACleanup();
        exit(0);
    }

    // 4. Server Socket을 Non-blocking 모드로 설정
    // (리눅스의 SOCK_NONBLOCK 대체)
    u_long mode = 1; // 1: Non-blocking, 0: Blocking
    if (ioctlsocket(server_sock, FIONBIO, &mode) == SOCKET_ERROR) {
        cerr << "ioctlsocket failed: " << WSAGetLastError() << "\n";
        exit(0);
    }

    // 5. Bind
    SOCKADDR_IN addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ROOM_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    enable = TRUE;
    setsockopt(server_sock, SOL_SOCKET, SO_BROADCAST, (char*) &enable, sizeof(enable));

    if (bind(server_sock, (SOCKADDR*) &addr, sizeof(addr)) == SOCKET_ERROR) {
        cerr << "bind failed: " << WSAGetLastError() << "\n";
        exit(0);
    }

    // [Terminal Non-blocking 처리]
    // epoll 대신 루프를 돌며 쌓인 패킷을 모두 처리하고 가장 최신 것을 가져옵니다.
    auto base_time = std::chrono::steady_clock::now();
    while (index < 4 || !is_open_server) {
        if (_kbhit() != 0 && !is_open_server) {
            cin >> selected_server_ip_address;
            selected_server_ip_address -= 1;
            if (selected_server_ip_address >= 0 && selected_server_ip_address < 1024)
                break;
        }

        addr_len = sizeof(client_addr);
        int r =
            recvfrom(server_sock, (char*) buf, 1024, 0, (SOCKADDR*) &client_addr, &addr_len);

        int err = (r == SOCKET_ERROR) ? WSAGetLastError() : 0;

        if (std::chrono::steady_clock::now() - base_time >= std::chrono::milliseconds(5000) &&
            is_open_server) {
            base_time = std::chrono::steady_clock::now();
            int send_result = sendto(client_sock, "BROADCAST", strlen("BROADCAST") + 1, 0,
                                     (SOCKADDR*) &another_user,
                                     sizeof(another_user));
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

        buf[r] = '\0';
        // 데이터 수신 성공
        if (is_open_server) {
            if (strcmp(buf, "BROADCAST") == 0) {
                cout << "BROADCAST" << '\n';
                continue;
            }
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip_address[index++],
                      sizeof(client_ip_address[0]));
            cout << client_ip_address[index - 1] << '\n';
        }
        else {
            inet_ntop(AF_INET, &client_addr.sin_addr, server_ip_address[index++],
                      sizeof(server_ip_address[0]));
            cout << server_ip_address[index - 1] << '\n';
            if (index == 1024) index = 0;
        }
    }
}

/**
 * @brief (클라이언트)방에 입장하는 함수
 */
void TerminalIpResolver::enter_room()
{
    char buf[1024];
    SOCKADDR_IN another_user;
    ZeroMemory(&another_user, sizeof(another_user));
    another_user.sin_family = AF_INET;
    another_user.sin_port = htons(ROOM_PORT);

    // 1. Client Socket 생성 (송신용)
    SOCKET client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock == INVALID_SOCKET) {
        cerr << "client socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        exit(0);
    }

    // Terminal에서는 inet_pton 사용 시 <WS2tcpip.h> 필요
    inet_pton(AF_INET, server_ip_address[selected_server_ip_address], &another_user.sin_addr);

    int send_result = sendto(client_sock, (char*) buf, 1024, 0, (SOCKADDR*) &another_user,
                             sizeof(another_user));

    if (send_result == SOCKET_ERROR) {
        cerr << "sendto failed: " << WSAGetLastError() << "\n";
    }
}

/**
 * @brief 저장된 클라이언트 ip 주소들을 반환하는 함수
 */
char (*TerminalIpResolver::get_client_ip_address())[16] { return client_ip_address; }

/**
 * @brief 저장된 서버 ip 주소를 반환하는 함수
 */
char (*TerminalIpResolver::get_server_ip_address())[16] { return server_ip_address; }