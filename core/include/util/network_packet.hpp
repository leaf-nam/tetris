#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <cstdint>

enum PacketInfo
{
    PACKET_SIZE = ((20 * 10 * 4) + (7 * 4) + 9),
    PACKET_MAGIC = 0x5041434B,
    PACKET_MAGIC_SIZE = 4,
    PACKET_ID_SIZE = 9
};

/*
serialilzation packet
magic 4byte
flag bit 4byte : 0 => board / 1 => type / 2 => rotation / 3 => r / 4 => c / 5 => deleted_line / 6 => is_game_over / 7 => is_win / 8 => id
data ? byte
*/

enum FlagBitInfo
{
    BOARD_BIT = (1 << 0),
    TYPE_BIT = (1 << 1),
    ROTATION_BIT = (1 << 2),
    R_BIT = (1 << 3),
    C_BIT = (1 << 4),
    DELETED_LINE_BIT = (1 << 5),
    IS_GAME_OVER_BIT = (1 << 6),
    IS_WIN_BIT = (1 << 7),
    ID_BIT = (1 << 8)
};

typedef struct PacketStruct {
    uint32_t board[20][10];
    uint32_t type;
    uint32_t rotation;
    uint32_t r;
    uint32_t c;
    uint32_t deleted_line;
    uint32_t is_game_over;
    uint32_t is_win;
    char id[9];
} Packet;

#endif
