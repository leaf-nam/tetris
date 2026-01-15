#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <cstdint>
#define PACKET_SIZE (20 * 10 * 4 + 5 * 4)

typedef struct PacketStruct {
    int32_t board[20][10];
    int32_t type;
    int32_t rotation;
    int32_t r;
    int32_t c;
    int32_t deleted_line;
} Packet;

#endif
