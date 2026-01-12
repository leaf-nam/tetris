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
    int mino_type = board.get_active_mino_type();

    vector<Pose> poses;
    size_t size;

    switch (user_input)
    {
        case Action::SWAP:
        {
            if (enable_hold) board.swap_mino();
            break;
        }
        case Action::DROP:
        {
            auto [new_r, new_c, new_rot] = action.resolve_move(curr_r, curr_c, curr_rot, user_input);
            board.move_active_mino(new_r, new_c, new_rot, MoveOption::FIX_IF_FAIL);
            break;
        }
        case Action::LEFT:
        case Action::RIGHT:
        {
            auto [new_r, new_c, new_rot] = action.resolve_move(curr_r, curr_c, curr_rot, user_input);
            board.move_active_mino(new_r, new_c, new_rot, MoveOption::DISMISS_IF_FAIL);
            break;
        }
        case Action::HARD_DROP:
        {
            auto [new_r, new_c, new_rot] = action.resolve_move(curr_r, curr_c, curr_rot, Action::DROP);
            while (board.move_active_mino(new_r, new_c, new_rot, MoveOption::FIX_IF_FAIL)) new_r++;
            break;
        }
        case Action::ROTATE_CW:
        case Action::ROTATE_CCW:
        {
            poses = action.resolve_rotation(curr_r, curr_c, curr_rot, mino_type, user_input, enable_kick);
            size = poses.size();

            for (size_t i = 0; i < size; ++i)
            {
                auto& [new_r, new_c, new_rot] = poses[i];
                if (board.move_active_mino(new_r, new_c, new_rot, MoveOption::DISMISS_IF_FAIL)) break;
            }

            break;
        }
    }
}