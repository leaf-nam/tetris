#include "game_rule/game_rule.hpp"

RuleEngine::RuleEngine() : level_game_time(0), current_level(1)
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
