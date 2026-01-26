#ifndef __MULTI_ENGINE_HPP__
#define __MULTI_ENGINE_HPP__

#include "engine/engine.hpp"
#include "board/board.hpp"
#include "game_rule/key_mapper.hpp"
#include "game_rule/rule_factory.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "util/action.hpp"
#include "util/timer.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>

class MultiEngine : public Engine {
private:
    Board board;
    unique_ptr<GameRule> rule;
    TetrominoQueue& tetromino_queue;
    Timer& timer;
    KeyMapper key_mapper;
    int attack;
    char another_user_ip[1024];

public:
    /**
     * @brief 게임 수행 전 필드 초기화(생성자)
     * @param 플랫폼 종속 인터페이스
     * @return 게임엔진
     */
    MultiEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer, INetwork* network);

    /**
     * @brief 게임 초기화
     */
    void init() override;

    /**
     * @brief 게임 로직 호출
     * @return 게임 상태 (GAME_OVER, GAME_CLAER 등)
     */
    int step() override;

    /**
     * @brief 게임 종료 시 메모리 및 기타 자원정리
     * @return 게임 비정상 종료 시 에러코드 반환
     */
    int finish() override;

    /**
     * @brief 게임 엔진 및 멤버 변수 정리(소멸자)
     */
    ~MultiEngine();
};

#endif
