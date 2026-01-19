#ifndef __I_IP_RESOLVER_HPP__
#define __I_IP_RESOLVER_HPP__

class IIpResolver
{
  protected:
    char server_ip_address[1024][16];
    char client_ip_address[4][16];
    int selected_server_ip_address;
  public:
    /**
     * @brief (서버)방을 열고 다른 사용자들의 ip 주소를 저장하게 하는 함수
     */
    virtual void open_server(bool is_open_server) = 0;

    /**
     * @brief (클라이언트)방에 입장하는 함수
     */
    virtual void enter_room() = 0;

    /**
     * @brief 저장된 클라이언트 ip 주소들을 반환하는 함수
     */
    virtual char (*get_client_ip_address())[16] = 0;

    /**
     * @brief 저장된 서버 ip 주소를 반환하는 함수
     */
    virtual char (*get_server_ip_address())[16] = 0;
};

#endif