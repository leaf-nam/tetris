#include "lobby_network/web_lobby_network.hpp"

#include <emscripten/emscripten.h>

#include <arpa/inet.h>
#include <cstring>

namespace {
constexpr const char* kBroadcastAddress = "broadcast";

EM_JS(void, js_web_lobby_find_broadcast_ip, (char* out_ptr, int out_size), {
    if (out_size <= 0) return;
    stringToUTF8("broadcast", out_ptr, out_size);
});

EM_JS(void, js_web_lobby_send_user_data, (const char* id_ptr, int is_enter, const char* send_ip_ptr), {
    if (!Module.webLobbyNetwork) return;
    Module.webLobbyNetwork.sendUserData({
        id: UTF8ToString(id_ptr),
        isEnter: is_enter !== 0,
        sendIp: UTF8ToString(send_ip_ptr)
    });
});

EM_JS(int, js_web_lobby_recv_user_data, (uint8_t* buf_ptr, int buf_size, char* ip_ptr, int ip_size), {
    if (!Module.webLobbyNetwork || buf_size < 17 || ip_size <= 0) return 0;
    const packet = Module.webLobbyNetwork.pollUserData();
    if (!packet) return 0;

    const view = Module.HEAPU8;
    let offset = buf_ptr;
    const write32 = (value) => {
        view[offset++] = (value >>> 24) & 0xff;
        view[offset++] = (value >>> 16) & 0xff;
        view[offset++] = (value >>> 8) & 0xff;
        view[offset++] = value & 0xff;
    };
    const writeFixedString = (value, length) => {
        for (let i = 0; i < length; ++i) {
            view[offset++] = i < value.length ? value.charCodeAt(i) & 0xff : 0;
        }
    };

    write32(0x55534552);
    writeFixedString((packet.id || "").slice(0, 8), 9);
    write32(packet.isEnter ? 1 : 0);
    stringToUTF8((packet.ip || "").slice(0, 15), ip_ptr, ip_size);
    return 1;
});

EM_JS(void, js_web_lobby_send_room_data, (
    const char* room_master_id_ptr, const char* ids_ptr, const char* room_name_ptr, int id_len,
    int is_enter_not_success, int is_game_start, int is_broadcast,
    int is_update, int is_broadcast_delete, const char* send_ip_ptr
), {
    if (!Module.webLobbyNetwork) return;
    const rawIds = UTF8ToString(ids_ptr);
    Module.webLobbyNetwork.sendRoomData({
        roomMasterId: UTF8ToString(room_master_id_ptr),
        ids: rawIds ? rawIds.split("\n").filter(Boolean).slice(0, 4) : [],
        roomName: UTF8ToString(room_name_ptr),
        idLen: id_len,
        isEnterNotSuccess: is_enter_not_success !== 0,
        isGameStart: is_game_start !== 0,
        isBroadcast: is_broadcast !== 0,
        isUpdate: is_update !== 0,
        isBroadcastDelete: is_broadcast_delete !== 0,
        sendIp: UTF8ToString(send_ip_ptr)
    });
});

EM_JS(int, js_web_lobby_recv_room_data, (uint8_t* buf_ptr, int buf_size, char* ip_ptr, int ip_size), {
    if (!Module.webLobbyNetwork || buf_size < 82 || ip_size <= 0) return 0;
    const packet = Module.webLobbyNetwork.pollRoomData();
    if (!packet) return 0;

    const view = Module.HEAPU8;
    let offset = buf_ptr;
    const write32 = (value) => {
        view[offset++] = (value >>> 24) & 0xff;
        view[offset++] = (value >>> 16) & 0xff;
        view[offset++] = (value >>> 8) & 0xff;
        view[offset++] = value & 0xff;
    };
    const writeFixedString = (value, length) => {
        for (let i = 0; i < length; ++i) {
            view[offset++] = i < value.length ? value.charCodeAt(i) & 0xff : 0;
        }
    };

    write32(0x524f4f4d);
    writeFixedString((packet.roomMasterId || "").slice(0, 8), 9);
    const ids = Array.isArray(packet.ids) ? packet.ids : [];
    for (let i = 0; i < 4; ++i) {
        writeFixedString((ids[i] || "").slice(0, 8), 9);
    }
    writeFixedString((packet.roomName || "").slice(0, 8), 9);
    write32(packet.idLen | 0);
    write32(packet.isEnterNotSuccess ? 1 : 0);
    write32(packet.isGameStart ? 1 : 0);
    write32(packet.isBroadcast ? 1 : 0);
    write32(packet.isUpdate ? 1 : 0);
    write32(packet.isBroadcastDelete ? 1 : 0);
    stringToUTF8((packet.ip || "").slice(0, 15), ip_ptr, ip_size);
    return 1;
});

std::string pack_ids(const std::unordered_map<std::string, std::string>& ids_ips)
{
    std::string packed;
    int index = 0;
    for (const auto& [id, ip] : ids_ips) {
        (void)ip;
        if (index >= 4) break;
        if (!packed.empty()) packed.push_back('\n');
        packed.append(id.substr(0, 8));
        ++index;
    }
    return packed;
}
} // namespace

void WebLobbyNetwork::write_32b(uint8_t*& p, int32_t v)
{
    int32_t n = htonl(v);
    std::memcpy(p, &n, 4);
    p += 4;
}

void WebLobbyNetwork::write_bytes(uint8_t*& p, const void* data, size_t size)
{
    std::memcpy(p, data, size);
    p += size;
}

int32_t WebLobbyNetwork::read_32b(const uint8_t*& p)
{
    int32_t n;
    std::memcpy(&n, p, 4);
    p += 4;
    return ntohl(n);
}

void WebLobbyNetwork::read_bytes(const uint8_t*& p, void* dst, size_t size)
{
    std::memcpy(dst, p, size);
    p += size;
}

void WebLobbyNetwork::serialize(uint8_t* buf, const user_data& pkt)
{
    uint8_t* p = buf;
    write_32b(p, pkt.magic);
    write_bytes(p, pkt.id, 9);
    write_32b(p, pkt.is_enter);
}

void WebLobbyNetwork::deserialize(const uint8_t* buf, user_data& pkt)
{
    const uint8_t* p = buf;
    pkt.magic = read_32b(p);
    read_bytes(p, pkt.id, 9);
    pkt.id[8] = '\0';
    pkt.is_enter = read_32b(p);
}

void WebLobbyNetwork::serialize(uint8_t* buf, const room_data& pkt)
{
    uint8_t* p = buf;
    write_32b(p, pkt.magic);
    write_bytes(p, pkt.room_master_id, 9);
    for (int i = 0; i < 4; ++i)
        write_bytes(p, pkt.id[i], 9);
    write_bytes(p, pkt.room_name, 9);
    write_32b(p, pkt.id_len);
    write_32b(p, pkt.is_enter_not_success);
    write_32b(p, pkt.is_game_start);
    write_32b(p, pkt.is_broadcast);
    write_32b(p, pkt.is_update);
    write_32b(p, pkt.is_broadcast_delete);
}

void WebLobbyNetwork::deserialize(const uint8_t* buf, room_data& pkt)
{
    const uint8_t* p = buf;
    pkt.magic = read_32b(p);
    read_bytes(p, pkt.room_master_id, 9);
    pkt.room_master_id[8] = '\0';
    for (int i = 0; i < 4; ++i) {
        read_bytes(p, pkt.id[i], 9);
        pkt.id[i][8] = '\0';
    }
    read_bytes(p, pkt.room_name, 9);
    pkt.room_name[8] = '\0';
    pkt.id_len = read_32b(p);
    pkt.is_enter_not_success = read_32b(p);
    pkt.is_game_start = read_32b(p);
    pkt.is_broadcast = read_32b(p);
    pkt.is_update = read_32b(p);
    pkt.is_broadcast_delete = read_32b(p);
}

void WebLobbyNetwork::find_broadcast_ip(char* broadcast_ip)
{
    js_web_lobby_find_broadcast_ip(broadcast_ip, 16);
}

void WebLobbyNetwork::send_udp(const char* id, int is_enter, const char* send_ip)
{
    js_web_lobby_send_user_data(id, is_enter, send_ip);
}

bool WebLobbyNetwork::recv_udp(user_data& ud, char* ip)
{
    uint8_t buf[USER_DATA_SIZE];
    if (js_web_lobby_recv_user_data(buf, USER_DATA_SIZE, ip, 16) == 0) return false;
    deserialize(buf, ud);
    return ud.magic == USER_DATA_MAGIC;
}

void WebLobbyNetwork::send_udp(const char* room_master_id,
                               std::unordered_map<std::string, std::string> ids_ips,
                               const char* room_name, int id_len,
                               int is_enter_not_success, int is_game_start, int is_broadcast,
                               int is_update, int is_broadcast_delete, const char* send_ip)
{
    const std::string ids = pack_ids(ids_ips);
    js_web_lobby_send_room_data(room_master_id, ids.c_str(), room_name, id_len,
                                is_enter_not_success, is_game_start, is_broadcast,
                                is_update, is_broadcast_delete, send_ip);
}

bool WebLobbyNetwork::recv_udp(room_data& rd, char* ip)
{
    uint8_t buf[ROOM_DATA_SIZE];
    if (js_web_lobby_recv_room_data(buf, ROOM_DATA_SIZE, ip, 16) == 0) return false;
    deserialize(buf, rd);
    return rd.magic == ROOM_DATA_MAGIC;
}

void WebLobbyNetwork::send_multi_udp(const char* room_master_id,
                                     std::unordered_map<std::string, std::string> pkt_ids_ips,
                                     const char* room_name, int id_len,
                                     int is_enter_not_success, int is_game_start, int is_broadcast,
                                     int is_update, int is_broadcast_delete,
                                     std::unordered_map<std::string, std::string> ids_ips)
{
    if (ids_ips.empty()) {
        send_udp(room_master_id, pkt_ids_ips, room_name, id_len, is_enter_not_success,
                 is_game_start, is_broadcast, is_update, is_broadcast_delete,
                 kBroadcastAddress);
        return;
    }

    for (const auto& [id, ip] : ids_ips) {
        (void)id;
        send_udp(room_master_id, pkt_ids_ips, room_name, id_len, is_enter_not_success,
                 is_game_start, is_broadcast, is_update, is_broadcast_delete, ip.c_str());
    }
}

