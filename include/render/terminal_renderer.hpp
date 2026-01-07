#ifndef TERMINAL_RENDERER_H
#define TERMINAL_RENDERER_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include "board/board.hpp"
#include "tetromino/tetromino_queue.hpp"

// --- 상수 정의 ---
static const uint16_t LEFT_EDGE = 1u << 12;
static const uint16_t RIGHT_EDGE = 1u << 3;

// --- 색상 네임스페이스 ---
namespace Color {
    extern const char* RESET;
    extern const char* RED;
    extern const char* GREEN;
    extern const char* YELLOW;
    extern const char* CYAN;
    extern const char* PURPLE;
    extern const char* GRAY;
    extern const char* BOLD;
}

class TRenderer {
public:
    void clear();
    void draw(const Board* board, const TetrominoQueue& queue);
    void display();

private:
    void setCursor(int x, int y);
    void drawUIBox(std::string title, int x, int y, int w, int h, const char* color);
    void renderMinoPattern(int x, int y, uint16_t shape, const char* color);
    //display
    void drawLogo();
    void drawUIBox(std::string title, int x, int y, int w, int h, const char* color);

    //draw
    void renderMinoPattern(int x, int y, uint16_t shape, const char* color);
    void drawHold(uint16_t hold_shape);
    void drawNext(const std::vector<uint16_t>& next_shapes);
};

#endif