#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <cstdint>

#pragma pack(push, 1)
typedef struct Packet {
    int32_t board[20][10];
    int32_t type;
    int32_t rotation;
    int32_t r;
    int32_t c;
}packet;
#pragma pack(pop)

#endif