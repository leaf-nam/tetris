#include "network/linux_network.hpp"

#include <iostream>
#include <stdlib.h>

LinuxNetwork::LinuxNetwork()
{
    // make client socket
    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock < 0) {
        perror("socket");
        exit(0);
    }

    // make_server_socket
    server_sock = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(0);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(0);
    }

    // make epoll
    epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        exit(0);
    }

    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, server_sock, &ev) < 0) {
        perror("epoll_ctl");
        exit(0);
    }
}

void LinuxNetwork::write_32b(uint8_t*& p, int32_t v)
{
    int32_t n = htonl(v);
    memcpy(p, &n, 4);
    p += 4;
}

void LinuxNetwork::write_bytes(uint8_t*& p, const void* data, size_t size)
{
    memcpy(p, data, size);
    p += size;
}

int32_t LinuxNetwork::read_32b(const uint8_t*& p)
{
    int32_t n;
    memcpy(&n, p, 4);
    p += 4;
    return ntohl(n);
}

void LinuxNetwork::read_bytes(const uint8_t*& p, void* dst, size_t size)
{
    memcpy(dst, p, size);
    p += size;
}

void LinuxNetwork::compress_32b(uint8_t*& p, uint32_t& flag_bit, int32_t v, uint32_t bit_check)
{
    uint8_t itc = 0;

    flag_bit |= bit_check;
    itc = static_cast<uint8_t>(v);
    write_bytes(p, &itc, 1);
}

void LinuxNetwork::compress_bytes(uint8_t*& p, uint32_t& flag_bit, const void* data, size_t size,
                                   uint32_t bit_check)
{
    flag_bit |= bit_check;
    write_bytes(p, data, size);
}

void LinuxNetwork::decompress_32b(const uint8_t*& p, int32_t& v)
{
    uint8_t itc;
    read_bytes(p, &itc, 1);
    v = static_cast<int32_t>(itc);
}

void LinuxNetwork::decompress_bytes(const uint8_t*& p, void* data, size_t size)
{
    read_bytes(p, data, size);
}

uint32_t LinuxNetwork::serialize(uint8_t* buf, const Packet& pkt)
{
    uint8_t* p = buf;
    uint8_t* op = buf;
    uint32_t flag_bit = 0;
    uint8_t len = 0;
    int32_t board_block_num = 0;
    int32_t board_block_type = 0;
    uint8_t compress_board[400];
    uint8_t non_compress_board[200];
    uint8_t* c_board_p = compress_board;
    uint8_t* n_c_board_p = non_compress_board;

    write_32b(p, PACKET_MAGIC);
    op += 4;

    // flag bit
    p += 4;

    board_block_type = pkt.board[0][0];
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (board_block_type == pkt.board[i][j])
                board_block_num++;
            else
            {
                compress_32b(c_board_p, flag_bit, board_block_num, BOARD_BIT);
                compress_32b(c_board_p, flag_bit, board_block_type, BOARD_BIT);
                board_block_num = 1;
                board_block_type = pkt.board[i][j];
            }
            compress_32b(n_c_board_p, flag_bit, pkt.board[i][j], BOARD_BIT);
        }
    }
    compress_32b(c_board_p, flag_bit, board_block_num, BOARD_BIT);
    compress_32b(c_board_p, flag_bit, board_block_type, BOARD_BIT);

    if (c_board_p - compress_board >= 200)
    {
        compress_32b(p, flag_bit, 0, BOARD_BIT);
        compress_bytes(p, flag_bit, non_compress_board, 200, BOARD_BIT);
    }
    else
    {
        compress_32b(p, flag_bit, 1, BOARD_BIT);
        compress_bytes(p, flag_bit, compress_board, c_board_p - compress_board, BOARD_BIT);
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

bool LinuxNetwork::deserialize(const uint8_t* buf, Packet& pkt)
{
    const uint8_t* p = buf;
    uint32_t magic = 0;
    uint32_t flag_bit = 0;
    uint8_t len = 0;
    uint32_t board_size = 200;
    int32_t board_block_num = 0;
    int32_t board_block_type = 0;
    int32_t is_board_compressed = 0;
    auto* board = &pkt.board[0][0];

    magic = read_32b(p);
    if (magic != PACKET_MAGIC) return false;

    flag_bit = read_32b(p);

    memset((void*) &pkt, 0, PACKET_SIZE);

    if (flag_bit & BOARD_BIT) {
        decompress_32b(p, is_board_compressed);

        if (is_board_compressed == 1) {
            while (board_size > 0) {
                decompress_32b(p, board_block_num);
                decompress_32b(p, board_block_type);
                for (int i = 0; i < board_block_num; ++i) {
                    *(board++) = board_block_type;
                }
                board_size -= board_block_num;
            }
        }
        else {
            for (int i = 0; i < 20; ++i)
                for (int j = 0; j < 10; ++j)
                    decompress_32b(p, pkt.board[i][j]);
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

void LinuxNetwork::send_udp(const Board& board, const Tetromino& tetromino, const int deleted_line, int is_game_over, int is_win, const char* another_user_ip, const char* my_id)
{
    Packet pkt;
    uint8_t buf[BUFFER_SIZE];
    auto [pos_r, pos_c] = tetromino.get_pos();
    sockaddr_in another_user;
    int send_result;
    uint32_t buffer_size = 0;

    another_user.sin_family = AF_INET;
    another_user.sin_port = htons(PORT);
    inet_pton(AF_INET, another_user_ip, &another_user.sin_addr);

    for (int r = 0; r < 20; ++r)
        for (int c = 0; c < 10; ++c)
            pkt.board[r][c] = board.at(r + 2, c);
    
    pkt.type = tetromino.get_mino_type();
    pkt.rotation = tetromino.get_rotation();
    pkt.r = pos_r;
    pkt.c = pos_c;
    pkt.deleted_line = deleted_line;
    pkt.is_game_over = is_game_over;
    pkt.is_win = is_win;
    snprintf(pkt.id, sizeof(pkt.id), "%s", my_id);

    buffer_size = serialize(buf, pkt);

    send_result = sendto(client_sock, (char*) buf, buffer_size, 0, (sockaddr*) &another_user,
                         sizeof(another_user));

    if(send_result < 0)
        perror("sendto failed: ");
}

void LinuxNetwork::send_multi_udp(
    const Board& board, const Tetromino& tetromino, int deleted_line, int is_game_over,
    int is_win, const char* my_id,
    std::vector<std::pair<std::string, std::string>> ids_ips)
{
    for (const auto& [id, ip] : ids_ips)
        send_udp(board, tetromino, deleted_line, is_game_over, is_win, ip.c_str(), my_id);
}

void LinuxNetwork::send_relay_udp(const Packet& packet,
                                   std::vector<std::pair<std::string, std::string>> ids_ips)
{
    char* another_user_ip;
    uint8_t buf[BUFFER_SIZE];
    int send_result;
    uint32_t buffer_size = 0;

    for (const auto& [id, ip] : ids_ips) {
        if (strcmp(id.c_str(), packet.id) == 0) continue;
        Packet pkt{};
        memset(buf, 0, sizeof(buf));
        sockaddr_in another_user{};
        another_user.sin_family = AF_INET;
        another_user.sin_port = htons(PORT);

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
        pkt.is_game_over = packet.is_game_over;
        pkt.is_win = packet.is_win;
        snprintf(pkt.id, sizeof(pkt.id), "%s", packet.id);

        buffer_size = serialize(buf, pkt);

        send_result = sendto(client_sock, (char*) buf, buffer_size, 0, (sockaddr*) &another_user,
                             sizeof(another_user));

        if (send_result < 0) {
            perror("sendto failed: ");
        }
    }
}

bool LinuxNetwork::recv_udp(Packet& recv_pkt)
{
    uint8_t buf[BUFFER_SIZE];
    int n = epoll_wait(epfd, events, MAX_EVENTS, 0);
    bool is_deserialize_success = false;
    int r;

    if (n < 0) {
        if (errno == EINTR) return false;
        perror("epoll_wait");
        exit(0);
    }
    if (n == 0) return false;

    for (int i = 0; i < n; i++) {
        if ((events[i].events & EPOLLIN) != 0) {
            sockaddr_in client{};
            socklen_t len = sizeof(client);

            r = recvfrom(server_sock, (char*)buf, BUFFER_SIZE, 0, (sockaddr*) &client, &len);

            if (r < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 수신 버퍼 비움
                    return false;
                }
                else {
                    perror("recvfrom");
                    return false;
                }
            }
            else if (r < PACKET_MAGIC_SIZE)
                return false;

            is_deserialize_success = deserialize(buf, recv_pkt);

            if (is_deserialize_success == false) return false;

            return true;
        }
    }
    return false;
}

LinuxNetwork::~LinuxNetwork()
{
    // close client socket
    close(client_sock);

    // close server socket
    close(server_sock);

    // close epoll
    close(epfd);
}
