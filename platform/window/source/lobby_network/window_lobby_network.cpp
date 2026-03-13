#include "lobby_network/window_lobby_network.hpp"

#include <stdio.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

WindowLobbyNetwork::WindowLobbyNetwork()
{
    WSADATA wsa_data;
    SOCKADDR_IN room_addr;
    BOOL enable = TRUE;

    // 1. 윈도우 소켓 초기화 (필수)
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        exit(0);
    }

    // 2. Room Socket 생성 (수신용)
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("socket failed: ");
        WSACleanup();
        exit(0);
    }

    // 3. Room Socket을 Non-blocking 모드로 설정
    u_long mode = 1; // 1: Non-blocking, 0: Blocking
    if (ioctlsocket(sock, FIONBIO, &mode) == SOCKET_ERROR) {
        perror("ioctlsocket failed: ");
        exit(0);
    }
    // 5. Bind data setting
    ZeroMemory(&room_addr, sizeof(room_addr));
    room_addr.sin_family = AF_INET;
    room_addr.sin_port = htons(LOBBY_PORT);
    room_addr.sin_addr.s_addr = INADDR_ANY;

    // 6. BroadCast settting
    enable = TRUE;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*) &enable, sizeof(enable));

    // 7. Bind
    if (bind(sock, (SOCKADDR*) &room_addr, sizeof(room_addr)) == SOCKET_ERROR) {
        perror("bind failed: ");
        exit(0);
    }

    // 8. Poll
    fds[0].fd = sock;
    fds[0].events = POLLIN;
}

/**
 * @brief (서버)브로드캐스트 주소를 찾는 함수
 */
void WindowLobbyNetwork::find_broadcast_ip(char* broadcast_ip)
{
    if (!broadcast_ip) return;
    broadcast_ip[0] = '\0';

    DWORD bestIf = 0;
    DWORD err = GetBestInterface(inet_addr("8.8.8.8"), &bestIf);
    if (err != NO_ERROR || bestIf == 0) return;

    ULONG size = 0;
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
    ULONG ret = GetAdaptersAddresses(AF_INET, flags, nullptr, nullptr, &size);
    if (ret != ERROR_BUFFER_OVERFLOW || size == 0) return;

    IP_ADAPTER_ADDRESSES* adapters = (IP_ADAPTER_ADDRESSES*) malloc(size);
    if (!adapters) return;

    ret = GetAdaptersAddresses(AF_INET, flags, nullptr, adapters, &size);
    if (ret != NO_ERROR) {
        free(adapters);
        return;
    }

    for (auto a = adapters; a; a = a->Next) {
        if (a->IfIndex != bestIf) continue;
        if (a->OperStatus != IfOperStatusUp) break;

        for (auto u = a->FirstUnicastAddress; u; u = u->Next) {
            if (!u->Address.lpSockaddr) continue;
            if (u->Address.lpSockaddr->sa_family != AF_INET) continue;

            auto* sa = (SOCKADDR_IN*) u->Address.lpSockaddr;

            // prefix 길이 확인
            if (u->OnLinkPrefixLength == 0 || u->OnLinkPrefixLength > 32) continue;

            uint32_t ip_host = ntohl(sa->sin_addr.s_addr);
            uint32_t mask_host = 0xFFFFFFFFu << (32 - u->OnLinkPrefixLength);
            uint32_t bc_host = ip_host | ~mask_host;

            IN_ADDR b{};
            b.s_addr = htonl(bc_host);

            inet_ntop(AF_INET, &b, broadcast_ip, 16);

            free(adapters);
            return;
        }

        break; // IfIndex 매칭 어댑터는 하나면 충분
    }

    free(adapters);
}

void WindowLobbyNetwork::write_32b(uint8_t*& p, int32_t v)
{
    int32_t n = htonl(v);
    memcpy(p, &n, 4);
    p += 4;
}

void WindowLobbyNetwork::write_bytes(uint8_t*& p, const void* data, size_t size)
{
    memcpy(p, data, size);
    p += size;
}

int32_t WindowLobbyNetwork::read_32b(const uint8_t*& p)
{
    int32_t n;
    memcpy(&n, p, 4);
    p += 4;
    return ntohl(n);
}

void WindowLobbyNetwork::read_bytes(const uint8_t*& p, void* dst, size_t size)
{
    memcpy(dst, p, size);
    p += size;
}

void WindowLobbyNetwork::compress_32b(uint8_t*& p, uint32_t& flag_bit, int32_t v,
                                      uint32_t bit_check)
{
    uint8_t itc = 0;

    flag_bit |= bit_check;
    itc = static_cast<uint8_t>(v);
    write_bytes(p, &itc, 1);
}

void WindowLobbyNetwork::compress_bytes(uint8_t*& p, uint32_t& flag_bit, const void* data,
                                        size_t size, uint32_t bit_check)
{
    flag_bit |= bit_check;
    write_bytes(p, data, size);
}

void WindowLobbyNetwork::decompress_32b(const uint8_t*& p, int32_t& v)
{
    uint8_t itc;
    read_bytes(p, &itc, 1);
    v = static_cast<int32_t>(itc);
}

void WindowLobbyNetwork::decompress_bytes(const uint8_t*& p, void* data, size_t size)
{
    read_bytes(p, data, size);
}

// user_data
uint32_t WindowLobbyNetwork::serialize(uint8_t* buf, const user_data& pkt)
{
    uint8_t* p = buf;
    uint8_t* op = buf;
    uint32_t flag_bit = 0;
    uint8_t len = 0;

    write_32b(p, USER_DATA_MAGIC);
    op += 4;

    // flag bit
    p += 4;
    
    len = static_cast<uint8_t>(strlen(pkt.id));
    compress_bytes(p, flag_bit, &len, 1, LOBBY_USER_ID_BIT);
    compress_bytes(p, flag_bit, pkt.id, len, LOBBY_USER_ID_BIT);

    if (pkt.is_enter == 1) compress_32b(p, flag_bit, pkt.is_enter, LOBBY_USER_IS_ENTER_BIT);
    if (pkt.is_out == 1) compress_32b(p, flag_bit, pkt.is_out, LOBBY_USER_IS_OUT_BIT);
    if (pkt.is_chat == 1) compress_32b(p, flag_bit, pkt.is_chat, LOBBY_USER_IS_CHAT_BIT);

    len = static_cast<uint8_t>(strlen(pkt.comment));
    if (len > 0) {
        compress_bytes(p, flag_bit, &len, 1, LOBBY_USER_COMMENT_BIT);
        compress_bytes(p, flag_bit, pkt.comment, len, LOBBY_USER_COMMENT_BIT);
    }

    write_32b(op, flag_bit);

    return (p - buf);
}

bool WindowLobbyNetwork::deserialize(const uint8_t* buf, user_data& pkt)
{
    const uint8_t* p = buf;
    uint32_t magic = 0;
    uint32_t flag_bit = 0;
    uint8_t len = 0;

    magic = read_32b(p);
    if (magic != USER_DATA_MAGIC) return false;

    flag_bit = read_32b(p);

    memset((void*) &pkt, 0, USER_DATA_SIZE);

    if (flag_bit & LOBBY_USER_ID_BIT) {
        decompress_bytes(p, &len, 1);
        decompress_bytes(p, pkt.id, len);
        pkt.id[len] = '\0';
    }
    if (flag_bit & LOBBY_USER_IS_ENTER_BIT) decompress_32b(p, pkt.is_enter);
    if (flag_bit & LOBBY_USER_IS_OUT_BIT) decompress_32b(p, pkt.is_out);
    if (flag_bit & LOBBY_USER_IS_CHAT_BIT) decompress_32b(p, pkt.is_chat);
    if (flag_bit & LOBBY_USER_COMMENT_BIT) {
        decompress_bytes(p, &len, 1);
        decompress_bytes(p, pkt.comment, len);
        pkt.id[len] = '\0';
    }

    return true;
}

// room_data
uint32_t WindowLobbyNetwork::serialize(uint8_t* buf, const room_data& pkt)
{
    uint8_t* p = buf;
    uint8_t* op = buf;
    uint32_t flag_bit = 0;
    uint8_t len = 0;

    write_32b(p, ROOM_DATA_MAGIC);
    op += 4;

    // flag bit
    p += 4;

    len = static_cast<uint8_t>(strlen(pkt.room_master_id));
    compress_bytes(p, flag_bit, &len, 1, LOBBY_ROOM_ROOM_MASTER_ID_BIT);
    compress_bytes(p, flag_bit, pkt.room_master_id, len, LOBBY_ROOM_ROOM_MASTER_ID_BIT);

    if (pkt.id_len > 0) {
        compress_32b(p, flag_bit, pkt.id_len, LOBBY_ROOM_ID_LEN_BIT);
        for (int i = 0; i < pkt.id_len; ++i) {
            len = static_cast<uint8_t>(strlen(pkt.id[i]));
            compress_bytes(p, flag_bit, &len, 1, LOBBY_ROOM_ID_BIT);
            compress_bytes(p, flag_bit, pkt.id[i], len, LOBBY_ROOM_ID_BIT);
        }
    }

    len = static_cast<uint8_t>(strlen(pkt.room_name));
    compress_bytes(p, flag_bit, &len, 1, LOBBY_ROOM_ROOM_NAME_BIT);
    compress_bytes(p, flag_bit, pkt.room_name, len, LOBBY_ROOM_ROOM_NAME_BIT);

    if (pkt.is_enter_not_success == 1)
        compress_32b(p, flag_bit, pkt.is_enter_not_success, LOBBY_ROOM_IS_ENTER_NOT_SUCCESS_BIT);
    if (pkt.is_game_start == 1)
        compress_32b(p, flag_bit, pkt.is_game_start, LOBBY_ROOM_IS_GAME_START_BIT);
    if (pkt.is_broadcast == 1)
        compress_32b(p, flag_bit, pkt.is_broadcast, LOBBY_ROOM_IS_BROADCAST_BIT);
    if (pkt.is_update == 1) compress_32b(p, flag_bit, pkt.is_update, LOBBY_ROOM_IS_UPDATE_BIT);
    if (pkt.is_broadcast_delete == 1)
        compress_32b(p, flag_bit, pkt.is_broadcast_delete, LOBBY_ROOM_IS_BROADCAST_DELETE_BIT);
    if (pkt.is_chat == 1)
        compress_32b(p, flag_bit, pkt.is_chat, LOBBY_ROOM_IS_CHAT_BIT);

    if (pkt.is_chat == 1) {
        len = static_cast<uint8_t>(strlen(pkt.comment_id));
        compress_bytes(p, flag_bit, &len, 1, LOBBY_ROOM_COMMENT_ID_BIT);
        compress_bytes(p, flag_bit, pkt.comment_id, len, LOBBY_ROOM_COMMENT_ID_BIT);
        len = static_cast<uint8_t>(strlen(pkt.comment));
        compress_bytes(p, flag_bit, &len, 1, LOBBY_ROOM_COMMENT_BIT);
        compress_bytes(p, flag_bit, pkt.comment, len, LOBBY_ROOM_COMMENT_BIT);
    }

    write_32b(op, flag_bit);

    return (p - buf);
}

bool WindowLobbyNetwork::deserialize(const uint8_t* buf, room_data& pkt)
{
    const uint8_t* p = buf;
    uint32_t magic = 0;
    uint32_t flag_bit = 0;
    uint8_t len = 0;

    magic = read_32b(p);
    if (magic != ROOM_DATA_MAGIC) return false;

    flag_bit = read_32b(p);

    memset((void*) &pkt, 0, ROOM_DATA_SIZE);

    if (flag_bit & LOBBY_ROOM_ROOM_MASTER_ID_BIT) {
        decompress_bytes(p, &len, 1);
        decompress_bytes(p, pkt.room_master_id, len);
        pkt.room_master_id[len] = '\0';
    }

    if (flag_bit & LOBBY_ROOM_ID_LEN_BIT) decompress_32b(p, pkt.id_len);

    if (flag_bit & LOBBY_ROOM_ID_BIT) {
        for (int i = 0; i < pkt.id_len; ++i) {
            decompress_bytes(p, &len, 1);
            decompress_bytes(p, pkt.id[i], len);
            pkt.id[i][len] = '\0';
        }
    }

    if (flag_bit & LOBBY_ROOM_ROOM_NAME_BIT) {
        decompress_bytes(p, &len, 1);
        decompress_bytes(p, pkt.room_name, len);
        pkt.room_name[len] = '\0';
    }

    if (flag_bit & LOBBY_ROOM_IS_ENTER_NOT_SUCCESS_BIT) decompress_32b(p, pkt.is_enter_not_success);
    if (flag_bit & LOBBY_ROOM_IS_GAME_START_BIT) decompress_32b(p, pkt.is_game_start);
    if (flag_bit & LOBBY_ROOM_IS_BROADCAST_BIT) decompress_32b(p, pkt.is_broadcast);
    if (flag_bit & LOBBY_ROOM_IS_UPDATE_BIT) decompress_32b(p, pkt.is_update);
    if (flag_bit & LOBBY_ROOM_IS_BROADCAST_DELETE_BIT) decompress_32b(p, pkt.is_broadcast_delete);
    if (flag_bit & LOBBY_ROOM_IS_CHAT_BIT) decompress_32b(p, pkt.is_chat);

    if (flag_bit & LOBBY_ROOM_COMMENT_ID_BIT) {
        decompress_bytes(p, &len, 1);
        decompress_bytes(p, pkt.comment_id, len);
        pkt.comment_id[len] = '\0';
    }
    
    if (flag_bit & LOBBY_ROOM_COMMENT_BIT) {
        decompress_bytes(p, &len, 1);
        decompress_bytes(p, pkt.comment, len);
        pkt.comment[len] = '\0';
    }

    return true;
}

void WindowLobbyNetwork::send_udp(const char* id, int is_enter, int is_out, int is_chat, const char* comment,
                                  const char* send_ip)
{
    SOCKADDR_IN addr;
    int addr_len;
    user_data data{};
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int send_result;
    uint32_t buffer_size = 0;

    ZeroMemory(&addr, sizeof(addr));
    addr_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LOBBY_PORT);
    inet_pton(AF_INET, send_ip, &addr.sin_addr);
    
    snprintf(data.id, sizeof(data.id), "%s", id);
    data.is_enter = is_enter;
    data.is_out = is_out;
    data.is_chat = is_chat;
    snprintf(data.comment, sizeof(data.comment), "%s", comment);
    buffer_size = serialize(buf, data);

    send_result = sendto(sock, (char*)buf, buffer_size, 0, (SOCKADDR*) &addr,
                         sizeof(addr));
    if (send_result == SOCKET_ERROR) printf("sendto failed: %d\n", send_result);
}

bool WindowLobbyNetwork::recv_udp(user_data& ud, char* ip)
{
    SOCKADDR_IN addr;
    int addr_len;
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int recv_result;
    bool data_received = false;
    bool is_deserialize_success = false;

    ZeroMemory(&addr, sizeof(addr));
    addr_len = sizeof(addr);
    memset(buf, 0, sizeof(buf));

     // [window Poll 처리]
    int ret = WSAPoll(fds, 1, 0);

    if (ret > 0) {
        if (fds[0].revents & POLLIN) {
            recv_result = recvfrom(sock, (char*) buf, sizeof(buf), 0, (SOCKADDR*) &addr,
                             &addr_len);

            if (recv_result == SOCKET_ERROR) {
                int err = WSAGetLastError();
                printf("recvfrom failed: %d\n", err);
                return false;
            }
            else if (recv_result < LOBBY_MAGIC_SIZE)
                return false;

            // 데이터 수신 성공
            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            is_deserialize_success = deserialize(buf, ud);

            if (is_deserialize_success == false) data_received = false;

            data_received = true;
        }
    }

    return data_received;
}

void WindowLobbyNetwork::send_udp(const char* room_master_id,
                                  std::unordered_map<std::string, std::string> ids_ips,
                                  const char* room_name, int id_len,
                                  int is_enter_not_success, int is_game_start, int is_broadcast, int is_update,
                                  int is_broadcast_delete, int is_chat, const char* comment_id,
                                  const char* comment, const char* send_ip)
{
    SOCKADDR_IN addr;
    int addr_len;
    room_data data{};
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int send_result;
    int index = 0;
    uint32_t buffer_size = 0;

    ZeroMemory(&addr, sizeof(addr));
    addr_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LOBBY_PORT);
    inet_pton(AF_INET, send_ip, &addr.sin_addr);

    snprintf(data.room_master_id, sizeof(data.room_master_id), "%s", room_master_id);
    for (const auto& [id, ip] : ids_ips)
        snprintf(data.id[index++], sizeof(data.id[0]), "%s", id.c_str());
    snprintf(data.room_name, sizeof(data.room_name), "%s", room_name);
    data.id_len = id_len;
    data.is_enter_not_success = is_enter_not_success;
    data.is_game_start = is_game_start;
    data.is_broadcast = is_broadcast;
    data.is_update = is_update;
    data.is_broadcast_delete = is_broadcast_delete;
    data.is_chat = is_chat;
    snprintf(data.comment_id, sizeof(data.comment_id), "%s", comment_id);
    snprintf(data.comment, sizeof(data.comment), "%s", comment);
    buffer_size = serialize(buf, data);

    send_result = sendto(sock, (char*)buf, buffer_size, 0, (SOCKADDR*) &addr, sizeof(addr));
    if (send_result == SOCKET_ERROR) printf("sendto failed: %d\n", send_result);
}

bool WindowLobbyNetwork::recv_udp(room_data& rd, char* ip)
{
    SOCKADDR_IN addr;
    int addr_len;
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int recv_result;
    bool data_received = false;
    bool is_deserialize_success = false;

    ZeroMemory(&addr, sizeof(addr));
    addr_len = sizeof(addr);
    memset(buf, 0, sizeof(buf));

     // [window Poll 처리]
    int ret = WSAPoll(fds, 1, 0);

    if (ret > 0) {
        if (fds[0].revents & POLLIN) {
            recv_result = recvfrom(sock, (char*) buf, sizeof(buf), 0, (SOCKADDR*) &addr, &addr_len);

            if (recv_result == SOCKET_ERROR) {
                int err = WSAGetLastError();
                printf("recvfrom failed: %d\n", err);
                return false;
            }
            else if (recv_result < LOBBY_MAGIC_SIZE)
                return false;

            // 데이터 수신 성공
            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            is_deserialize_success = deserialize(buf, rd);

            if (is_deserialize_success == false) data_received = false;

            data_received = true;
        }
    }

    return data_received;
}

void WindowLobbyNetwork::send_multi_udp(
    const char* room_master_id, std::unordered_map<std::string, std::string> pkt_ids_ips,
    const char* room_name, int id_len,
    int is_enter_not_success, int is_game_start, int is_broadcast, int is_update,
    int is_broadcast_delete, int is_chat, const char* comment_id, const char* comment,
    std::unordered_map<std::string, std::string> ids_ips)
{
    for (const auto& [user_id, user_ip] : ids_ips)
        send_udp(room_master_id, pkt_ids_ips, room_name, id_len, is_enter_not_success, is_game_start, is_broadcast,
                 is_update, is_broadcast_delete, is_chat, comment_id, comment, user_ip.c_str());
}

WindowLobbyNetwork::~WindowLobbyNetwork()
{ 
    closesocket(sock); 
    WSACleanup();
}
