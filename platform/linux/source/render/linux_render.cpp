#include <iostream>
#include "tetromino/tetromino.hpp"
#include "board/board.hpp"
#include "render/linux_render.hpp"

using namespace std;

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
    // board
    const board_t& game_board = board.get_board();

    for (int r = BOARD_UPPER; r < BOARD_ROW; ++r) 
    {
        for (int c = 0; c < BOARD_COL; ++c)
        {
            cout << (board.is_filled(r, c) ? "■" : "□");
        }
        cout << "\n";
    }

    cout << "\x1b[20A"; // return to left-top side

    // mino
    if (board.has_active_mino())
    {
        
        const auto [pos_r, pos_c] = tetromino.get_pos();
        const mino& m = tetromino.get_shape();

        cout << "\x1b[" << pos_r << "B";

        for (int r = 0; r < MINO_SIZE; ++r) 
        {
            cout << "\x1b[" << pos_c << "C";
            for (int c = 0; c < MINO_SIZE; ++c) 
            {
                cout << (m[r][c] ? "■" : "\x1b[C");
            }
            cout << "\n";
        }

        cout << "\x1b[" << pos_r + MINO_SIZE << "A" << flush; // return to left-top side
    } 
}

/**
 * @brief 타이머에 현재 시간 렌더링
 * @param 현재시간(초)
 */
void LinuxRender::renderTimer(const int sec)
{
    cout << "\x1b[20B";
    cout << "seconds: " << sec << "\r";
    cout << "\x1b[20A" << flush;
}

/**
 * @brief 다음 블럭 3개 렌더링
 * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
 */
void LinuxRender::renderNextBlock(const int* tetrominoArray)
{
    cout << "\x1b[11C";
    cout << "NEXT\n"; // row : 1

    for (int tetromino_num = 0; tetromino_num < 3; ++tetromino_num)
    {
        const mino& m = TETROMINO[tetrominoArray[tetromino_num]][0];
        cout << "\n\x1b[11C"; // row : 2 / 7 / 12
        for (int r = 0; r < MINO_SIZE; ++r)
        {
            for (int c = 0; c < MINO_SIZE; ++c)
            {
                cout << (m[r][c] ? "■" : "\x1b[C");
            }
            cout << "\n";
        }
        // row: 16
    }
    cout << "\x1b[16A" << flush;
}

/**
 * @brief 홀드할 블럭 렌더링
 * @param 홀드할 블럭
 */
void LinuxRender::renderHold(const Tetromino& tetromino)
{
    int saved_mino_type = tetromino.get_mino_type();
    const mino& m = TETROMINO[saved_mino_type][0];;

    if (saved_mino_type != -1)
    {
        std::cout << "\x1b[16B";
        std::cout << "\x1b[11C";
        std::cout << "SAVE\n";

        for (int r = 0; r < 4; ++r)
        {
            std::cout << "\x1b[11C";
            for (int r = 0; r < MINO_SIZE; ++r)
            {
                for (int c = 0; c < MINO_SIZE; ++c)
                {
                    cout << (m[r][c] ? "■" : "\x1b[C");
                }
                cout << "\n";
            }
        }
        std::cout << "\x1b[20A" << flush;
    }
}

/**
 * @brief 점수판 렌더링
 * @param 현재 점수
 */
void LinuxRender::renderScore(const int score)
{
    cout << "\x1b[21B";
    cout << "score: " << score << "\r";
    cout << "\x1b[21A";
}

/**
 * @brief 레벨 렌더링
 * @param 현재 레벨
 */
void LinuxRender::renderLevel(const int level)
{
    cout << "\x1b[22B";
    cout << "level: " << level << "\r";
    cout << "\x1b[22A";
}
