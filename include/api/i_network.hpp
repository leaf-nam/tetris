#ifndef __I_NETWORK_HPP__
#define __I_NETWORK_HPP__

#include "util/network_packet.hpp"
#include "board/board.hpp"
#include <string>

class INetwork
{
    public:
    virtual void send_udp(const Board& board, const Tetromino& tetromino, const int deleted_line, const char* another_user_ip) = 0;
    
    virtual bool recv_udp(packet& recv_pkt) = 0;
    
    ~INetwork() = default;
};

#endif
