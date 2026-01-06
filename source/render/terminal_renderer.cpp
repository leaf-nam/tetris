#include "terminal_renderer.h"
#include <cstdio>

namespace Color {
    const char* RESET = "\x1b[0m";
    const char* RED = "\x1b[31m";
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[33m";
    const char* CYAN = "\x1b[36m";
    const char* PURPLE = "\x1b[35m";
    const char* GRAY = "\x1b[90m";
    const char* BOLD = "\x1b[1m";
}

void TRenderer::setCursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void TRenderer::clear() {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void TRenderer::drawLogo() {
    const char* logo[] = {
        "TTTTT EEEEE TTTTT RRRR  III  SSSS  SSSS  EEEE N   N",
        "  T   E       T   R   R  I  S     S      E    NN  N",
        "  T   EEE     T   RRRR   I   SSS   SSS   EEE  N N N",
        "  T   E       T   R  R   I      S     S  E    N  NN",
        "  T   EEEEE   T   R   R III SSSS  SSSS   EEEE N   N"
    };
    for (int i = 0; i < 5; i++) {
        setCursor(22, 1 + i);
        printf("%s%s%s", Color::RED, logo[i], Color::RESET);
    }
}

void TRenderer::drawBoard(const uint16_t* game_board, bool is_active, int pos_r, int pos_c, uint16_t mino_shape) {
    int startX = 35, startY = 7;
    setCursor(startX, startY);
    std::cout << Color::BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━" << "┓" << Color::RESET;

    for (int r = 2; r < 22; ++r) {
        setCursor(startX, startY + (r - 1));
        std::cout << Color::BOLD << "┃ " << Color::RESET;

        uint16_t mino_row_bits = 0;
        if (is_active && r >= pos_r && r < pos_r + 4) {
            int shift = (3 - (r - pos_r)) * 4;
            mino_row_bits = (mino_shape >> shift) & 0xF;
            mino_row_bits <<= (12 - 3 - pos_c + 3);
        }

        for (uint16_t mask = LEFT_EDGE; mask >= RIGHT_EDGE; mask >>= 1) {
            if (game_board[r] & mask) std::cout << Color::YELLOW << "■ " << Color::RESET;
            else if (mino_row_bits & mask) std::cout << Color::CYAN << "■ " << Color::RESET;
            else std::cout << Color::GRAY << ". " << Color::RESET;
        }
        std::cout << Color::BOLD << "┃" << Color::RESET;
    }
    setCursor(startX, startY + 21);
    std::cout << Color::BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━" << "┛" << Color::RESET;
}

void TRenderer::drawUIBox(std::string title, int x, int y, int w, int h, const char* color) {
    setCursor(x, y);
    std::cout << color << "┌" << std::string(w * 2, '─') << "┐" << Color::RESET;
    for (int i = 1; i <= h; i++) {
        setCursor(x, y + i);
        std::cout << color << "│" << std::string(w * 2, ' ') << "│" << Color::RESET;
    }
    setCursor(x, y + h + 1);
    std::cout << color << "└" << std::string(w * 2, '─') << "┘" << Color::RESET;
    if (!title.empty()) {
        setCursor(x + (w * 2 - title.length()) / 2 , y);
        std::cout << color << Color::BOLD << "[" << title << "]" << Color::RESET;
    }
}

void TRenderer::renderMinoPattern(int x, int y, uint16_t shape, const char* color) {
    for (int i = 0; i < 4; i++) {
        setCursor(x, y + i);
        uint16_t row = (shape >> ((3 - i) * 4)) & 0xF;
        for (int j = 0; j < 4; j++) {
            if (row & (0x8 >> j)) std::cout << color << "■ " << Color::RESET;
            else std::cout << Color::GRAY << ". " << Color::RESET;
        }
    }
}

void TRenderer::drawHold(uint16_t hold_shape) {
    drawUIBox("HOLD", 4, 2, 6, 4, Color::GREEN);
    renderMinoPattern(7, 3, hold_shape, Color::YELLOW);
}

void TRenderer::drawNext(const std::vector<uint16_t>& next_shapes) {
    drawUIBox("NEXT", 80, 7, 6, 17, Color::PURPLE);
    for (size_t i = 0; i < next_shapes.size() && i < 3; ++i) {
        renderMinoPattern(83, 9 + (i * 5), next_shapes[i], Color::CYAN);
    }
}

void TRenderer::drawStats(int score, int lv, std::string time) {
    drawUIBox("SCORE", 4, 9, 6, 3, Color::CYAN);
    setCursor(7, 11); printf("%s%06d%s", Color::BOLD, score, Color::RESET);
    drawUIBox("LEVEL", 4, 14, 6, 3, Color::CYAN);
    setCursor(9, 16); printf("%s%02d%s", Color::BOLD, lv, Color::RESET);
    drawUIBox("TIME", 80, 2, 6, 2, Color::RESET);
    setCursor(84, 4); std::cout << Color::BOLD << time << Color::RESET;
}