#include "terminal_renderer.hpp"


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


void TerminalRenderer::setCursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    
}

void TerminalRenderer::clear() {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("\e[?25l");
}

void TerminalRenderer::drawLogo() {
    // 각 글자를 배열로 분리

    const char* T[] = {"█████", "  █  ", "  █  ", "  █  ", "  █  "};
    const char* E[] = {"█████", "█    ", "███  ", "█    ", "█████"};
    const char* R[] = {"████ ", "█   █", "████ ", "█  █ ", "█   █"};
    const char* I[] = {"███", " █ ", " █ ", " █ ", "███"};
    const char* S[] = {" ████", "█    ", " ███ ", "    █", "████ "};
    const char* N[] = {"██  █", "█ █ █", "█  ██", "█   █", "█   █"};

    int x = 20; // 시작 X 좌표
    int y = 1;  // 시작 Y 좌표

    for (int i = 0; i < 5; i++) {
        setCursor(x, y + i);
        // 순서대로 색상과 함께 출력
        printf("%s%s ", Color::RED,    T[i]); // T
        printf("%s%s ", Color::YELLOW, E[i]); // E
        printf("%s%s ", Color::GREEN,  T[i]); // T
        printf("%s%s ", Color::CYAN,   R[i]); // R
        printf("%s%s ", Color::PURPLE, I[i]); // I
        printf("%s%s ", Color::RED,    S[i]); // S
        printf("%s%s ", Color::YELLOW, S[i]); // S
        printf("%s%s ", Color::GREEN,  E[i]); // E
        printf("%s%s",  Color::CYAN,   N[i]); // N
        printf("%s", Color::RESET);
    }

    // v1 출력 (N 글자 아래쪽 끝에 배치)
    setCursor(x + 52, y + 4);
    printf("%s%s%s", Color::GRAY, "v1", Color::RESET);
}

void TerminalRenderer::renderBoard(const Board & board, const Tetromino & tetromino){
    int startX = 35, startY = 7;
    const uint16_t* game_board = board.get_board();
    auto [pos_r, pos_c] = tetromino.get_pos();
    uint16_t mino_shape = tetromino.get_shape(tetromino.get_rotation());
    setCursor(startX, startY);
    pos_c += 3;
    std::cout << Color::BOLD << "┏" << "━━━━━━━━━━━━━━━━━━━━━" << "┓" << Color::RESET;

    for (int r = 2; r < 22; ++r) {
        setCursor(startX, startY + (r - 1));
        std::cout << Color::BOLD << "┃ " << Color::RESET;
        
        uint16_t mino_row_bits = 0;
        if ( r >= pos_r && r < pos_r + 4) {
            int shift = (3 - (r - pos_r)) * 4;
            mino_row_bits = (mino_shape >> shift) & 0xF;
            mino_row_bits <<= (12 - 3 - pos_c + 3);
        }
        
        for (uint16_t mask = LEFT_EDGE; mask >= RIGHT_EDGE; mask >>= 1) {
            if (game_board[r] & mask) std::cout << Color::YELLOW << "██" << Color::RESET;
            else if (mino_row_bits & mask) std::cout << Color::CYAN << "██" << Color::RESET;
            else std::cout << Color::GRAY << ". " << Color::RESET;
        }
            
        std::cout << Color::BOLD << "┃" << Color::RESET;
    }
    setCursor(startX, startY + 21);
    std::cout << Color::BOLD << "┗" << "━━━━━━━━━━━━━━━━━━━━━" << "┛" << Color::RESET;

}

void TerminalRenderer::renderTimer(const int totalSec) {
    int min = (totalSec / 60);
    int sec = totalSec % 60;

    setCursor(84, 4);

    std::cout << Color::BOLD;
    // 두 자리(setw(2))를 잡고, 빈 곳은 '0'으로 채움
    std::cout << std::setfill('0') << std::setw(2) << min%100
        << ":"
        << std::setw(2) << sec;
    std::cout << Color::RESET;
}

void TerminalRenderer::renderNextBlock(const int* tetrominoArray) {
    for (size_t i = 0; i < 3; ++i) {
        renderMinoPattern(83, 9 + (i * 5), TETROMINO[tetrominoArray[i]][0], Color::CYAN);
    }
}

void TerminalRenderer::renderHold(const Tetromino& tetromino) {
    if (tetromino.get_mino_type() < 0 || tetromino.get_mino_type() > 6) return;
    renderMinoPattern(16, 3, tetromino.get_shape(), Color::YELLOW);
}

void TerminalRenderer::drawUIBox(std::string title, int x, int y, int w, int h, const char* color) {
    setCursor(x, y);
    std::cout << color << "┌" << std::string(w * 2, ' ') << "┐" << Color::RESET;
    for (int i = 1; i <= h; i++) {
        setCursor(x, y + i);
        std::cout << color << "│" << std::string(w * 2, ' ') << "│" << Color::RESET;
    }
    setCursor(x, y + h + 1);
    std::cout << color << "└" << std::string(w * 2, ' ') << "┘" << Color::RESET;
    if (!title.empty()) {
        setCursor(x + (w * 2 - title.length()) / 2 +1 , y);
        std::cout << color << Color::BOLD << "[" << title << "]" << Color::RESET;
    }
}

void TerminalRenderer::renderMinoPattern(int x, int y, uint16_t shape, const char* color) {
    for (int i = 0; i < 4; i++) {
        setCursor(x, y + i);
        uint16_t row = (shape >> ((3 - i) * 4)) & 0xF;
        for (int j = 0; j < 4; j++) {
            if (row & (0x8 >> j)) std::cout << color << "██" << Color::RESET;
            else std::cout << Color::GRAY << ". " << Color::RESET;
        }
    }
}



void TerminalRenderer::renderBackground() {
    fflush(stdout);
    drawLogo();
    drawUIBox("HOLD", 4, 9, 6, 4, Color::GREEN);
    drawUIBox("NEXT", 80, 7, 6, 17, Color::PURPLE);
    drawUIBox("SCORE", 4, 16, 6, 3, Color::CYAN);
    drawUIBox("LEVEL", 4, 21, 6, 3, Color::CYAN);
    drawUIBox("TIME", 80, 2, 6, 2, Color::RESET);


}
void TerminalRenderer::drawHold(uint16_t hold_shape) {
    renderMinoPattern(16, 3, hold_shape, Color::YELLOW);
}
void TerminalRenderer::renderScore(int score) {
    setCursor(7, 18); printf("%s%06d%s", Color::BOLD, score, Color::RESET);
}
void TerminalRenderer::renderLevel(int level) {
    setCursor(9, 23); printf("%s%02d%s", Color::BOLD, level, Color::RESET);
}
TerminalRenderer::~TerminalRenderer() {

}

