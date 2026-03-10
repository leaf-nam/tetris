#ifndef __LOBBY_PACKET_HPP__
#define __LOBBY_PACKET_HPP__

#include <cstdint>
#define USER_DATA_SIZE (4 + 13)
#define ROOM_DATA_SIZE (4 + 69 + 9)

#define USER_DATA_MAGIC 0x55534552
#define ROOM_DATA_MAGIC 0x524F4F4D

typedef struct _user_data
{
    int32_t magic;
    char id[9];
    int32_t is_enter; // true == enter, false == out
} user_data;

typedef struct _room_data
{
    int32_t magic;
    char room_master_id[9];
    char id[4][9];
    char room_name[9];
    int32_t id_len;
    int32_t is_enter_not_success;
    int32_t is_game_start;
    int32_t is_broadcast;
    int32_t is_update;
    int32_t is_broadcast_delete;
} room_data;

#endif
