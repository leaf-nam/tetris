#ifndef __I_RENDERER_HPP__
#define __I_RENDERER_HPP__

#include "board/board.hpp"
#include "tetromino/tetromino.hpp"
#include "util/network_packet.hpp"

#include <string>

class IRenderer
{
  public:
    /**
     * @brief 게임 로직과 무관한 배경 렌더링
     */
    virtual void render_background() = 0;

    /**
     * @brief 게임판 렌더링
     * @param 보드 현상태 + 현재 테트로미노 렌더링
     */
    virtual void render_board(const Board& board, const Tetromino& tetromino) = 0;

    /**
     * @brief 타이머에 현재 시간 렌더링
     * @param 현재시간(초)
     */
    virtual void render_timer(int sec) = 0;

    /**
     * @brief 다음 블럭 3개 렌더링
     * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
     */
    virtual void render_next_block(const int* tetrominoArray) = 0;

    /**
     * @brief 홀드할 블럭 렌더링
     * @param 홀드할 블럭
     */
    virtual void render_hold(const Tetromino& tetromino) = 0;

    /**
     * @brief 점수판 렌더링
     * @param 현재 점수
     */
    virtual void render_score(int score) = 0;

    virtual void render_other_board(Packet& pkt) = 0;

    /**
     * @brief 레벨 렌더링
     * @param 현재 레벨
     */
    virtual void render_level(int level) = 0;


    virtual void render_clear() = 0;

    /**
     * @brief 소멸자
     */
    virtual ~IRenderer() = default;
};
#endif
