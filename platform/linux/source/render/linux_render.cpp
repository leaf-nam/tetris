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
    static bool called = false;
    static const uint16_t left_edge = 1u << 12;
    static const uint16_t right_edge = 1u << 3;
    const uint16_t* game_board = board.get_board();

    if (called) std::cout << "\x1b[20A";
    else called = true;

    for (int r = 2; r < 22; ++r) 
    {
        for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
        {
            cout << ((game_board[r] & mask) ? "■" : "□");
        }
        cout << endl;
    }
    cout << flush;

    // mino
    if (board.has_active_mino())
    {
        std::cout << "\x1b[20A";
        const auto [pos_r, pos_c] = tetromino.get_pos();
        mino mino_mask = 0b1111000000000000;
        const mino mino_shape = tetromino.get_shape();
        uint16_t mino_row;

        for (int r = 2; r < 22; ++r) 
        {
            if (r >= pos_r && r < pos_r + 4)
            {
                mino_row = (mino_mask >> (r - pos_r) * 4) & mino_shape;
                mino_row >>= (3 - (r - pos_r)) * 4;
                mino_row <<= (9 - pos_c);

                for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
                {
                    cout << ((mino_row & mask) ? "■" : "\x1b[C");
                }
            }
            cout << endl;
        }
        cout << flush;
    } 
}

/**
 * @brief 타이머에 현재 시간 렌더링
 * @param 현재시간(초)
 */
void LinuxRender::renderTimer(const int sec)
{
    cout << "seconds: " << sec << "\r";
}

/**
 * @brief 다음 블럭 3개 렌더링
 * @param 다음 블럭 3개를 가진 배열 포인터(순서 중요)
 */
void LinuxRender::renderNextBlock(const int* tetrominoArray)
{
    static bool called = false;
    mino mino_mask = 0b1111000000000000;
    mino mino_shape;
    static const uint16_t left_edge = 1u << 12;
    static const uint16_t right_edge = 1u << 3;
    uint16_t mino_row;

    std::cout << "\x1b[15A";
    std::cout << "\x1b[11C";
    std::cout << "NEXT\n";
    for (int tetromino_num = 0; tetromino_num < 3; ++tetromino_num)
    {
        mino_shape = TETROMINO[tetrominoArray[tetromino_num]][0];
        for (int r = 0; r < 4; ++r)
        {
            std::cout << "\x1b[11C";
            for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
            {
                cout << " ";
            }

            mino_row = (mino_mask >> r * 4) & mino_shape;
            mino_row >>= (3 - r) * 4;
            mino_row <<= (9);

            std::cout << "\x1b[10D";
            for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
            {
                cout << ((mino_row & mask) ? "■" : "\x1b[C");
            }
            cout << endl;
        }
    }
    std::cout << "\x1b[16B";
    std::cout << "\x1b[11D";
    cout << flush;
}

/**
 * @brief 홀드할 블럭 렌더링
 * @param 홀드할 블럭
 */
void LinuxRender::renderHold(const Tetromino& tetromino)
{
    static bool called = false;
    mino mino_mask = 0b1111000000000000;
    mino mino_shape;
    static const uint16_t left_edge = 1u << 12;
    static const uint16_t right_edge = 1u << 3;
    uint16_t mino_row;
    int saved_mino_type = tetromino.get_mino_type();

    std::cout << "\x1b[20A";
    std::cout << "\x1b[11C";
    std::cout << "SAVE\n";

    if (saved_mino_type == -1)
    {
        std::cout << "\x1b[20B";
        std::cout << "\x1b[11D";
        cout << flush;
        return;
    }

    mino_shape = TETROMINO[saved_mino_type][0];
    for (int r = 0; r < 4; ++r)
    {
        std::cout << "\x1b[11C";
        for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
        {
            cout << " ";
        }

        mino_row = (mino_mask >> r * 4) & mino_shape;
        mino_row >>= (3 - r) * 4;
        mino_row <<= (9);

        std::cout << "\x1b[10D";
        for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
        {
            cout << ((mino_row & mask) ? "■" : "\x1b[C");
        }
        cout << endl;
    }
    std::cout << "\x1b[20B";
    std::cout << "\x1b[11D";
    cout << flush;
}

/**
 * @brief 점수판 렌더링
 * @param 현재 점수
 */
void LinuxRender::renderScore(const int score)
{
    cout << "score: " << score << " ";
}

/**
 * @brief 레벨 렌더링
 * @param 현재 레벨
 */
void LinuxRender::renderLevel(const int level)
{
    cout << "level: " << level << " ";
}
