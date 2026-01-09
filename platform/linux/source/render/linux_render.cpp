#include <iostream>
#include "tetromino/tetromino.hpp"
#include "board/board.hpp"
#include "render/linux_render.hpp"

using namespace std;

static const char* ANSI_RESET = "\x1b[0m";

static unordered_map<int, string> color_map = {
    {MinoType::I,           "\x1b[38;2;139;233;253m"},
    {MinoType::O,           "\x1b[38;2;80;250;123m"},
    {MinoType::Z,           "\x1b[38;2;255;184;108m"},
    {MinoType::S,           "\x1b[38;2;255;121;198m"},
    {MinoType::J,           "\x1b[38;2;189;147;249m"},
    {MinoType::L,           "\x1b[38;2;255;85;85m"},
    {MinoType::T,           "\x1b[38;2;241;250;140m"},
    {MinoType::OBSTACLE,    "\x1b[38;2;98;114;164m"},
};

/**
 * @brief 게임 로직과 무관한 배경 렌더링
 */
void LinuxRender::renderBackground() {}
    
/**
 * @brief 게임판 렌더링
 * @param 보드 현상태 + 현재 테트로미노 렌더링
 */
void LinuxRender::renderBoard(const Board& board, const Tetromino& tetromino)
{
    cout << "\x1b[s"; // save current cursor position
    // board
    const board_t& game_board = board.get_board();
    string color;

    for (int r = BOARD_UPPER; r < BOARD_ROW; ++r) 
    {
        for (int c = 0; c < BOARD_COL; ++c)
        {
            color = color_map[board.at(r, c)];
            cout << (board.is_filled(r, c) ? color + "[]" : "  ");
        }
        cout << "\n";
    }

    cout << "\x1b[u" << flush; // return to origin

    // mino
    if (board.has_active_mino())
    {
        const auto [pos_r, pos_c] = tetromino.get_pos();
        const mino& m = tetromino.get_shape();
        int mino_type = tetromino.get_mino_type();
        const string color = color_map[mino_type];
        int mino_r, mino_c;

        for (int r = BOARD_UPPER; r < BOARD_ROW; ++r) 
        {
            mino_r = r - pos_r;
            for (int c = 0; c < BOARD_COL; ++c)
            {
                mino_c = c - pos_c;
                if (mino_r >= 0 && mino_r < MINO_SIZE && mino_c >= 0 && mino_c < MINO_SIZE) cout << (m[mino_r][mino_c] ? color + "[]" : "\x1b[2C") << ANSI_RESET;
                else cout << "\x1b[2C";
            }
            cout << "\n";
        }       

        cout << "\x1b[u" << flush; // return to origin
    } 
}

/**
 * @brief 타이머에 현재 시간 렌더링
 * @param 현재시간(초)
 */
void LinuxRender::renderTimer(const int sec)
{
    cout << "\x1b[s"; // save current cursor position
    cout << "\x1b[20B";
    cout << "seconds: " << sec << "\r";
    cout << "\x1b[u" << flush; // return to origin
}

/**
 * @brief 다음 블럭 3개 렌더링
 * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
 */
void LinuxRender::renderNextBlock(const int* tetrominoArray)
{
    string color;
    
    cout << "\x1b[s"; // save current cursor position
    cout << "\x1b[21C";
    cout << "NEXT\n";

    for (int tetromino_num = 0; tetromino_num < 3; ++tetromino_num)
    {
        const mino& m = TETROMINO[tetrominoArray[tetromino_num]][0];
        color = color_map[tetromino_num];

        for (int r = 0; r < MINO_SIZE; ++r)
        {
            cout << "\x1b[21C";
            for (int c = 0; c < MINO_SIZE; ++c)
            {
                cout << (m[r][c] ? color + "[]" : "  ") << ANSI_RESET;
            }
            cout << "\n";
        }
    }

    cout << "\x1b[u" << flush; // return to origin
}

/**
 * @brief 홀드할 블럭 렌더링
 * @param 홀드할 블럭
 */
void LinuxRender::renderHold(const Tetromino& tetromino)
{
    int saved_mino_type = tetromino.get_mino_type();
    const mino& m = TETROMINO[saved_mino_type][0];
    string color = color_map[saved_mino_type];

    cout << "\x1b[s"; // save current cursor position
    cout << "\x1b[16B";
    cout << "\x1b[21C";
    cout << "SAVE\n";

    if (saved_mino_type != -1)
    {
        for (int r = 0; r < MINO_SIZE; ++r)
        {
            cout << "\x1b[21C";
            for (int c = 0; c < MINO_SIZE; ++c)
            {
                cout << (m[r][c] ? color + "[]" : "  ") << ANSI_RESET;
            }
            cout << "\n";
        }
    }

    cout << "\x1b[u" << flush; // return to origin
}

/**
 * @brief 점수판 렌더링
 * @param 현재 점수
 */
void LinuxRender::renderScore(const int score)
{
    cout << "\x1b[s"; // save current cursor position
    cout << "\x1b[21B";
    cout << "score: " << score << "\r";
    cout << "\x1b[u" << flush; // return to origin
}

/**
 * @brief 레벨 렌더링
 * @param 현재 레벨
 */
void LinuxRender::renderLevel(const int level)
{
    cout << "\x1b[s"; // save current cursor position
    cout << "\x1b[22B";
    cout << "level: " << level << "\r";
    cout << "\x1b[u" << flush; // return to origin
}
