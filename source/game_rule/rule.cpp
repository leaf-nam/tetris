#include "game_rule/rule.hpp"

using namespace std;

GameRule::GameRule(Board& board) : board(board), enable_kick(true), enable_hold(true) {}

/**
 * @brief 유저 인풋을 처리하여 board에 지시
 * @param user_input
 */
void GameRule::process(int user_input)
{
    auto [curr_r, curr_c] = board.get_active_mino_pos();
    int curr_rot = board.get_active_mino_rotation();
    int new_r, new_c, new_rot;

    if (user_input == Action::DROP)
    {
        new_r = curr_r + 1;
        new_c = curr_c;
        new_rot = curr_rot;

        if (board.can_place_mino(new_r, new_c, new_rot)) board.set_active_mino_pos(new_r, new_c);
        else board.update_board();
    }
    else if (user_input == Action::LEFT || user_input == Action::RIGHT)
    {
        new_r = curr_r;
        new_c = (user_input == Action::LEFT ? curr_c - 1 : curr_c + 1);
        new_rot = curr_rot;
        
        if (board.can_place_mino(new_r, new_c, new_rot)) board.set_active_mino_pos(new_r, new_c);
    }
    else if (user_input == Action::ROTATE_CCW || user_input == Action::ROTATE_CW)
    {
        new_r = curr_r;
        new_c = curr_c;
        new_rot = (user_input == Action::ROTATE_CW ? curr_rot + 1 : curr_rot - 1);

        if (new_rot == -1) new_rot = 3;
        else if (new_rot == 4) new_rot = 0;

        if (board.can_place_mino(new_r, new_c, new_rot))
        {
            board.set_active_mino_pos(new_r, new_c);
            board.set_active_mino_rotation(new_rot);
        }
        
    }
    else if (user_input == Action::HARD_DROP)
    {
        new_r = curr_r;
        new_c = curr_c;
        new_rot = curr_rot;
        while (board.has_active_mino())
        {
            if (board.can_place_mino(++new_r, new_c, new_rot)) board.set_active_mino_pos(new_r, new_c);
            else board.update_board();
        }
    }
    else if (user_input == Action::SWAP)
    {
        if (board.has_swaped_mino() == false)
            board.swap_mino();
        else return;
    }
}