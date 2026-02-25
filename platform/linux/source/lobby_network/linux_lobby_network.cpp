#include "lobby_network/linux_lobby_network.hpp"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdint.h>

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
    std::array<char, INET_ADDRSTRLEN> buffer{};
    
    if (sock < 0) {
        perror("socket failed:");
        return;
    }

    std::strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFBRDADDR, &ifr) < 0) {
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

    write_bytes(p, pkt.id, 9);
    write_32b(p, pkt.is_enter);
}

void LinuxLobbyNetwork::deserialize(const uint8_t* buf, user_data& pkt)
{
    const uint8_t* p = buf;

    read_bytes(p, pkt.id, 9);
    pkt.id[8] = '\0';
    pkt.is_enter = read_32b(p);
}

// room_data
void LinuxLobbyNetwork::serialize(uint8_t* buf, const room_data& pkt)
{
    uint8_t* p = buf;

    write_bytes(p, pkt.room_master_id, 9);
    for (int i = 0; i < 4; ++i)
    {
        write_bytes(p, pkt.id[i], 9);
    }
    write_32b(p, pkt.id_len);
    write_32b(p, pkt.is_enter_not_success);
    write_32b(p, pkt.is_game_start);
    write_32b(p, pkt.is_broadcast);
    write_32b(p, pkt.is_update);
    write_32b(p, pkt.is_broadcast_delete);
}

void LinuxLobbyNetwork::deserialize(const uint8_t* buf, room_data& pkt)
{
    const uint8_t* p = buf;

    read_bytes(p, pkt.room_master_id, 9);
    pkt.room_master_id[8] = '\0';
    for (int i = 0; i < 4; ++i) {
        read_bytes(p, pkt.id[i], 9);
        pkt.id[i][8] = '\0';
    }
    pkt.id_len = read_32b(p);
    pkt.is_enter_not_success = read_32b(p);
    pkt.is_game_start = read_32b(p);
    pkt.is_broadcast = read_32b(p);
    pkt.is_update = read_32b(p);
    pkt.is_broadcast_delete = read_32b(p);
}

void LinuxLobbyNetwork::send_udp(const char* id, int is_enter, const char* send_ip)
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
    
    snprintf(data.id, sizeof(data.id), "%s", id);
    data.is_enter = is_enter;
    serialize(buf, data);

    send_result = sendto(sock, (char*) buf, USER_DATA_SIZE, 0, (sockaddr*) &addr, sizeof(addr));
    if (send_result < 0) perror("sendto failed: ");
}

bool LinuxLobbyNetwork::recv_udp(user_data& ud, char* ip)
{
    socklen_t addr_len;
    uint8_t buf[USER_DATA_SIZE];
    bool data_received = false;
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
            while (true) {
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
                        return false;
                   }
                }
                else if (recv_result != USER_DATA_SIZE)
                    return false;

                inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
                deserialize(buf, ud);
		data_received = true;
            }
        }
    }
    return data_received;
}

void LinuxLobbyNetwork::send_udp(const char* room_master_id,
                                       std::unordered_map<std::string, std::string> ids_ips,
                                       int id_len,
    int is_enter_not_success, int is_game_start, int is_broadcast,
    int is_update, int is_broadcast_delete, const char* send_ip)
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

    snprintf(data.room_master_id, sizeof(data.room_master_id), "%s", room_master_id);
    for (const auto& [id, ip] : ids_ips)
        snprintf(data.id[index++], sizeof(data.id[0]), "%s", id.c_str());
    data.id_len = id_len;
    data.is_enter_not_success = is_enter_not_success;
    data.is_game_start = is_game_start;
    data.is_broadcast = is_broadcast;
    data.is_update = is_update;
    data.is_broadcast_delete = is_broadcast_delete;
    serialize(buf, data);

    send_result = sendto(sock, (char*) buf, ROOM_DATA_SIZE, 0, (sockaddr*) &addr, sizeof(addr));
    if (send_result < 0) perror("sendto failed: ");
}

bool LinuxLobbyNetwork::recv_udp(room_data& rd, char* ip)
{
    socklen_t addr_len;
    uint8_t buf[ROOM_DATA_SIZE];
    bool data_received = false;
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
            while (true) {
                sockaddr_in addr{};
                addr_len = sizeof(addr);
                memset(buf, 0, sizeof(buf));
                recv_result = recvfrom(sock, (char*) buf, sizeof(buf), 0, (sockaddr*) &addr, &addr_len);
                if (recv_result < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        // clear receive buffer
                        return false;
                    else {
                        perror("recvfrom failed: ");
                        return false;
                    }
                }
                else if (recv_result != ROOM_DATA_SIZE)
                    return false;

                inet_ntop(AF_INET, &addr.sin_addr, ip, 16);
                deserialize(buf, rd);
		data_received = true;
            }
        }
    }
    return true;
}

void LinuxLobbyNetwork::send_multi_udp(
    const char* room_master_id, std::unordered_map<std::string, std::string> pkt_ids_ips,
    int id_len,
    int is_enter_not_success, int is_game_start, int is_broadcast,
    int is_update, int is_broadcast_delete, std::unordered_map<std::string, std::string> ids_ips)
{
    for (const auto& [user_id, user_ip] : ids_ips)
        send_udp(room_master_id, pkt_ids_ips, id_len, is_enter_not_success, is_game_start, is_broadcast,
                 is_update, is_broadcast_delete, user_ip.c_str());
}

LinuxLobbyNetwork::~LinuxLobbyNetwork()
{ 
    close(sock); 
}
