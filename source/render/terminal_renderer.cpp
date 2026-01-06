#include "render/terminal_renderer.h"
using namespace std;

const int BOARD_Y = 22;
static const uint16_t left_edge = 1u << 12;
static const uint16_t right_edge = 1u << 3;
static const uint16_t full_line = ((left_edge << 1) - 1) ^ (right_edge - 1);

~IRenderer() {}
bool initialize()
{
    clear();
}
void TRenderer::clear()
{
    std::cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear terminal
}
void TRenderer::draw(const Board &board, const std::vector<Tetromino> &queue)
{
    // get board state
    const uint16_t *game_board = board.get_board();
    // draw board
    static bool called = false;

    if (called)
        cout << "\x1b[20A";
    else
        called = true;

    for (int r = 2; r < 22; ++r)
    {
        for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
        {
            cout << ((game_board[r] & mask) ? "■" : "□");
        }
        cout << endl;
    }
    cout << flush;
}

void TRenderer::display()
{
    cout << flush;
}
