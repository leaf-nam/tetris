#ifndef __I_IP_RESOLVER_HPP__
#define __I_IP_RESOLVER_HPP__

#include <unordered_map>
#include <vector>
#include <string>
#include "i_ip_resolver_network.hpp"
#include "i_ip_resolver_renderer.hpp"

class IIpResolver
{
  protected:
    std::unordered_map<std::string, std::string> server_ip_address; // key=>ip, value=>id
    std::unordered_map<std::string, std::string> client_ip_address; // server: key=>id, value=>ip / client: key=>room_host_id, value=>room_user_id
    char selected_server_ip_address[16];
    char my_id[9];
    IIpResolverNetwork* ip_resolver_network;
  public:
    /**
     * @brief 현재 어떤 모드로 들어갈지 확인하는 함수
     */
    virtual bool start() = 0;

    /**
     * @brief 저장된 클라이언트 ip 주소를 키에 따라 반환하는 함수
     */
    virtual const char* get_client_ip_address(std::string key) = 0;

    /**
     * @brief 저장된 클라이언트 id들을 반환하는 함수
     */
    virtual std::vector<std::pair<std::string, std::string>> get_client_ids_ips() = 0;

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