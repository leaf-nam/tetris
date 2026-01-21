#include "network/window_network.hpp"

#include <iostream>

using namespace std;

// [window] 생성자
WindowNetwork::WindowNetwork()
{
    // 1. 윈도우 소켓 초기화 (필수)
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        cerr << "WSAStartup failed\n";
        exit(0);
    }

    // 2. Client Socket 생성 (송신용)
    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock == INVALID_SOCKET) {
        cerr << "client socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        exit(0);
    }

    // 3. Server Socket 생성 (수신용)
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock == INVALID_SOCKET) {
        cerr << "server socket creation failed: " << WSAGetLastError() << "\n";
        closesocket(client_sock);
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
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (SOCKADDR*) &addr, sizeof(addr)) == SOCKET_ERROR) {
        cerr << "bind failed: " << WSAGetLastError() << "\n";
        exit(0);
    }

    // window에는 epoll이 없으므로 epoll 생성 코드는 삭제됩니다.
    // 대신 recv_udp에서 논블로킹 소켓의 특성을 이용합니다.
}

void WindowNetwork::write_32b(uint8_t*& p, int32_t v)
{
    int32_t n = htonl(v);
    memcpy(p, &n, 4);
    p += 4;
}

void WindowNetwork::write_bytes(uint8_t*& p, const void* data, size_t size)
{
    memcpy(p, data, size);
    p += size;
}

void WindowNetwork::serialize(uint8_t* buf, const Packet& pkt)
{
    uint8_t* p = buf;

    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 10; ++j)
            write_32b(p, pkt.board[i][j]);

    write_32b(p, pkt.type);
    write_32b(p, pkt.rotation);
    write_32b(p, pkt.r);
    write_32b(p, pkt.c);
    write_32b(p, pkt.deleted_line);
    write_32b(p, pkt.is_game_over);

    write_bytes(p, pkt.id, 9);
}

int32_t WindowNetwork::read_32b(const uint8_t*& p)
{
    int32_t n;
    memcpy(&n, p, 4);
    p += 4;
    return ntohl(n);
}

void WindowNetwork::read_bytes(const uint8_t*& p, void* dst, size_t size)
{
    memcpy(dst, p, size);
    p += size;
}

void WindowNetwork::deserialize(const uint8_t* buf, Packet& pkt)
{
    const uint8_t* p = buf;

    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 10; ++j)
            pkt.board[i][j] = read_32b(p);

    pkt.type = read_32b(p);
    pkt.rotation = read_32b(p);
    pkt.r = read_32b(p);
    pkt.c = read_32b(p);
    pkt.deleted_line = read_32b(p);
    pkt.is_game_over = read_32b(p);

    read_bytes(p, pkt.id, 9);
    pkt.id[8] = '\0';
}

void WindowNetwork::send_udp(const Board& board, const Tetromino& tetromino, int deleted_line, int is_game_over,
                             const char* another_user_ip, const char* my_id)
{
    Packet pkt{};
    auto [pos_r, pos_c] = tetromino.get_pos();
    uint8_t buf[PACKET_SIZE];
    SOCKADDR_IN another_user;
    ZeroMemory(&another_user, sizeof(another_user));
    another_user.sin_family = AF_INET;
    another_user.sin_port = htons(PORT);

    // window에서는 inet_pton 사용 시 <WS2tcpip.h> 필요
    inet_pton(AF_INET, another_user_ip, &another_user.sin_addr);

    // 보드 데이터 복사
    for (int r = 0; r < 20; ++r)
        for (int c = 0; c < 10; ++c)
            pkt.board[r][c] = board.at(r + 2, c); // 숨겨진 2줄 제외하고 복사

    pkt.type = tetromino.get_mino_type();
    pkt.rotation = tetromino.get_rotation();
    pkt.r = pos_r;
    pkt.c = pos_c;
    pkt.deleted_line = deleted_line;
    pkt.is_game_over = is_game_over;
    snprintf(pkt.id, sizeof(pkt.id), "%s", my_id);

    serialize(buf, pkt);

    int send_result = sendto(client_sock, (char*) buf, PACKET_SIZE, 0, (SOCKADDR*) &another_user,
                             sizeof(another_user));

    if (send_result == SOCKET_ERROR) {
        cerr << "sendto failed: " << WSAGetLastError() << "\n";
    }
}

void WindowNetwork::send_multi_udp(
    const Board& board, const Tetromino& tetromino, int deleted_line,
    int is_game_over, const char* my_id,
    std::vector<std::pair<std::string, std::string>> ids_ips)
{
    for (const auto& [id, ip] : ids_ips)
        send_udp(board, tetromino, deleted_line, is_game_over, ip.c_str(), my_id);
}

void WindowNetwork::send_relay_udp(const Packet& packet,
                                   std::vector<std::pair<std::string, std::string>> ids_ips)
{
    char* another_user_ip;
    uint8_t buf[PACKET_SIZE];
    SOCKADDR_IN another_user;

    for (const auto& [id, ip] : ids_ips) {
        if (strcmp(id.c_str(), packet.id) == 0) continue;
        Packet pkt{};
        memset(buf, 0, sizeof(buf));
        ZeroMemory(&another_user, sizeof(another_user));
        another_user.sin_family = AF_INET;
        another_user.sin_port = htons(PORT);

        // window에서는 inet_pton 사용 시 <WS2tcpip.h> 필요
        inet_pton(AF_INET, ip.c_str(), &another_user.sin_addr);

        // 보드 데이터 복사
        for (int r = 0; r < 20; ++r)
            for (int c = 0; c < 10; ++c)
                pkt.board[r][c] = packet.board[r][c]; // 숨겨진 2줄 제외하고 복사

        pkt.type = packet.type;
        pkt.rotation = packet.rotation;
        pkt.r = packet.r;
        pkt.c = packet.c;
        pkt.deleted_line = packet.deleted_line;
        snprintf(pkt.id, sizeof(pkt.id), "%s", packet.id);

        serialize(buf, pkt);

        int send_result = sendto(client_sock, (char*) buf, PACKET_SIZE, 0,
                                 (SOCKADDR*) &another_user, sizeof(another_user));

        if (send_result == SOCKET_ERROR) {
            cerr << "sendto failed: " << WSAGetLastError() << "\n";
        }
    }
}

bool WindowNetwork::recv_udp(Packet& recv_pkt)
{
    uint8_t buf[PACKET_SIZE];
    bool data_received = false;
    SOCKADDR_IN client_addr;
    int addr_len = sizeof(client_addr);

    // [window Non-blocking 처리]
    // epoll 대신 루프를 돌며 쌓인 패킷을 모두 처리하고 가장 최신 것을 가져옵니다.
    while (true) {
        int r =
            recvfrom(server_sock, (char*) buf, PACKET_SIZE, 0, (SOCKADDR*) &client_addr, &addr_len);

        if (r == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK) {
                // 더 이상 읽을 데이터가 없음 (버퍼 비워짐)
                break;
            }
            else {
                // 진짜 에러 발생
                cerr << "recvfrom failed: " << err << "\n";
                return false;
            }
        }

        // 데이터 수신 성공
        deserialize(buf, recv_pkt);
        data_received = true;
    }

    return data_received;
}

WindowNetwork::~WindowNetwork()
{
    // 소켓 닫기
    if (client_sock != INVALID_SOCKET) closesocket(client_sock);
    if (server_sock != INVALID_SOCKET) closesocket(server_sock);

    // 윈도우 소켓 정리
    WSACleanup();
}