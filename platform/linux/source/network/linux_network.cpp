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

void LinuxNetwork::serialize(uint8_t* buf, const Packet& pkt)
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
    write_32b(p, pkt.is_win);

    write_bytes(p, pkt.id, 9);
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

void LinuxNetwork::deserialize(const uint8_t* buf, Packet& pkt)
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
    pkt.is_win = read_32b(p);

    read_bytes(p, pkt.id, 9);
    pkt.id[8] = '\0';
}

void LinuxNetwork::send_udp(const Board& board, const Tetromino& tetromino, const int deleted_line, int is_game_over, int is_win, const char* another_user_ip, const char* my_id)
{
    Packet pkt;
    uint8_t buf[PACKET_SIZE];
    auto [pos_r, pos_c] = tetromino.get_pos();
    sockaddr_in another_user;
    int send_result;

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

    serialize(buf, pkt);

    send_result = sendto(client_sock, (char*) buf, PACKET_SIZE, 0, (sockaddr*) &another_user, sizeof(another_user));

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
    uint8_t buf[PACKET_SIZE];
    int send_result;

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

        serialize(buf, pkt);

        send_result = sendto(client_sock, (char*) buf, PACKET_SIZE, 0, (sockaddr*) &another_user, sizeof(another_user));

        if (send_result < 0) {
            perror("sendto failed: ");
        }
    }
}

bool LinuxNetwork::recv_udp(Packet& recv_pkt)
{
    uint8_t buf[PACKET_SIZE];
    bool data_received = false;
    int n = epoll_wait(epfd, events, MAX_EVENTS, 0);
    int r;

    if (n < 0) {
        if (errno == EINTR) return false;
        perror("epoll_wait");
        exit(0);
    }
    if (n == 0) return false;

    for (int i = 0; i < n; i++) {
        if ((events[i].events & EPOLLIN) != 0) {
            while (true) {
                sockaddr_in client{};
                socklen_t len = sizeof(client);

                r = recvfrom(server_sock, (char*) buf, PACKET_SIZE, 0, (sockaddr*) &client, &len);

                if (r < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // 수신 버퍼 비움
                        break;
                    }
                    else {
                        perror("recvfrom");
                        return false;
                    }
                }
                else if (r != PACKET_SIZE)
                    return false;

                deserialize(buf, recv_pkt);
                data_received = true;
            }
        }
    }
    return data_received;
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
