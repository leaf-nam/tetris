#include "game_rule/rule_zen.hpp"
#include "util/action.hpp"

ZEN::ZEN(Board& board) : GameRule(board), level_game_time(0), current_level(1)
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

int ZEN::update_score()
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

bool ZEN::is_game_clear()
{
    return false;
}

bool ZEN::is_game_over()
{
    const board_t& game_board = board.get_board();
    for (int r = 0; r < 2; ++r) 
    {
        if (game_board[r]) return true;
    }

    return false;
}

bool ZEN::time_and_level_update()
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

int ZEN::get_level()
{
    return current_level;
}
