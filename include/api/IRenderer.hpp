#ifndef __I_RENDERER_HPP__
#define __I_RENDERER_HPP__

#include <string>
#include "tetromino/tetromino.hpp"
#include "board/board.hpp"

using namespace std;

class IRenderer {
public:

    /**
     * @brief 게임 로직과 무관한 배경 렌더링
     */
    virtual void renderBackground();
    
    /**
     * @brief 게임판 렌더링
     * @param 보드 현상태 + 현재 테트로미노 렌더링
     */
    virtual void renderBoard(Board& board, Tetromino& tetromino);

    /**
     * @brief 타이머에 현재 시간 렌더링
     * @param 현재시간(초)
     */
    virtual void renderTimer(int sec);

    /**
     * @brief 다음 블럭 3개 렌더링
     * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
     */
    virtual void renderNextBlock(Tetromino* tetrominoArray);

    /**
     * @brief 홀드할 블럭 렌더링
     * @param 홀드할 블럭
     */
    virtual void renderHold(Tetromino& tetromino);

    /**
     * @brief 점수판 렌더링
     * @param 현재 점수
     */
    virtual void renderScore(int score);

    /**
     * @brief 레벨 렌더링
     * @param 현재 레벨
     */
    virtual void renderLevel(int level);

    /**
     * @brief 소멸자
     */
    virtual ~IRenderer();
};
#endif