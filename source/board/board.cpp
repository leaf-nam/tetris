#include "board/board.hpp"
#include "input/action.hpp"

using namespace std;

static const uint16_t left_edge = 1u << 12;
static const uint16_t right_edge = 1u << 3;
static const uint16_t full_line =  ((left_edge << 1) - 1) ^ (right_edge - 1);

static const int dr[5] = {0, 0, 1, 0, 0};
static const int dc[5] = {-1, 1, 0, 0, 0};

enum Move_result
{
    BLOCKED_BY_WALL,
    BLOCKED,
    NON_BLOCKED
};

Board::Board()
{
    is_mino_active = false;

    for (size_t i = 0; i < 22; ++i)
    {
        game_board[i] = 0u;
    }
}

/**
 * @brief board 상의 mino가 active 상태인지 반환
 * @return mino가 active이면 true 반환
 */
bool Board::has_active_mino()
{
    return is_mino_active;
}

/**
 * @brief 테트로미노가 새로운 위치와 회전 상태일 때 보드 상에서 충돌이 있는 지 판정
 * @return 0: 벽에 충돌 / 1: 바닥이나 벽에 충돌 / 2: 충돌 없음
 */
int Board::can_move_mino(int new_r, int new_c, int new_rot, int cmd)
{
    int move_result;
    uint16_t mino_mask = 0b1111000000000000;
    uint16_t mino_shape;
    uint16_t mino_row;

    mino_shape = active_mino.get_shape(new_rot);

    for (int i = 3, r = new_r; i >= 0; --i, mino_mask >>= 4, r++)
    {
        mino_row = mino_mask & mino_shape;
        mino_row >>= i * 4;
        mino_row <<= (9 - new_c);
        if ((r >= 22 && mino_row) ) return BLOCKED;
        if (r < 22 && game_board[r] & mino_row) 
        {
            if (cmd == Action::LEFT || cmd == Action::RIGHT) return BLOCKED_BY_WALL;
            else return BLOCKED;
        }
        if ((mino_row & left_edge << 1) || (mino_row & right_edge >> 1)) return BLOCKED_BY_WALL;
    }

    return NON_BLOCKED;
}

/**
 * @brief 테트로미노 이동. 벽에 막힌 경우 무시, 바닥이나 다른 블록에 닿은 경우 해당 위치에 고정됨
 */
void Board::move_mino(int cmd)
{   
    if (!is_mino_active) return;

    auto [curr_r, curr_c] = active_mino.get_pos();
    int curr_rot = active_mino.get_rotation();
    int new_r, new_c, new_rot;
    int move_result;

    new_r = curr_r + dr[cmd], new_c = curr_c + dc[cmd];

    if (cmd == Action::ROTATE_CW) new_rot = curr_rot + 1;
    else if (cmd == Action::ROTATE_CCW) new_rot = curr_rot - 1;
    else new_rot = curr_rot;

    if (new_rot == -1) new_rot = 3;
    else if (new_rot == 4) new_rot = 0;

    move_result = can_move_mino(new_r, new_c, new_rot, cmd);

    if (move_result == NON_BLOCKED) 
    {
        active_mino.set_pos(new_r, new_c);
        active_mino.set_rotation(new_rot);
    }
    else if (move_result == BLOCKED)
    {
        update_board();
        is_mino_active = false;
    }
}

/**
 * @brief 새 테트로미노를 스폰
 * @param type 테트로미노의 타입
 * @return 테트토미노의 스폰 위치에 장애물이 있는 경우 false 반환
 */
bool Board::spawn_mino(int type)
{
    active_mino.init_mino(type);

    is_mino_active = (can_move_mino(0, 3, 0 , Action::DROP) == NON_BLOCKED);
    return is_mino_active;

    return is_mino_active;
}

/**
 * @brief 보드 상태를 업데이트
 */
void Board::update_board()
{
    if (!is_mino_active) return;

    auto [pos_r, pos_c] = active_mino.get_pos();
    uint16_t mino_mask = 0b1111000000000000;
    uint16_t mino_shape = active_mino.get_shape();
    uint16_t mino_row;

    for (int i = 3; i >= 0; --i, mino_mask >>= 4, pos_r++)
    {
        if (pos_r >= 22) break;
        mino_row = mino_mask & mino_shape;
        mino_row >>= i * 4;
        mino_row <<= (9 - pos_c);
        game_board[pos_r] |= mino_row;
    }
}

/**
 * @brief 게임 보드 출력
 */
void Board::draw_board()
{
    static bool called = false;

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
}

/**
 * @brief 게임 보드 위에 테트로미노를 출력 
 */
void Board::draw_mino() 
{
    if (is_mino_active)
    {
        std::cout << "\x1b[20A";
        auto [pos_r, pos_c] = active_mino.get_pos();
        mino mino_mask = 0b1111000000000000;
        mino mino_shape = active_mino.get_shape();
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
 * @brief 게임 화면 렌더링
 */
void Board::render()
{
    draw_board();
    draw_mino();
}

/**
 * @brief 게임 보드를 반환
 */
const uint16_t *Board::get_board() const
{
    return game_board;
}

/**
 * @brief 특정 행을 지우고, 위쪽 행을 아래쪽으로 1칸씩 땡김
 */
void Board::delete_line(int del_row)
{
    if (del_row >= 22 || del_row < 0) return;
    for (int r = del_row; r >= 1; --r)
    {
        game_board[r] = game_board[r - 1];
    }
    game_board[0] = 0u;
}

bool Board::is_line_full(int row)
{
    if (row >= 22 || row < 0) return false;
    return (full_line == game_board[row]);
}