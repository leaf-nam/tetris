#include "board/board.hpp"
#include "input/action.hpp"
#include "input/console_input.hpp"
#include "game_rule/game_rule.hpp"
#include "tetromino/tetromino_queue.hpp"
#include "render/terminal_renderer.hpp"
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

int main(void)
{
   TRenderer renderer;
    renderer.clear();

    // 더미 데이터 세팅
    uint16_t board[22] = {0};
    board[20] = 0b0000011111111000;
    board[21] = 0b0000011111111000;

    std::vector<uint16_t> nexts = {0x4E00, 0x6600, 0x0F00};

    // 렌더링 호출
    renderer.drawLogo();
    renderer.drawHold(0x4444);
    renderer.drawBoard(board, true, 10, 4, 0x4E00); // T-Mino
    renderer.drawNext(nexts);
    renderer.drawStats(12500, 3, "01:23");

    // 종료 안내 (y 좌표 30으로 이동)
    printf("\033[30;1H\nRenderer Test Done!\n");
    return 0;
}
