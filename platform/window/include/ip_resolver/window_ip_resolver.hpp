#ifndef __WINDOW_IP_RESOLVER_HPP__
#define __WINDOW_IP_RESOLVER_HPP__

#define ROOM_PORT 44321

#include "i_ip_resolver.hpp"

class WindowIpResolver : public IIpResolver
{
  private:
    /**
     * @brief (서버)브로드캐스트 주소를 찾는 함수
     */
    void find_broadcast_ip(char* broadcast_ip);
  public:
    /**
     * @brief (서버)방을 열고 다른 사용자들의 ip 주소를 저장하게 하는 함수
     */
    void open_room() override;

    /**
     * @brief (클라이언트)방에 입장하는 함수
     */
    void enter_room() override;

    /**
     * @brief 저장된 클라이언트 ip 주소를 키에 따라 반환하는 함수
     */
    const char* get_client_ip_address(std::string key) override;

    /**
     * @brief 저장된 클라이언트 id들을 반환하는 함수
     */
    std::vector<std::string> get_client_ids() override;

    /**
     * @brief 저장된 서버 ip 주소를 반환하는 함수
     */
    const char* get_server_ip_address() override;

    /**
     * @brief 저장된 내 id를 반환하는 함수
     */
    const char* get_my_id() override;
};

#endif
