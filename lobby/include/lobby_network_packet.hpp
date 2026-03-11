#ifndef __LOBBY_PACKET_HPP__
#define __LOBBY_PACKET_HPP__

#include <cstdint>
#define USER_DATA_SIZE (4 + 13 + 4 + 101)
#define ROOM_DATA_SIZE (4 + 69 + 9 + 4 + 9 + 101)
#define IDSIZE 9
#define MAXUSER 4
#define ROOMNAMESIZE 9
#define COMMENTSIZE 101

#define USER_DATA_MAGIC 0x55534552
#define ROOM_DATA_MAGIC 0x524F4F4D

typedef struct _user_data
{
    int32_t magic;
    char id[IDSIZE];
    int32_t is_enter; // true == enter, false == out
    int32_t is_chat;
    char comment[COMMENTSIZE];
} user_data;

typedef struct _room_data
{
    int32_t magic;
    char room_master_id[IDSIZE];
    char id[MAXUSER][IDSIZE];
    char room_name[ROOMNAMESIZE];
    int32_t id_len;
    int32_t is_enter_not_success;
    int32_t is_game_start;
    int32_t is_broadcast;
    int32_t is_update;
    int32_t is_broadcast_delete;
    int32_t is_chat;
    char comment_id[IDSIZE];
    char comment[COMMENTSIZE];
} room_data;

#endif
