#include "board/board.hpp"
#include "input/action.hpp"
#include "util/rand_gen.hpp"
#include "tetromino/tetromino_queue.hpp"

using namespace std;

static const uint16_t left_edge = 1u << 12;
static const uint16_t right_edge = 1u << 3;
static const uint16_t full_line =  ((left_edge << 1) - 1) ^ (right_edge - 1);

Board::Board()
{
    is_mino_active = false;

    for (size_t i = 0; i < 22; ++i)
    {
        game_board[i] = 0u;
    }

    saved_mino.set_mino_type(-1);
}

/**
 * @brief active tetromino의 위치 반환
 * @return pair<int, int> 순서대로 row, col
 */
pair<int, int> Board::get_active_mino_pos()
{
    return active_mino.get_pos();
}

/**
 * @brief active tetromino의 회전 반환
 * @return 회전 (0 ~ 3)
 */
int Board::get_active_mino_rotation()
{
    return active_mino.get_rotation();
}

/**
 * @brief active tetromino의 타입 반환
 * @return 타입 (0 ~ 6)
 */
int Board::get_active_mino_type()
{
    return active_mino.get_mino_type();
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
 * @brief swap된 mino가 있는지 반환
 * @return swap된 mino가 있으면 true 반환
 */
bool Board::has_swaped_mino()
{
    return is_mino_swaped;
}

/**
 * @brief 테트로미노가 새로운 위치와 회전 상태일 때 보드 상에서 충돌이 있는 지 판정
 * @return false: 충돌 / true: 충돌 없음
 */
bool Board::can_move_mino(int new_r, int new_c, int new_rot)
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
        if ((r >= 22 && mino_row) ) return false; // 보드 바깥으로 나가는지 판정
        if (r < 22 && game_board[r] & mino_row) return false; // 보드 안에 쌓여있는 블록과 충돌하는지 확인
        if ((mino_row & left_edge << 1) || (mino_row & right_edge >> 1)) return false; // 보드 벽과 충돌하는지 확인
    }

    return true;
}

/**
 * @brief 테트로미노를 지정한 위치로 이동
 * @param new_r
 * @param new_c
 * @param new_rot
 * @param move_option DISMISS_IF_FAIL: 충돌 발생하면 무시 / FIX_IF_FAIL: 충돌 발생하면 board에 고정
 * @return false: 이동 중 충돌 발생 / true: 이동 성공
 */
bool Board::move_active_mino(int new_r, int new_c, int new_rot, MoveOption move_option)
{
    bool res = false;
    
    if (can_move_mino(new_r, new_c, new_rot) && is_mino_active)
    {
        active_mino.set_pos(new_r, new_c);
        active_mino.set_rotation(new_rot);
        res = true;
    }
    else if (move_option == MoveOption::FIX_IF_FAIL)
    {
        update_board();
    }

    return res;
}

/**
 * @brief 새 테트로미노를 스폰
 * @param type 테트로미노의 타입
 * @return 테트토미노의 스폰 위치에 장애물이 있는 경우 false 반환
 */
bool Board::spawn_mino(int type)
{
    is_mino_swaped = false;
    active_mino.init_mino(type);

    is_mino_active = can_move_mino(0, 3, 0);
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

    is_mino_active = false;
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

void Board::draw_saved_mino()
{
    static bool called = false;
    mino mino_mask = 0b1111000000000000;
    mino mino_shape;
    static const uint16_t left_edge = 1u << 12;
    static const uint16_t right_edge = 1u << 3;
    uint16_t mino_row;
    int saved_mino_type = saved_mino.get_mino_type();

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
 * @brief 게임 화면 렌더링
 */
void Board::render()
{
    draw_board();
    draw_mino();
    draw_saved_mino();
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

bool Board::insert_line(int ins_row)
{
    RandGen& rand_gen = RandGen::get_instance();
    auto [curr_r, curr_c] = active_mino.get_pos();
    int curr_rot = active_mino.get_rotation();
    int new_r = curr_r;
    int index = 0;

    for (int r = ins_row; r < 22; ++r)
    {
        game_board[r - ins_row] = game_board[r];
    }
    for (int r = 21; r > 21-ins_row; --r)
    {
        game_board[r] = ~(1 << ((rand_gen.get_rand_int() % 10) + 3));
    }

    while (!can_move_mino(new_r, curr_c, curr_rot) && new_r > 0)
    {
        new_r--;
        index++;
    }

    if(index == 0) return true;
    else if(new_r < 2)
    {
        active_mino.set_pos(new_r, curr_c);
        return false;
    }
    else
    {
        active_mino.set_pos(new_r, curr_c);
        update_board();
        is_mino_active = false;
        return true;
    }
}

bool Board::is_line_full(int row)
{
    if (row >= 22 || row < 0) return false;
    return (full_line == game_board[row]);
}

void Board::swap_mino()
{
    auto [curr_r, curr_c] = active_mino.get_pos();
    int curr_rot = active_mino.get_rotation();
    int temp_mino_type;
    TetrominoQueue& tetromino_queue = TetrominoQueue::get_instance();

    if (saved_mino.get_mino_type() == -1)
    {
        saved_mino.init_mino(active_mino.get_mino_type());
        active_mino.init_mino(tetromino_queue.get_new_tetromino());
        active_mino.set_pos(curr_r, curr_c);
        if (can_move_mino(curr_r, curr_c, 0) == false)
        {
            tetromino_queue.set_new_tetromino(active_mino.get_mino_type());
            active_mino.set_mino_type(saved_mino.get_mino_type());
            active_mino.set_rotation(curr_rot);
            saved_mino.set_mino_type(-1);
        }
        else
            is_mino_swaped = true;
    }
    else
    {
        temp_mino_type = saved_mino.get_mino_type();
        saved_mino.init_mino(active_mino.get_mino_type());
        active_mino.init_mino(temp_mino_type);
        active_mino.set_pos(curr_r, curr_c);
        if (can_move_mino(curr_r, curr_c, 0) == false)
        {
            temp_mino_type = saved_mino.get_mino_type();
            saved_mino.set_mino_type(active_mino.get_mino_type());
            active_mino.set_mino_type(temp_mino_type);
            active_mino.set_rotation(curr_rot);
        }
        else
            is_mino_swaped = true;
    }
}
