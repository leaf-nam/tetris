#ifndef __LINUX_NETWORK_HPP__
#define __LINUX_NETWORK_HPP__

#include <api/i_network.hpp>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 41234
#define MAX_EVENTS 10

class LinuxNetwork : public INetwork
{
    private:
    int client_sock;
    int server_sock;
    int epfd;
    epoll_event events[MAX_EVENTS];

    void serialize(packet& pkt);
    void deserialize(packet& pkt);

    public:
    LinuxNetwork();
    void send_udp(const Board& board, const Tetromino& tetromino, const char* another_user_ip) override;
    bool recv_udp(packet& recv_pkt) override;
    ~LinuxNetwork();
};

#endif

