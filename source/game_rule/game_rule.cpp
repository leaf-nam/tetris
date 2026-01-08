#include "game_rule/game_rule.hpp"
#include "util/action.hpp"

RuleEngine::RuleEngine(Board& board) : level_game_time(0), current_level(1), board(board), enable_kick(false)
{
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

        if (enable_kick)
        {
            /**
             * According to Standard Rotation System, tetromino's rotaion is defined as below:
             * 0: Spawn
             * R: Right turn from spawn
             * L: Left turn from spawn
             * 2: opposite direction from spawn
             */
        }
        else
        {
            if (board.can_place_mino(new_r, new_c, new_rot))
            {
                board.set_active_mino_pos(new_r, new_c);
                board.set_active_mino_rotation(new_rot);
            }
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
