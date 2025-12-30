#include "game_rule/game_rule.hpp"

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