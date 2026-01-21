#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "util/setting.hpp"

#include <i_input_handler.hpp>
#include <i_network.hpp>
#include <i_renderer.hpp>
#include <i_ip_resolver.hpp>

class Engine
{
  protected:
    Setting* setting;
    IInputHandler* input_handler;
    IRenderer* renderer;    
    INetwork* network;
    IIpResolver* ip_resolver;

  public:
    /**
     * @brief 게임 수행 전 필드 초기화(생성자)
     * @param 핸들러, 렌더러, 네트워크 인터페이스
     * @return 게임엔진
     */
    Engine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer, INetwork* network, IIpResolver* ip_resolver);
    
    /**
     * @brief 게임 메인루프
     */
    virtual void run(bool is_server) = 0;

    /**
     * @brief 게임 메인루프 정지
     */
    virtual void stop() = 0;

    /**
     * @brief 게임 종료 시 메모리 및 기타 자원정리
     * @return 게임 비정상 종료 시 에러코드 반환
     */
    virtual int finish() = 0;

    /**
     * @brief 게임 엔진 및 멤버 변수 정리(소멸자)
     */
    ~Engine() = default;
};

#endif
