#include "lobby_network/linux_lobby_network.hpp"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdint.h>
#include <array>

LinuxLobbyNetwork::LinuxLobbyNetwork()
{
    sockaddr_in room_addr;
    int enable;
    epoll_event ev{};

    // make socket
    sock = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (sock < 0) {
        perror("socket failed: ");
        exit(0);
    }

    room_addr.sin_family = AF_INET;
    room_addr.sin_port = htons(LOBBY_PORT);
    room_addr.sin_addr.s_addr = INADDR_ANY;

    // broadcast settting
    enable = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));

    // bind
    if (bind(sock, (sockaddr*) &room_addr, sizeof(room_addr)) < 0) {
        perror("bind failed: ");
        exit(0);
    }

    // make epoll
    epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        exit(0);
    }

    ev.events = EPOLLIN;
    ev.data.fd = sock;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) < 0) {
        perror("epoll_ctl");
        exit(0);
    }
}

void LinuxLobbyNetwork::find_broadcast_ip(char* broadcast_ip)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr{};
    const char* adapter_name[] = {"eth0", "ens33"};
    std::array<char, INET_ADDRSTRLEN> buffer{};
    int index = 0, adapter_name_size = sizeof(adapter_name) / sizeof(adapter_name[0]);
    
    if (sock < 0) {
        perror("socket failed:");
        return;
    }

    for (index = 0; index < adapter_name_size; ++index) {
        std::strncpy(ifr.ifr_name, adapter_name[index], IFNAMSIZ - 1);
        if (ioctl(sock, SIOCGIFBRDADDR, &ifr) >= 0) {
            break;
        }
    }

    if (index == adapter_name_size) {
        perror("ioctl failed:");
        close(sock);
        return;
    }

    close(sock);

    auto* addr = reinterpret_cast<sockaddr_in*>(&ifr.ifr_broadaddr);

    if (!inet_ntop(AF_INET, &addr->sin_addr, broadcast_ip, 16)) {
        perror("inet_ntop failed:");
        return;
    }
}

void LinuxLobbyNetwork::write_32b(uint8_t*& p, int32_t v)
{
    int32_t n = htonl(v);
    memcpy(p, &n, 4);
    p += 4;
}

void LinuxLobbyNetwork::write_bytes(uint8_t*& p, const void* data, size_t size)
{
    memcpy(p, data, size);
    p += size;
}

int32_t LinuxLobbyNetwork::read_32b(const uint8_t*& p)
{
    int32_t n;
    memcpy(&n, p, 4);
    p += 4;
    return ntohl(n);
}

void LinuxLobbyNetwork::read_bytes(const uint8_t*& p, void* dst, size_t size)
{
    memcpy(dst, p, size);
    p += size;
}

void LinuxLobbyNetwork::compress_32b(uint8_t*& p, uint32_t& flag_bit, int32_t v,
                                      uint32_t bit_check)
{
    uint8_t itc = 0;

    flag_bit |= bit_check;
    itc = static_cast<uint8_t>(v);
    write_bytes(p, &itc, 1);
}

void LinuxLobbyNetwork::compress_bytes(uint8_t*& p, uint32_t& flag_bit, const void* data,
                                        size_t size, uint32_t bit_check)
{
    flag_bit |= bit_check;
    write_bytes(p, data, size);
}

void LinuxLobbyNetwork::decompress_32b(const uint8_t*& p, int32_t& v)
{
    uint8_t itc;
    read_bytes(p, &itc, 1);
    v = static_cast<int32_t>(itc);
}

void LinuxLobbyNetwork::decompress_bytes(const uint8_t*& p, void* data, size_t size)
{
    read_bytes(p, data, size);
}

// user_data
uint32_t LinuxLobbyNetwork::serialize(uint8_t* buf, const user_data& pkt)
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

bool LinuxLobbyNetwork::deserialize(const uint8_t* buf, user_data& pkt)
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
        pkt.comment[len] = '\0';
    }

    return true;
}

// room_data
uint32_t LinuxLobbyNetwork::serialize(uint8_t* buf, const room_data& pkt)
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
    if (pkt.is_chat == 1) compress_32b(p, flag_bit, pkt.is_chat, LOBBY_ROOM_IS_CHAT_BIT);

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

bool LinuxLobbyNetwork::deserialize(const uint8_t* buf, room_data& pkt)
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

void LinuxLobbyNetwork::send_udp(const char* id, int is_enter, int is_out, int is_chat, const char* comment,
                                 const char* send_ip)
{
    sockaddr_in addr{};
    int addr_len;
    user_data data{};
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int send_result;
    uint32_t buffer_size = 0;

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

    send_result = sendto(sock, (char*)buf, buffer_size, 0, (sockaddr*) &addr, sizeof(addr));
    if (send_result < 0) perror("sendto failed: ");
}

bool LinuxLobbyNetwork::recv_udp(user_data& ud, char* ip)
{
    socklen_t addr_len;
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int recv_result;
    bool is_deserialize_success = false;
    int n = epoll_wait(epfd, events, MAX_EVENTS, 0);

    if (n < 0) {
        if (errno == EINTR) return false;
        perror("epoll_wait");
        return false;
    }
    if (n == 0) return false;

    for (int i = 0; i < n; ++i) {
        if ((events[i].events & EPOLLIN) != 0) {
            sockaddr_in addr{};
            addr_len = sizeof(addr);
            memset(buf, 0, sizeof(buf));

            recv_result = recvfrom(sock, (char*) buf, sizeof(buf), 0, (sockaddr*) &addr, &addr_len);
            if (recv_result < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    // clear receive buffer
                    break;
                else {
                    perror("recvfrom failed: ");
                    break;
               }
            }
            else if (recv_result < LOBBY_MAGIC_SIZE)
                break;

            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            is_deserialize_success = deserialize(buf, ud);
            
            if (is_deserialize_success == false) break;

            return true;
        }
    }
    return false;
}

void LinuxLobbyNetwork::send_udp(const char* room_master_id,
                                 std::unordered_map<std::string, std::string> ids_ips,
                                 const char* room_name,
                                 int id_len, int is_enter_not_success, int is_game_start, int is_broadcast, int is_update,
                                 int is_broadcast_delete, int is_chat, const char* comment_id,
                                 const char* comment, const char* send_ip)
{
    sockaddr_in addr{};
    int addr_len;
    room_data data{};
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int send_result;
    int index = 0;
    uint32_t buffer_size = 0;

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

    send_result = sendto(sock, (char*) buf, buffer_size, 0, (sockaddr*) &addr, sizeof(addr));
    if (send_result < 0) perror("sendto failed: ");
}

bool LinuxLobbyNetwork::recv_udp(room_data& rd, char* ip)
{
    socklen_t addr_len;
    uint8_t buf[LOBBY_BUFFER_SIZE];
    int recv_result;
    bool is_deserialize_success = false;
    int n = epoll_wait(epfd, events, MAX_EVENTS, 0);

    if (n < 0) {
        if (errno == EINTR) return false;
        perror("epoll_wait");
        return false;
    }
    if (n == 0) return false;

    for (int i = 0; i < n; ++i) {
        if ((events[i].events & EPOLLIN) != 0) {
            sockaddr_in addr{};
            addr_len = sizeof(addr);
            memset(buf, 0, sizeof(buf));
            recv_result = recvfrom(sock, (char*) buf, sizeof(buf), 0, (sockaddr*) &addr, &addr_len);
            if (recv_result < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    // clear receive buffer
                    break;
                else {
                    perror("recvfrom failed: ");
                    break;
                }
            }
            else if (recv_result < LOBBY_MAGIC_SIZE)
                break;

            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            is_deserialize_success = deserialize(buf, rd);

            if (is_deserialize_success == false) break;

            return true;
        }
    }
    return false;
}

void LinuxLobbyNetwork::send_multi_udp(
    const char* room_master_id, std::unordered_map<std::string, std::string> pkt_ids_ips,
    const char* room_name,
    int id_len, int is_enter_not_success, int is_game_start, int is_broadcast,
    int is_update, int is_broadcast_delete, 
    int is_chat, const char* comment_id, const char* comment,
    std::unordered_map<std::string, std::string> ids_ips)
{
    for (const auto& [user_id, user_ip] : ids_ips)
        send_udp(room_master_id, pkt_ids_ips, room_name, id_len, is_enter_not_success, is_game_start, is_broadcast,
                 is_update, is_broadcast_delete, is_chat, comment_id, comment, user_ip.c_str());
}

LinuxLobbyNetwork::~LinuxLobbyNetwork()
{ 
    close(sock);
    close(epfd);
}
