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

    // 6. Poll
    fds[0].fd = server_sock;
    fds[0].events = POLLIN;
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

void WindowNetwork::compress_32b(uint8_t*& p, uint32_t& flag_bit, int32_t v, uint32_t bit_check)
{
    uint8_t itc = 0;

    flag_bit |= bit_check;
    itc = static_cast<uint8_t>(v);
    write_bytes(p, &itc, 1);
}

void WindowNetwork::compress_bytes(uint8_t*& p, uint32_t& flag_bit, const void* data,
                                   size_t size, uint32_t bit_check)
{
    flag_bit |= bit_check;
    write_bytes(p, data, size);
}

void WindowNetwork::decompress_32b(const uint8_t*& p, int32_t& v)
{
    uint8_t itc;
    read_bytes(p, &itc, 1);
    v = static_cast<int32_t>(itc);
}

void WindowNetwork::decompress_bytes(const uint8_t*& p, void* data, size_t size)
{
    read_bytes(p, data, size);
}

uint32_t WindowNetwork::serialize(uint8_t* buf, const Packet& pkt)
{
    uint8_t* p = buf;
    uint8_t* op = buf;
    uint32_t flag_bit = 0;
    uint8_t len = 0;

    write_32b(p, PACKET_MAGIC);
    op += 4;

    // flag bit
    p += 4;

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            compress_32b(p, flag_bit, pkt.board[i][j], BOARD_BIT);
        }
    }
    compress_32b(p, flag_bit, pkt.type, TYPE_BIT);
    compress_32b(p, flag_bit, pkt.rotation, ROTATION_BIT);
    compress_32b(p, flag_bit, pkt.r, R_BIT);
    compress_32b(p, flag_bit, pkt.c, C_BIT);
    compress_32b(p, flag_bit, pkt.deleted_line, DELETED_LINE_BIT);

    if (pkt.is_game_over == 1) compress_32b(p, flag_bit, pkt.is_game_over, IS_GAME_OVER_BIT);
    if (pkt.is_win == 1) compress_32b(p, flag_bit, pkt.is_win, IS_WIN_BIT);

    len = static_cast<uint8_t>(strlen(pkt.id));
    compress_bytes(p, flag_bit, &len, 1, ID_BIT);
    compress_bytes(p, flag_bit, pkt.id, len, ID_BIT);

    // flag bit
    write_32b(op, flag_bit);

    return (p - buf);
}

bool WindowNetwork::deserialize(const uint8_t* buf, Packet& pkt)
{
    const uint8_t* p = buf;
    uint32_t magic = 0;
    uint32_t flag_bit = 0;
    uint8_t len = 0;

    magic = read_32b(p);
    if (magic != PACKET_MAGIC) return false;

    flag_bit = read_32b(p);

    memset((void*) &pkt, 0, PACKET_SIZE);

    if (flag_bit & BOARD_BIT) {
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 10; ++j) {
                decompress_32b(p, pkt.board[i][j]);
            }
        }
    }
    if (flag_bit & TYPE_BIT) decompress_32b(p, pkt.type);
    if (flag_bit & ROTATION_BIT) decompress_32b(p, pkt.rotation);
    if (flag_bit & R_BIT) decompress_32b(p, pkt.r);
    if (flag_bit & C_BIT) decompress_32b(p, pkt.c);
    if (flag_bit & DELETED_LINE_BIT) decompress_32b(p, pkt.deleted_line);
    if (flag_bit & IS_GAME_OVER_BIT) decompress_32b(p, pkt.is_game_over);
    if (flag_bit & IS_WIN_BIT) decompress_32b(p, pkt.is_win);
    if (flag_bit & ID_BIT) {
        decompress_bytes(p, &len, 1);
        decompress_bytes(p, pkt.id, len);
        pkt.id[len] = '\0';
    }

    return true;
}

void WindowNetwork::send_udp(const Board& board, const Tetromino& tetromino, int deleted_line, int is_game_over, int is_win,
                             const char* another_user_ip, const char* my_id)
{
    Packet pkt{};
    auto [pos_r, pos_c] = tetromino.get_pos();
    uint8_t buf[BUFFER_SIZE];
    SOCKADDR_IN another_user;
    ZeroMemory(&another_user, sizeof(another_user));
    another_user.sin_family = AF_INET;
    another_user.sin_port = htons(PORT);
    uint32_t buffer_size = 0;

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
    pkt.is_win = is_win;
    snprintf(pkt.id, PACKET_ID_SIZE, "%s", my_id);

    buffer_size = serialize(buf, pkt);

    int send_result = sendto(client_sock, (char*)buf, buffer_size, 0, (SOCKADDR*) &another_user,
                             sizeof(another_user));

    if (send_result == SOCKET_ERROR) {
        cerr << "sendto failed: " << WSAGetLastError() << "\n";
    }
}

void WindowNetwork::send_multi_udp(
    const Board& board, const Tetromino& tetromino, int deleted_line, int is_game_over,
    int is_win, const char* my_id,
    std::vector<std::pair<std::string, std::string>> ids_ips)
{
    for (const auto& [id, ip] : ids_ips)
        send_udp(board, tetromino, deleted_line, is_game_over, is_win, ip.c_str(), my_id);
}

void WindowNetwork::send_relay_udp(const Packet& packet,
                                   std::vector<std::pair<std::string, std::string>> ids_ips)
{
    char* another_user_ip;
    uint8_t buf[BUFFER_SIZE];
    SOCKADDR_IN another_user;
    uint32_t buffer_size = 0;
    int send_result = 0;

    for (const auto& [id, ip] : ids_ips) {
        if (strcmp(id.c_str(), packet.id) == 0) continue;
        Packet pkt{};
        memset(buf, 0, sizeof(buf));
        ZeroMemory(&another_user, sizeof(another_user));
        another_user.sin_family = AF_INET;
        another_user.sin_port = htons(PORT);

        // window에서는 inet_pton 사용 시 <WS2tcpip.h> 필요
        inet_pton(AF_INET, ip.c_str(), &another_user.sin_addr);

        for (int r = 0; r < 20; ++r)
            for (int c = 0; c < 10; ++c)
                pkt.board[r][c] = packet.board[r][c]; // 숨겨진 2줄 제외하고 복사

        pkt.type = packet.type;
        pkt.rotation = packet.rotation;
        pkt.r = packet.r;
        pkt.c = packet.c;
        pkt.deleted_line = packet.deleted_line;
        pkt.is_game_over = packet.is_game_over;
        pkt.is_win = packet.is_win;
        snprintf(pkt.id, PACKET_ID_SIZE, "%s", packet.id);

        buffer_size = serialize(buf, pkt);

        send_result = sendto(client_sock, (char*)buf, buffer_size, 0, (SOCKADDR*) &another_user, sizeof(another_user));

        if (send_result == SOCKET_ERROR) {
            cerr << "sendto failed: " << WSAGetLastError() << "\n";
        }
    }
}

bool WindowNetwork::recv_udp(Packet& recv_pkt)
{
    uint8_t buf[BUFFER_SIZE];
    bool data_received = false;
    SOCKADDR_IN client_addr;
    int addr_len = sizeof(client_addr);
    bool is_deserialize_success = false;
    int r;

    // [window Poll 처리]
    int ret = WSAPoll(fds, 1, 0);

    if (ret > 0) {
        if (fds[0].revents & POLLIN) {
            r = recvfrom(server_sock, (char*) buf, BUFFER_SIZE, 0, (SOCKADDR*) &client_addr,
                             &addr_len);

            if (r == SOCKET_ERROR) {
                int err = WSAGetLastError();
                cerr << "recvfrom failed: " << err << "\n";
                return false;
            }
            else if (r < PACKET_MAGIC_SIZE)
                return false;

            // 데이터 수신 성공
            is_deserialize_success = deserialize(buf, recv_pkt);

            if (is_deserialize_success == false) data_received = false;

            data_received = true;
        }
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
