#ifndef __I_IP_RESOLVER_HPP__
#define __I_IP_RESOLVER_HPP__

#include <unordered_map>
#include <string>

#pragma pack(push, 1)
typedef struct _user_data
{
    char id[9];
    bool is_enter; // true == enter, false == out
}user_data;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _room_data
{
    char room_master_id[9];
    char id[4][9];
    int id_len;
    bool is_enter_not_success;
    bool is_game_start;
    bool is_broadcast;
    bool is_update;
    bool is_broadcast_delete;
}room_data;
#pragma pack(pop)

class IIpResolver
{
  protected:
    std::unordered_map<std::string, std::string> server_ip_address; // key=>ip, value=>id
    std::unordered_map<std::string, std::string> client_ip_address; // server: key=>id, value=>ip / client: key=>room_host_id, value=>room_user_id
    char selected_server_ip_address[16];
    char my_id[9];
  public:
    /**
     * @brief (서버)방을 열고 다른 사용자들의 ip 주소를 저장하게 하는 함수
     */
    virtual void open_room() = 0;

    /**
     * @brief (클라이언트)방에 입장하는 함수
     */
    virtual void enter_room() = 0;

    /**
     * @brief 저장된 클라이언트 ip 주소를 키에 따라 반환하는 함수
     */
    virtual const char* get_client_ip_address(std::string key) = 0;

    /**
     * @brief 저장된 클라이언트 id들을 반환하는 함수
     */
    virtual std::vector<std::string> get_client_ids() = 0;

    /**
     * @brief 저장된 서버 ip 주소를 반환하는 함수
     */
    virtual const char* get_server_ip_address() = 0;

    /**
     * @brief 저장된 내 id를 반환하는 함수
     */
    virtual const char* get_my_id() = 0;
};

#endif