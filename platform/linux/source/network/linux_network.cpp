#include "network/linux_network.hpp"
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

    if (bind(server_sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
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

void LinuxNetwork::serialize(packet& pkt)
{
    pkt.type = htonl(pkt.type);
    pkt.rotation = htonl(pkt.rotation);
    pkt.r = htonl(pkt.r);
    pkt.c = htonl(pkt.c);

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 20; ++j)
            pkt.board[i][j] = htonl(pkt.board[i][j]);
}

void LinuxNetwork::deserialize(packet& pkt)
{
    pkt.type = ntohl(pkt.type);
    pkt.rotation = ntohl(pkt.rotation);
    pkt.r = ntohl(pkt.r);
    pkt.c = ntohl(pkt.c);

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 20; ++j)
            pkt.board[i][j] = htonl(pkt.board[i][j]);
}

void LinuxNetwork::send_udp(const Board& board, const Tetromino& tetromino, const char* another_user_ip)
{
    packet pkt;
    auto [pos_r, pos_c] = tetromino.get_pos();    
    sockaddr_in another_user;
    
    another_user.sin_family = AF_INET;
    another_user.sin_port = htons(PORT);
    inet_pton(AF_INET, another_user_ip, &another_user.sin_addr);

    for(int r = 0; r < 20; ++r)
        for(int c = 0; c < 10; ++c)
            pkt.board[r][c] = board.at(r + 2, c);
    pkt.type = tetromino.get_mino_type();
    pkt.rotation = tetromino.get_rotation();
    pkt.r = pos_r;
    pkt.c = pos_c;

    serialize(pkt);

    sendto(
        client_sock,
        &pkt,
        sizeof(pkt),
        0,
        (sockaddr*)&another_user,
        sizeof(another_user)
    );
}

bool LinuxNetwork::recv_udp(packet& recv_pkt)
{
    int n = epoll_wait(epfd, events, MAX_EVENTS, 0);
    if (n < 0) {
        if (errno == EINTR)
            return false;
        perror("epoll_wait");
        exit(0);
    }
    if (n == 0)
        return false;

    for (int i = 0; i < n; i++) {
        if (events[i].events & EPOLLIN) {
            while (true) {
                sockaddr_in client{};
                socklen_t len = sizeof(client);

                int r = recvfrom(
                    server_sock,
                    &recv_pkt,
                    sizeof(recv_pkt),
                    0,
                    (sockaddr*)&client,
                    &len
                );

                if (r < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // 수신 버퍼 비움
                        break;
                    } else {
                        perror("recvfrom");
                        exit(0);
                    }
                }
                    
                deserialize(recv_pkt);
            }
        }
    }
    return true;
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
