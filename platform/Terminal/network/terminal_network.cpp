#include "terminal_network.hpp" // 헤더 파일 경로에 맞게 수정하세요
#include <iostream>

// [Terminal] 생성자
TerminalNetwork::TerminalNetwork() {
    // 1. 윈도우 소켓 초기화 (필수)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(0);
    }

    // 2. Client Socket 생성 (송신용)
    client_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock == INVALID_SOCKET) {
        std::cerr << "client socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(0);
    }

    // 3. Server Socket 생성 (수신용)
    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock == INVALID_SOCKET) {
        std::cerr << "server socket creation failed: " << WSAGetLastError() << std::endl;
        closesocket(client_sock);
        WSACleanup();
        exit(0);
    }

    // 4. Server Socket을 Non-blocking 모드로 설정
    // (리눅스의 SOCK_NONBLOCK 대체)
    u_long mode = 1; // 1: Non-blocking, 0: Blocking
    if (ioctlsocket(server_sock, FIONBIO, &mode) == SOCKET_ERROR) {
        std::cerr << "ioctlsocket failed: " << WSAGetLastError() << std::endl;
        exit(0);
    }

    // 5. Bind
    SOCKADDR_IN addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        exit(0);
    }

    // window에는 epoll이 없으므로 epoll 생성 코드는 삭제됩니다.
    // 대신 recv_udp에서 논블로킹 소켓의 특성을 이용합니다.
}

void TerminalNetwork::serialize(packet& pkt) {
    pkt.type = htonl(pkt.type);
    pkt.rotation = htonl(pkt.rotation);
    pkt.r = htonl(pkt.r);
    pkt.c = htonl(pkt.c);

    for (int i = 0; i < 20; ++i) {      // 보드 크기에 맞게 수정 (Row)
        for (int j = 0; j < 10; ++j) {  // Col
            pkt.board[i][j] = htonl(pkt.board[i][j]);
        }
    }
}

void WindowsNetwork::deserialize(packet& pkt) {
    pkt.type = ntohl(pkt.type);
    pkt.rotation = ntohl(pkt.rotation);
    pkt.r = ntohl(pkt.r);
    pkt.c = ntohl(pkt.c);

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            // [수정] 원본 코드의 오타 수정 (htonl -> ntohl)
            pkt.board[i][j] = ntohl(pkt.board[i][j]); 
        }
    }
}

void TerminalNetwork::send_udp(const Board& board, const Tetromino& tetromino, const char* another_user_ip) {
    packet pkt;
    auto [pos_r, pos_c] = tetromino.get_pos();
    
    SOCKADDR_IN another_user;
    ZeroMemory(&another_user, sizeof(another_user));
    another_user.sin_family = AF_INET;
    another_user.sin_port = htons(PORT);
    
    // Terminal에서는 inet_pton 사용 시 <WS2tcpip.h> 필요
    inet_pton(AF_INET, another_user_ip, &another_user.sin_addr);

    // 보드 데이터 복사
    for (int r = 0; r < 20; ++r)
        for (int c = 0; c < 10; ++c)
            pkt.board[r][c] = board.at(r + 2, c); // 숨겨진 2줄 제외하고 복사
            
    pkt.type = tetromino.get_mino_type();
    pkt.rotation = tetromino.get_rotation();
    pkt.r = pos_r;
    pkt.c = pos_c;

    serialize(pkt);

    int sendResult = sendto(
        client_sock,
        (const char*)&pkt, // char* 캐스팅 필요
        sizeof(pkt),
        0,
        (SOCKADDR*)&another_user,
        sizeof(another_user)
    );

    if (sendResult == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << std::endl;
    }
}

bool TerminalNetwork::recv_udp(packet& recv_pkt) {
    bool data_received = false;
    packet temp_pkt;
    SOCKADDR_IN client_addr;
    int addr_len = sizeof(client_addr);

    // [Terminal Non-blocking 처리]
    // epoll 대신 루프를 돌며 쌓인 패킷을 모두 처리하고 가장 최신 것을 가져옵니다.
    while (true) {
        int r = recvfrom(
            server_sock,
            (char*)&temp_pkt,
            sizeof(temp_pkt),
            0,
            (SOCKADDR*)&client_addr,
            &addr_len
        );

        if (r == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK) {
                // 더 이상 읽을 데이터가 없음 (버퍼 비워짐)
                break;
            }
            else {
                // 진짜 에러 발생
                std::cerr << "recvfrom failed: " << err << std::endl;
                return false;
            }
        }
        
        // 데이터 수신 성공
        recv_pkt = temp_pkt;
        deserialize(recv_pkt);
        data_received = true;
    }

    return data_received;
}

TerminalNetwork::~TerminalNetwork() {
    // 소켓 닫기
    if (client_sock != INVALID_SOCKET) closesocket(client_sock);
    if (server_sock != INVALID_SOCKET) closesocket(server_sock);

    // 윈도우 소켓 정리
    WSACleanup();
}