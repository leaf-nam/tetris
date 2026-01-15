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

    void write_32b(uint8_t*& p, int32_t v);
    void serialize(uint8_t* buf, const Packet& pkt);
    int32_t read_32b(const uint8_t*& p);
    void deserialize(const uint8_t* buf, Packet& pkt);

  public:
    LinuxNetwork();
    void send_udp(const Board& board, const Tetromino& tetromino, int deleted_line,
                  const char* another_user_ip) override;
    bool recv_udp(Packet& recv_pkt) override;
    ~LinuxNetwork();
};

#endif
