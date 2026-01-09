#include "board/board.hpp"
#include "util/action.hpp"
#include "util/rand_gen.hpp"
#include "tetromino/tetromino_queue.hpp"

using namespace std;

Board::Board()
{
    is_mino_active = false;

    for (int r = 0; r < BOARD_ROW; ++r)
    {
        for (int c = 0; c < BOARD_COL; ++c)
        {
            game_board[r][c] = MinoType::NONE;
        }
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
 * @brief active tetromino의 위치를 수정
 * @param new_r 새로운 row 위치
 * @param new_c 새로운 col 위치
 */
void Board::set_active_mino_pos(int new_r, int new_c)
{
    active_mino.set_pos(new_r, new_c);
}

/**
 * @brief active tetromino의 회전을 수정
 * @param new_rot 새로운 회전 방향
 */
void Board::set_active_mino_rotation(int new_rot)
{
    active_mino.set_rotation(new_rot);
}

/**
 * @brief board 상의 mino가 active 상태인지 반환
 * @return mino가 active이면 true 반환
 */
const bool Board::has_active_mino() const
{
    return is_mino_active;
}

/**
 * @brief swap된 mino가 있는지 반환
 * @return swap된 mino가 있으면 true 반환
 */
const bool Board::has_swaped_mino() const
{
    return is_mino_swaped;
}

/**
 * @brief 테트로미노가 새로운 위치와 회전 상태일 때 보드 상에서 충돌이 있는 지 판정
 * @return false: 충돌 / true: 충돌 없음
 */
bool Board::can_place_mino(int new_r, int new_c, int new_rot)
{
    const mino& m = active_mino.get_shape(new_rot);

    for (int r = 0; r < MINO_SIZE; ++r)
    {
        for (int c = 0; c < MINO_SIZE; ++c)
        {
            if (is_filled(new_r + r, new_c + c) && m[r][c]) return false;
        }
    }

    return true;
}

/**
 * @brief 게임 보드에 블록을 채움
 * @return false: 해당 위치에 이미 블록이 있음 / true: 블록을 보드에 채워넣음
 * @note 범위 밖인 경우 false 반환
 */
bool Board::fill(int r, int c, int type)
{
    if (r < 0 || r >= BOARD_ROW || c < 0 || c >= BOARD_COL) return false;
    game_board[r][c] = type;
    return true;
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

    is_mino_active = can_place_mino(0, 3, 0);
    return is_mino_active;
}

/**
 * @brief 보드 상태를 업데이트
 */
void Board::update_board()
{
    if (!is_mino_active) return;

    int mino_type = active_mino.get_mino_type();
    auto [pos_r, pos_c] = active_mino.get_pos();
    const mino& m = active_mino.get_shape();

    for (int r = 0; r < MINO_SIZE; ++r)
    {
        for (int c = 0; c < MINO_SIZE; ++c)
        {
            if (m[r][c]) fill(pos_r + r , pos_c + c, mino_type);
        }
    }

    is_mino_active = false;
}

Tetromino& Board::get_active_mino()
{
    return active_mino;
}

Tetromino& Board::get_saved_mino()
{
    return saved_mino;
}

bool Board::get_is_mino_swaped()
{
    return is_mino_swaped;
}

/**
 * @brief 게임 보드를 반환
 */
const board_t& Board::get_board() const
{
    return game_board;
}

/**
 * @brief 보드의 특정 좌표에 블록이 있는지 반환
 * @return true: 블록 있음, false: 블록 없음
 * @note
 * 보드 범위를 벗어나는 좌표인 경우 0 반환
 */
const bool Board::is_filled(int r, int c) const
{
    if (r < 0 || r >= BOARD_ROW || c < 0 || c >= BOARD_COL) return true;
    return game_board[r][c] != MinoType::NONE;
}

/**
 * @brief 보드의 특정 좌표에 있는 블록의 타입을 반환
 * @return 블록의 타입 (MinoType)
 * @note
 * 보드 범위를 벗어나는 좌표인 경우 Obstacle (7) 반환
 */
const int Board::at(int r, int c) const
{
    if (r < 0 || r >= BOARD_ROW || c < 0 || c >= BOARD_COL) return MinoType::OBSTACLE;
    return game_board[r][c];
}

/**
 * @brief 특정 행을 지우고, 위쪽 행을 아래쪽으로 1칸씩 땡김
 */
void Board::delete_line(int del_row)
{
    if (del_row >= 22 || del_row < 0) return;

    for (int r = del_row; r >= 1; --r) copy(game_board[r - 1], game_board[r - 1] + BOARD_COL, game_board[r]);
    for (int c = 0; c < BOARD_COL; ++c) game_board[0][c] = MinoType::NONE;
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
        copy(game_board[r], game_board[r] + BOARD_COL, game_board[r - ins_row]);
    }

    for (int r = 21; r > 21-ins_row; --r)
    {
        int rand_idx = rand_gen.get_rand_int() % BOARD_COL;
        board_row new_row = { MinoType::OBSTACLE };
        new_row[rand_idx] = MinoType::NONE;
        copy(new_row, new_row + BOARD_COL, game_board[r]);
    }

    while (!can_place_mino(new_r, curr_c, curr_rot) && new_r > 0)
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
    for (int col = 0; col < BOARD_COL; ++col)
    {
        if (!is_filled(row, col)) return false; 
    }
    return true;
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
        if (can_place_mino(curr_r, curr_c, 0) == false)
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
        if (can_place_mino(curr_r, curr_c, 0) == false)
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
