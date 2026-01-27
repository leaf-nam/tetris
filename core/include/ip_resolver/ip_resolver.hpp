#ifndef __IP_RESOLVER_HPP__
#define __IP_RESOLVER_HPP__

#define ROOM_PORT 44321

#include <unordered_map>
#include <vector>
#include <string>
#include "i_ip_resolver_network.hpp"
#include "i_ip_resolver_renderer.hpp"
#include "i_ip_resolver_input_handler.hpp"

class IpResolver
{
  private:
    std::unordered_map<std::string, std::string> server_ip_address; // key=>ip, value=>id
    std::unordered_map<std::string, std::string> client_ip_address; // server: key=>id, value=>ip / client: key=>room_host_id, value=>room_user_id
    char selected_server_ip_address[16];
    char my_id[9];
    IIpResolverNetwork* ip_resolver_network;
    IIpResolverRenderer* ip_resolver_renderer;
    IIpResolverInputHandler* ip_resolver_input_handler;
    /**
     * @brief (서버)브로드캐스트 주소를 찾는 함수
     */
    void find_broadcast_ip(char* broadcast_ip);
    
    /**
     * @brief (서버)방을 열고 다른 사용자들의 ip 주소를 저장하게 하는 함수
     */
    bool open_room();

    /**
     * @brief (클라이언트)방에 입장하는 함수
     */
    bool enter_room();
  public:
    IpResolver(IIpResolverNetwork* ip_resolver_network,
               IIpResolverRenderer* ip_resolver_renderer,
               IIpResolverInputHandler* ip_resolver_input_handler);

    /**
     * @brief 현재 어떤 모드로 들어갈지 확인하는 함수
     */
    bool start();

    void finish();

    /**
     * @brief 저장된 클라이언트 ip 주소를 키에 따라 반환하는 함수
     */
    const char* get_client_ip_address(std::string key);

    /**
     * @brief 저장된 클라이언트 id들을 반환하는 함수
     */
    std::vector<std::pair<std::string, std::string>> get_client_ids_ips();

    /**
     * @brief 저장된 전체 id들을 반환하는 함수
     */
    std::unordered_map<std::string, std::string> get_ids(bool is_server);

    /**
     * @brief 저장된 서버 ip 주소를 반환하는 함수
     */
    const char* get_server_ip_address();

    /**
     * @brief 저장된 내 id를 반환하는 함수
     */
    const char* get_my_id();
};

#endif
