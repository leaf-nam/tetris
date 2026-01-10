#include "game_rule/rule_blitz.hpp"
#include "util/action.hpp"

BLITZ::BLITZ(Board& board) : GameRule(board) {}

int BLITZ::update_score()
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

bool BLITZ::is_game_clear()
{
    time++;
    return time < 240; // 500 ms 기준 240 tick (120 초) 가 되면 게임 오버
}

bool BLITZ::is_game_over()
{
    const uint16_t* game_board = board.get_board();
    for (int r = 0; r < 2; ++r) 
    {
        if (game_board[r]) return true;
    }

    return false;
}

bool BLITZ::time_and_level_update()
{
    return false; // BLITZ 모드에서는 시간에 따른 level up 기능 필요 없음
}

int BLITZ::get_level()
{
    return 0; // BLITZ 모드에서는 level system이 없음
}
