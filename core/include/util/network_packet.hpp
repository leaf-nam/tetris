#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <cstdint>
#define PACKET_SIZE (4 + (20 * 10 * 4) + (7 * 4) + 9)
#define PACKET_MAGIC 0x5041434B

typedef struct PacketStruct {
    int32_t magic;
    int32_t board[20][10];
    int32_t type;
    int32_t rotation;
    int32_t r;
    int32_t c;
    int32_t deleted_line;
    int32_t is_game_over; // 1 => True, 0 => False
    int32_t is_win;
    char id[9];
} Packet;

#endif
