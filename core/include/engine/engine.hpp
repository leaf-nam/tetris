#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <i_input_handler.hpp>
#include <i_renderer.hpp>
#include <i_network.hpp>
#include "util/setting.hpp"

class Engine {
    protected:
    Setting setting;
    IInputHandler* input_handler;
    IRenderer* renderer;    
    INetwork* network;    

    public:

    /**
     * @brief 게임 메인루프
     */
    virtual void run() = 0;

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
