#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "util/setting.hpp"

#include <i_input_handler.hpp>
#include <i_network.hpp>
#include <i_renderer.hpp>

enum EngineState
{
  RUNNING,
  GAME_OVER,
  GAME_CLEAR,
  PAUSED
};

class Engine
{
  protected:
    Setting* setting;
    IInputHandler* input_handler;
    IRenderer* renderer;
    INetwork* network;

  public:
    Engine(Setting*, IInputHandler*, IRenderer*, INetwork*);
    /**
     * @brief 게임 초기화
     */
    virtual void init() = 0;

    /**
     * @brief 게임 로직 호출
     */
    virtual int step() = 0;

    /**
     * @brief 게임 종료 시 메모리 및 기타 자원정리
     * @return 게임 비정상 종료 시 에러코드 반환
     */
    virtual int finish() = 0;

    /**
     * @brief 게임 화면 업데이트
     */
    virtual void render() = 0;

    /**
     * @brief 게임 엔진 및 멤버 변수 정리(소멸자)
     */
    ~Engine() = default;
};

#endif
