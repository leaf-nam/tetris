#ifndef __TERMINAL_NETWORK_HPP__
#define __TERMINAL_NETWORK_HPP__

#include <i_network.hpp>

// [terminal] 윈도우 소켓 헤더 포함
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h>

// [terminal] 링커 설정 (Visual Studio가 ws2_32.lib를 링크하도록 지시)
#pragma comment(lib, "ws2_32.lib")

#define PORT 41234
// MAX_EVENTS는 epoll용이므로 삭제했습니다.

class TerminalNetwork : public INetwork
{
private:
    // [terminal] 리눅스의 int 대신 SOCKET 타입을 사용합니다.
    SOCKET client_sock;
    SOCKET server_sock;

    // [Terminal] epoll 관련 변수(epfd, events)는 삭제했습니다.
    // 대신 수신 주소 정보를 저장할 구조체를 미리 선언해두면 편합니다.
    SOCKADDR_IN server_addr;
    SOCKADDR_IN client_addr;

    void write_32b(uint8_t*& p, int32_t v);
    void serialize(uint8_t* buf, const Packet& pkt);
    int32_t read_32b(const uint8_t*& p);
    void deserialize(const uint8_t* buf, Packet& pkt);

public:
    TerminalNetwork();
    
    // 부모 클래스(INetwork)의 가상 함수 오버라이드
    void send_udp(const Board& board, const Tetromino& tetromino, int deleted_line, const char* another_user_ip) override;
    bool recv_udp(Packet& recv_pkt) override;
    
    ~TerminalNetwork();
};

#endif