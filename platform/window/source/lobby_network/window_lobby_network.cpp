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

// user_data
void WindowLobbyNetwork::serialize(uint8_t* buf, const user_data& pkt)
{
    uint8_t* p = buf;

    write_32b(p, pkt.magic);
    write_bytes(p, pkt.id, 9);
    write_32b(p, pkt.is_enter);
    write_32b(p, pkt.is_out);
    write_32b(p, pkt.is_chat);
    write_bytes(p, pkt.comment, 101);
}

void WindowLobbyNetwork::deserialize(const uint8_t* buf, user_data& pkt)
{
    const uint8_t* p = buf;

    pkt.magic = read_32b(p);
    read_bytes(p, pkt.id, 9);
    pkt.id[8] = '\0';
    pkt.is_enter = read_32b(p);
    pkt.is_out = read_32b(p);
    pkt.is_chat = read_32b(p);
    read_bytes(p, pkt.comment, 101);
}

// room_data
void WindowLobbyNetwork::serialize(uint8_t* buf, const room_data& pkt)
{
    uint8_t* p = buf;
    
    write_32b(p, pkt.magic);
    write_bytes(p, pkt.room_master_id, 9);
    for (int i = 0; i < 4; ++i)
    {
        write_bytes(p, pkt.id[i], 9);
    }
    write_bytes(p, pkt.room_name, 9);
    write_32b(p, pkt.id_len);
    write_32b(p, pkt.is_enter_not_success);
    write_32b(p, pkt.is_game_start);
    write_32b(p, pkt.is_broadcast);
    write_32b(p, pkt.is_update);
    write_32b(p, pkt.is_broadcast_delete);
    write_32b(p, pkt.is_chat);
    write_bytes(p, pkt.comment_id, 9);
    write_bytes(p, pkt.comment, 101);
}

void WindowLobbyNetwork::deserialize(const uint8_t* buf, room_data& pkt)
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
    pkt.id_len = read_32b(p);
    pkt.is_enter_not_success = read_32b(p);
    pkt.is_game_start = read_32b(p);
    pkt.is_broadcast = read_32b(p);
    pkt.is_update = read_32b(p);
    pkt.is_broadcast_delete = read_32b(p);
    pkt.is_chat = read_32b(p);
    read_bytes(p, pkt.comment_id, 9);
    read_bytes(p, pkt.comment, 101);
}

void WindowLobbyNetwork::send_udp(const char* id, int is_enter, int is_out, int is_chat, const char* comment,
                                  const char* send_ip)
{
    SOCKADDR_IN addr;
    int addr_len;
    user_data data{};
    uint8_t buf[USER_DATA_SIZE];
    int send_result;

    ZeroMemory(&addr, sizeof(addr));
    addr_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LOBBY_PORT);
    inet_pton(AF_INET, send_ip, &addr.sin_addr);
    
    data.magic = USER_DATA_MAGIC;
    snprintf(data.id, sizeof(data.id), "%s", id);
    data.is_enter = is_enter;
    data.is_out = is_out;
    data.is_chat = is_chat;
    snprintf(data.comment, sizeof(data.comment), "%s", comment);
    serialize(buf, data);

    send_result = sendto(sock, (char*) buf, USER_DATA_SIZE, 0, (SOCKADDR*) &addr,
                             sizeof(addr));
    if (send_result == SOCKET_ERROR) printf("sendto failed: %d\n", send_result);
}

bool WindowLobbyNetwork::recv_udp(user_data& ud, char* ip)
{
    SOCKADDR_IN addr;
    int addr_len;
    uint8_t buf[USER_DATA_SIZE];
    int recv_result;
    bool data_received = false;

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
            else if (recv_result != USER_DATA_SIZE)
                return false;

            // 데이터 수신 성공
            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            deserialize(buf, ud);

            if (ud.magic != USER_DATA_MAGIC) data_received = false;

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
    uint8_t buf[ROOM_DATA_SIZE];
    int send_result;
    int index = 0;

    ZeroMemory(&addr, sizeof(addr));
    addr_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LOBBY_PORT);
    inet_pton(AF_INET, send_ip, &addr.sin_addr);

    data.magic = ROOM_DATA_MAGIC;
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
    serialize(buf, data);

    send_result = sendto(sock, (char*) buf, ROOM_DATA_SIZE, 0, (SOCKADDR*) &addr, sizeof(addr));
    if (send_result == SOCKET_ERROR) printf("sendto failed: %d\n", send_result);
}

bool WindowLobbyNetwork::recv_udp(room_data& rd, char* ip)
{
    SOCKADDR_IN addr;
    int addr_len;
    uint8_t buf[ROOM_DATA_SIZE];
    int recv_result;
    bool data_received = false;

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
            else if (recv_result != ROOM_DATA_SIZE)
                return false;

            // 데이터 수신 성공
            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            deserialize(buf, rd);

            if (rd.magic != ROOM_DATA_MAGIC) data_received = false;

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
