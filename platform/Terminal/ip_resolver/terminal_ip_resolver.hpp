#ifndef __TERMINAL_IP_RESOLVER_HPP__
#define __TERMINAL_IP_RESOLVER_HPP__

#define ROOM_PORT 44321

#include "i_ip_resolver.hpp"

#pragma comment(lib, "ws2_32.lib")

class TerminalIpResolver : public IIpResolver
{
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
     * @brief 저장된 클라이언트 ip 주소들을 반환하는 함수
     */
    char (*get_client_ip_address())[16] override;

    /**
     * @brief 저장된 서버 ip 주소를 반환하는 함수
     */
    char (*get_server_ip_address())[16] override;
};

#endif