#ifndef __I_NETWORK_HPP__
#define __I_NETWORK_HPP__

#include "board/board.hpp"
#include "util/network_packet.hpp"

#include <string>
#include <vector>

#define PORT 41234

class INetwork
{
  public:
    virtual void send_udp(const Board& board, const Tetromino& tetromino, int deleted_line,
                          int is_game_over, int is_win,
                          const char* another_user_ip, const char* my_id) = 0;

    virtual void send_multi_udp(const Board& board, const Tetromino& tetromino, int deleted_line,
                                int is_game_over, int is_win, const char* my_id,
                                std::vector<std::pair<std::string, std::string>> ids_ips) = 0;

    virtual void send_relay_udp(const Packet& packet,
                                std::vector<std::pair<std::string, std::string>> ids_ips) = 0;

    virtual bool recv_udp(Packet& recv_pkt) = 0;

    virtual ~INetwork() = default;
};

#endif
