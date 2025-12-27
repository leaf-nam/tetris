#include "board/board.hpp"
#include "input/action.hpp"

using namespace std;

static uint16_t left_edge = 1u << 9;

static int dr[6] = {0, 0, 1, 0, 0, 0};
static int dc[6] = {-1, 1, 0, 0, 0, 0};

Board::Board()
{
    is_mino_active = false;

    for (size_t i = 0; i < 22; ++i)
    {
        game_board[i] = 0;
    }
}

/**
 * @brief 테트로미노 충돌 판정
 * @return 충돌 없는 경우 true 반환
 */
void Board::move_mino(int cmd)
{   
    if (!is_mino_active) return;

    bool can_go = true;
    auto [curr_r, curr_c] = active_mino.get_pos();
    int new_r, new_c;

    uint16_t mino_mask = 0b1111000000000000;
    uint16_t mino_shape = active_mino.get_shape();
    uint16_t mino_row;

    new_r = curr_r + dr[cmd], new_c = curr_c + dc[cmd];

    if (new_c < 0 || new_c >= 10) return;

    for (int i = 3; i >= 0; --i, mino_mask >>= 4, new_r++)
    {
        mino_row = mino_mask & mino_shape;
        mino_row >>= i * 4;
        mino_row <<= (6 - new_c);
        if (game_board[new_r] & mino_row) 
        {
            can_go = false;
            break;
        }
    }

    if (can_go) active_mino.set_pos(new_r, new_c);
}

/**
 * @brief 새 테트로미노를 스폰
 * @param type 테트로미노의 타입
 */
void Board::spawn_mino(int type)
{
    active_mino.init_mino(type);
    is_mino_active = true;
}

/**
 * @brief 보드 상태를 업데이트
 */
void Board::update_board()
{
    auto [pos_r, pos_c] = active_mino.get_pos();
    uint16_t mino_mask = 0b1111000000000000;
    uint16_t mino_shape = active_mino.get_shape();
    uint16_t mino_row;

    for (int i = 3; i >= 0; --i, mino_mask >>= 4, pos_r++)
    {
        mino_row = mino_mask & mino_shape;
        mino_row >>= i * 4;
        mino_row <<= (6 - pos_c);
        game_board[pos_r] |= mino_row;
    }
}

/**
 * @brief 테스트용 콘솔 출력 함수
 */
void Board::draw_board()
{
    static bool called = false;

    if (called) std::cout << "\x1b[20A";
    else called = true;

    for (int r = 2; r < 22; ++r) 
    {
        for (uint16_t mask = left_edge; mask > 0u; mask >>= 1)
        {
            cout << ((game_board[r] & mask) ? "■" : "□");
        }
        cout << endl;
    }
    cout << flush;
}