#ifndef TERMINAL_RENDERER_H
#define TERMINAL_RENDERER_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include <cstdio>

#include "board/board.hpp"
#include "tetromino/tetromino.hpp"
#include "api/i_renderer.hpp"

// --- 상수 정의 ---
static const uint16_t LEFT_EDGE = 1u << 12;
static const uint16_t RIGHT_EDGE = 1u << 3;

// --- 색상 네임스페이스 ---
namespace Color
{
    extern const char* RESET;
    extern const char* RED;
    extern const char* GREEN;
    extern const char* YELLOW;
    extern const char* CYAN;
    extern const char* PURPLE;
    extern const char* GRAY;
    extern const char* BOLD;
}

class TerminalRenderer : public IRenderer
{
public:
     /**
     * @brief 게임 로직과 무관한 배경 렌더링
     */
    void renderBackground() override;

    /**
     * @brief 게임판 렌더링
     * @param 보드 현상태 + 현재 테트로미노 렌더링
     */
    void renderBoard(const Board& board, const Tetromino& tetromino) override;

    /**
     * @brief 타이머에 현재 시간 렌더링
     * @param 현재시간(초)
     */
    void renderTimer(const int sec) override;

    /**
     * @brief 다음 블럭 3개 렌더링
     * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
     */
    void renderNextBlock(const int* tetrominoArray) override;

    /**
     * @brief 홀드할 블럭 렌더링
     * @param 홀드할 블럭
     */
    void renderHold(const Tetromino& tetromino) override;

    /**
     * @brief 점수판 렌더링
     * @param 현재 점수
     */
    void renderScore(const int score) override;

    /**
     * @brief 레벨 렌더링
     * @param 현재 레벨
     */
    void renderLevel(const int level) override;
    
    /**
     * @brief 소멸자
     */
    virtual ~TerminalRenderer() override;

private:
    void clear();
    void setCursor(int x, int y);
    //display
    void drawLogo();
    void drawUIBox(std::string title, int x, int y, int w, int h, const char* color);

    //draw
    void renderMinoPattern(int x, int y, uint16_t shape, const char* color);
    void drawHold(uint16_t hold_shape);
};

#endif