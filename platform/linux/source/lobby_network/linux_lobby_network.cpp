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

// user_data
void LinuxLobbyNetwork::serialize(uint8_t* buf, const user_data& pkt)
{
    uint8_t* p = buf;

    write_32b(p, pkt.magic);
    write_bytes(p, pkt.id, 9);
    write_32b(p, pkt.is_enter);
    write_32b(p, pkt.is_out);
    write_32b(p, pkt.is_chat);
    write_bytes(p, pkt.comment, 101);
}

void LinuxLobbyNetwork::deserialize(const uint8_t* buf, user_data& pkt)
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
void LinuxLobbyNetwork::serialize(uint8_t* buf, const room_data& pkt)
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

void LinuxLobbyNetwork::deserialize(const uint8_t* buf, room_data& pkt)
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

void LinuxLobbyNetwork::send_udp(const char* id, int is_enter, int is_out, int is_chat, const char* comment,
                                 const char* send_ip)
{
    sockaddr_in addr{};
    int addr_len;
    user_data data{};
    uint8_t buf[USER_DATA_SIZE];
    int send_result;

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

    send_result = sendto(sock, (char*) buf, USER_DATA_SIZE, 0, (sockaddr*) &addr, sizeof(addr));
    if (send_result < 0) perror("sendto failed: ");
}

bool LinuxLobbyNetwork::recv_udp(user_data& ud, char* ip)
{
    socklen_t addr_len;
    uint8_t buf[USER_DATA_SIZE];
    int recv_result;
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
            else if (recv_result != USER_DATA_SIZE)
                break;

            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            deserialize(buf, ud);
            
            if(ud.magic != USER_DATA_MAGIC)
                break;

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
    uint8_t buf[ROOM_DATA_SIZE];
    int send_result;
    int index = 0;

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

    send_result = sendto(sock, (char*) buf, ROOM_DATA_SIZE, 0, (sockaddr*) &addr, sizeof(addr));
    if (send_result < 0) perror("sendto failed: ");
}

bool LinuxLobbyNetwork::recv_udp(room_data& rd, char* ip)
{
    socklen_t addr_len;
    uint8_t buf[ROOM_DATA_SIZE];
    int recv_result;
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
            else if (recv_result != ROOM_DATA_SIZE)
                break;

            inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
            deserialize(buf, rd);

            if (rd.magic != ROOM_DATA_MAGIC)
                break;

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
