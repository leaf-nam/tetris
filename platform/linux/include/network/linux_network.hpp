#ifndef __LINUX_NETWORK_HPP__
#define __LINUX_NETWORK_HPP__

#include <i_network.hpp>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 10

class LinuxNetwork : public INetwork
{
  private:
    int client_sock;
    int server_sock;
    int epfd;
    epoll_event events[MAX_EVENTS];

    void write_32b(uint8_t*& p, int32_t v);
    void write_bytes(uint8_t*& p, const void* data, size_t size);
    void serialize(uint8_t* buf, const Packet& pkt);
    int32_t read_32b(const uint8_t*& p);
    void read_bytes(const uint8_t*& p, void* dst, size_t size);
    void deserialize(const uint8_t* buf, Packet& pkt);

  public:
    LinuxNetwork();
    
    // 부모 클래스(INetwork)의 가상 함수 오버라이드
    void send_udp(const Board& board, const Tetromino& tetromino, int deleted_line, int is_game_over, int is_win,
                  const char* another_user_ip, const char* my_id) override;
    void send_multi_udp(const Board& board, const Tetromino& tetromino, int deleted_line, 
                        int is_game_over, int is_win, const char* my_id,
                        std::vector<std::pair<std::string, std::string>> ids_ips) override;
    void send_relay_udp(const Packet& packet,
                        std::vector<std::pair<std::string, std::string>> ids_ips) override;
    bool recv_udp(Packet& recv_pkt) override;
    
    ~LinuxNetwork();
};

#endif
