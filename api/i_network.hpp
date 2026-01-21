#ifndef __I_NETWORK_HPP__
#define __I_NETWORK_HPP__

#include "board/board.hpp"
#include "util/network_packet.hpp"

#include <string>

class INetwork
{
  public:
    virtual void send_udp(const Board& board, const Tetromino& tetromino, int deleted_line,
                          const char* another_user_ip, const char* my_id) = 0;

    virtual void send_relay_udp(const Packet& packet, const char* another_user_ip) = 0;

    virtual bool recv_udp(Packet& recv_pkt) = 0;

    virtual ~INetwork() = default;
};

#endif
