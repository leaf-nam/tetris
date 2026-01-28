#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <cstdint>
#define PACKET_SIZE (20 * 10 * 4 + 7 * 4 + 9)
#define USER_DATA_SIZE 13
#define ROOM_DATA_SIZE 69

typedef struct PacketStruct {
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

typedef struct _user_data
{
    char id[9];
    int32_t is_enter; // true == enter, false == out
} user_data;

typedef struct _room_data
{
    char room_master_id[9];
    char id[4][9];
    int32_t id_len;
    int32_t is_enter_not_success;
    int32_t is_game_start;
    int32_t is_broadcast;
    int32_t is_update;
    int32_t is_broadcast_delete;
} room_data;

#endif
