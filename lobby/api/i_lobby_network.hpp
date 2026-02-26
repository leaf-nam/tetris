#ifndef __I_LOBBY_NETWORK_HPP__
#define __I_LOBBY_NETWORK_HPP__

#include <vector>
#include <string>
#include <unordered_map>
#include <lobby_network_packet.hpp>

#define LOBBY_PORT 44111
#define BUF_SIZE 1024

class ILobbyNetwork
{
  public:
    virtual void find_broadcast_ip(char* broadcast_ip) = 0;
    // user_data
    virtual void send_udp(const char* id, int is_enter, const char* send_ip) = 0;
    virtual bool recv_udp(user_data& ud, char* ip) = 0;
    // room_data
    virtual void send_udp(const char* room_master_id,
                          std::unordered_map<std::string, std::string> ids_ips, int id_len,
                          int is_enter_not_success, int is_game_start, int is_broadcast,
                          int is_update, int is_broadcast_delete, const char* send_ip) = 0;
    virtual bool recv_udp(room_data& rd, char* ip) = 0;
    virtual void send_multi_udp(const char* room_master_id,
                                std::unordered_map<std::string, std::string> pkt_ids_ips,
                                int id_len,
                                int is_enter_not_success, int is_game_start, int is_broadcast,
                                int is_update, int is_broadcast_delete,
                                std::unordered_map<std::string, std::string> ids_ips) = 0;
    virtual ~ILobbyNetwork() = default;
};

#endif
