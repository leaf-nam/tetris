#include "i_network.hpp"

class MockNetwork : public INetwork
{
    void send_udp(const Board& board, const Tetromino& tetromino, int deleted_line, const char* another_user_ip) override {}

    bool recv_udp(Packet& recv_pkt) override
    {
        return false;
    }
};