#ifndef __I_NETWORK_HPP__
#define __I_NETWORK_HPP__

#include "util/network_packet.hpp"
#include "board/board.hpp"
#include <string>

class INetwork
{
<<<<<<< HEAD
    public:
    virtual void send_udp(const Board& board, const Tetromino& tetromino,const int deleted_line, const char* another_user_ip) = 0;
    
    virtual bool recv_udp(packet& recv_pkt) = 0;
    
    ~INetwork() = default;
=======
  public:
    virtual void send_udp(const Board& board, const Tetromino& tetromino, int deleted_line,
                          const char* another_user_ip) = 0;

    virtual bool recv_udp(Packet& recv_pkt) = 0;

    virtual ~INetwork() = default;
>>>>>>> c7e1a86a78dd0967e2bebb2f8d87d13b5ead2365
};

#endif
