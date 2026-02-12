#ifndef __WINDOW_LOBBY_NETWORK_HPP__
#define __WINDOW_LOBBY_NETWORK_HPP__

#include "i_lobby_network.hpp"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

class WindowLobbyNetwork : public ILobbyNetwork
{
  private:
    SOCKET sock;

    void write_32b(uint8_t*& p, int32_t v);
    void write_bytes(uint8_t*& p, const void* data, size_t size);
    int32_t read_32b(const uint8_t*& p);
    void read_bytes(const uint8_t*& p, void* dst, size_t size);
    
    // user_data
    void serialize(uint8_t* buf, const user_data& pkt);
    void deserialize(const uint8_t* buf, user_data& pkt);
    // room_data
    void serialize(uint8_t* buf, const room_data& pkt);
    void deserialize(const uint8_t* buf, room_data& pkt);
  public:
    WindowLobbyNetwork();
    void find_broadcast_ip(char* broadcast_ip) override;
    // user_data
    void send_udp(const char* id, int is_enter, const char* send_ip) override;
    bool recv_udp(user_data& ud, char* ip) override;
    // room_data
    void send_udp(const char* room_master_id, std::unordered_map<std::string, std::string> ids_ips,
                  int id_len,
                  int is_enter_not_success, int is_game_start, int is_broadcast,
                  int is_update, int is_broadcast_delete, const char* send_ip) override;
    bool recv_udp(room_data& rd, char* ip) override;
    void send_multi_udp(const char* room_master_id,
                        std::unordered_map<std::string, std::string>  pkt_ids_ips, int id_len,
                        int is_enter_not_success, int is_game_start, int is_broadcast,
                        int is_update, int is_broadcast_delete,
                        std::unordered_map<std::string, std::string> ids_ips) override;
    ~WindowLobbyNetwork();
};

#endif