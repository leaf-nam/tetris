#include "game_rule/game_rule.hpp"

using namespace std;

RuleEngine::RuleEngine(Board& board) : level_game_time(0), current_level(1), board(board)
{
    // Game play rule
    enable_hard_drop = true;
    enable_hold = true;
    enable_kick = true;

    // 120 == 1 minute, when timer is 500ms
    time_for_level_up[0] = 0;
    time_for_level_up[1] = 0;
    time_for_level_up[2] = 120;
    time_for_level_up[3] = 120;
    time_for_level_up[4] = 120;
    time_for_level_up[5] = 120;
    time_for_level_up[6] = 120;
    time_for_level_up[7] = 120;
    time_for_level_up[8] = 120;
    time_for_level_up[9] = 120;
    time_for_level_up[10] = 120;
}

/**
 * @brief 유저 인풋을 처리하여 board에 지시
 * @param user_input
 */
void RuleEngine::process(int user_input)
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
            if (enable_hard_drop)
            {
                auto [new_r, new_c, new_rot] = action.resolve_move(curr_r, curr_c, curr_rot, Action::DROP);
                while (board.move_active_mino(new_r, new_c, new_rot, MoveOption::FIX_IF_FAIL)) new_r++;
            }
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

int RuleEngine::update_score(Board& board)
{
    int score = 0;
    int base_score = 100;
    int r = 21;
    while (r >= 2) 
    {
        if (board.is_line_full(r))
        {
            board.delete_line(r);
            score += base_score;
        }
        else
        {
            --r;
        }
    }
    return score;
}

bool RuleEngine::is_game_over(const uint16_t *board)
{
    for (int r = 0; r < 2; ++r) 
    {
        if (board[r]) return true;
    }

    return false;
}

bool RuleEngine::time_and_level_update()
{
    level_game_time++;
    if ((current_level < 10) && (time_for_level_up[current_level + 1] <= level_game_time))
    {
        level_game_time = 0;
        current_level++;
        return true;
    }
    return false;
}

int RuleEngine::get_level()
{
    return current_level;
}
