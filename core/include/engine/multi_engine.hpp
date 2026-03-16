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

class MultiEngine : public Engine {
    private:
    Board board;
    std::unique_ptr<GameRule> rule;
    KeyMapper key_mapper;
    Timer timer;
    std::vector<std::pair<std::string, std::string>> ids_ips;
    std::unordered_map<std::string, std::string> active_user;
    std::unordered_map<std::string, int> active_user_time_checker; // 7 => timeout
    PacketStruct recv_pkt;

    int curr_mino;
    int action;
    int attack;
    bool is_line_fill_complete, is_tetromino_or_board_change;
    int key;
    int index;
    char c;
    int is_game_over, is_win;

    public:
    /**
     * @brief 게임 수행 전 필드 초기화(생성자)
     * @param 플랫폼 종속 인터페이스
     * @return 게임엔진
     */
    MultiEngine(Setting* setting, IInputHandler* input_handler, IRenderer* renderer, INetwork* network, Lobby* lobby);

    void init(bool is_server) override;

    /**
     * @brief 게임 메인루프
     */
    bool run(bool is_server) override;

    /**
     * @brief 게임 메인루프 정지
     */
    bool stop(bool is_server) override;

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
