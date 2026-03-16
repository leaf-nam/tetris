#ifndef __LOBBY_PACKET_HPP__
#define __LOBBY_PACKET_HPP__

#include <cstdint>

enum LobbyPacketInfo
{
    USER_DATA_SIZE = (4 + 13 + 4 + 4 + 101),
    ROOM_DATA_SIZE = (4 + 69 + 9 + 4 + 9 + 101),
    LOBBY_BUFFER_SIZE = 1024,
    IDSIZE = 9,
    MAXUSER = 4,
    ROOMNAMESIZE = 9,
    COMMENTSIZE = 101,
    LOBBY_MAGIC_SIZE = 4,
    USER_DATA_MAGIC = 0x55534552,
    ROOM_DATA_MAGIC = 0x524F4F4D
};

/*
serialization packet
magic 4byte
flag bit 4byte : 0 => id / 1 => is_enter / 2 => is_out / 3 => is_chat / 4 => comment 
data ? byte 
uint8_t id_len;
char id[IDSIZE];
uint8_t is_enter;
uint8_t is_out;
uint8_t is_chat;
uint8_t comment_len; 
char comment[COMMENTSIZE];
*/

enum LobbyUserFlagBitInfo : uint32_t
{
    LOBBY_USER_ID_BIT = (1 << 0),
    LOBBY_USER_IS_ENTER_BIT = (1 << 1),
    LOBBY_USER_IS_OUT_BIT = (1 << 2),
    LOBBY_USER_IS_CHAT_BIT = (1 << 3),
    LOBBY_USER_COMMENT_BIT = (1 << 4),
};

typedef struct _user_data
{
    char id[IDSIZE];
    int32_t is_enter;
    int32_t is_out;
    int32_t is_chat;
    char comment[COMMENTSIZE];
} user_data;

/*
serialization packet
magic 4byte
flag bit 4byte : 0 => room_master_id / 1 => id / 2 => room_name / 3 => id_len / 4 => is_enter_not_success / 5 => is_game_start
/ 6 => is_broadcast / 7 => is_update / 8 => is_broadcast_delete / 9 => is_chat / 10 => comment_id / 11 => comment
data ? byte 
uint8_t room_master_id_len;
char room_master_id[IDSIZE]; 
uint8_t id_len;
[for id_len]
uint8_t user_per_id_len;
char id[MAXUSER][IDSIZE] 
[end]
uint8_t room_name_len; 
char room_name[ROOMNAMESIZE]; 
uint8_t is_enter_not_success;
uint8_t is_game_start;
uint8_t is_broadcast;
uint8_t is_update;
uint8_t is_broadcast_delete;
uint8_t is_chat;
uint8_t comment_id_len;
char comment_id[IDSIZE];
uint8_t comment_len;
char comment[COMMENTSIZE];
*/

enum LobbyRoomFlagBitInfo : uint32_t
{
    LOBBY_ROOM_ROOM_MASTER_ID_BIT = (1 << 0),
    LOBBY_ROOM_ID_BIT = (1 << 1),
    LOBBY_ROOM_ROOM_NAME_BIT = (1 << 2),
    LOBBY_ROOM_ID_LEN_BIT = (1 << 3),
    LOBBY_ROOM_IS_ENTER_NOT_SUCCESS_BIT = (1 << 4),
    LOBBY_ROOM_IS_GAME_START_BIT = (1 << 5),
    LOBBY_ROOM_IS_BROADCAST_BIT = (1 << 6),
    LOBBY_ROOM_IS_UPDATE_BIT = (1 << 7),
    LOBBY_ROOM_IS_BROADCAST_DELETE_BIT = (1 << 8),
    LOBBY_ROOM_IS_CHAT_BIT = (1 << 9),
    LOBBY_ROOM_COMMENT_ID_BIT = (1 << 10),
    LOBBY_ROOM_COMMENT_BIT = (1 << 11)
};

typedef struct _room_data
{
    char room_master_id[IDSIZE];
    int32_t id_len;
    char id[MAXUSER][IDSIZE];
    char room_name[ROOMNAMESIZE];
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
