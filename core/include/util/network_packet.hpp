#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <cstdint>

enum PacketInfo
{
    PACKET_SIZE = ((20 * 10 * 4) + (7 * 4) + 9),
    BUFFER_SIZE = 1024,
    PACKET_MAGIC = 0x5041434B,
    PACKET_MAGIC_SIZE = 4,
    PACKET_ID_SIZE = 9
};

/*
serialization packet
magic 4byte
flag bit 4byte : 0 => board / 1 => type / 2 => rotation / 3 => r / 4 => c / 5 => deleted_line / 6 => is_game_over / 7 => is_win / 8 => id
data ? byte 
uint8_t board[20][10]; => compress len + compress data
uint8_t type; 
uint8_t rotation;
uint8_t r; 
uint8_t c;
uint8_t deleted_line;
uint8_t is_game_over;
uint8_t is_win;
uint8_t id_len;
char id[9];
*/

enum FlagBitInfo : uint32_t
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
    int32_t board[20][10];
    int32_t type;
    int32_t rotation;
    int32_t r;
    int32_t c;
    int32_t deleted_line;
    int32_t is_game_over;
    int32_t is_win;
    char id[9];
} Packet;

#endif
