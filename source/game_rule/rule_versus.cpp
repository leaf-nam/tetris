#include "game_rule/rule_versus.hpp"
#include "util/action.hpp"

VERSUS::VERSUS(Board& board) : GameRule(board), combo(0) 
{
    combo_table[0] = 0;
    combo_table[1] = 0;
    combo_table[2] = 1;
    combo_table[3] = 1;
    combo_table[4] = 2;
    combo_table[5] = 2;
    combo_table[6] = 3;
    combo_table[7] = 4;
    combo_table[8] = 4;
    combo_table[9] = 4;
    combo_table[10] = 4;
    combo_table[11] = 5;

    garbage_table[0] = 0;
    garbage_table[1] = 0;
    garbage_table[2] = 1;
    garbage_table[3] = 2;
    garbage_table[4] = 4;
}

int VERSUS::update_score()
{
    int r = BOARD_ROW - 1;
    int complete_lines = 0;
    int garbage = 0;

    // line 완성 판정 및 보드 갱신
    while (r >= BOARD_UPPER) 
    {
        if (board.is_line_full(r))
        {
            board.delete_line(r);
            complete_lines++;
        }
        else
        {
            --r;
        }
    }

    if (complete_lines > 0)
    {
        combo++;
    } 
    else 
    {
        combo = 0;
    }

    garbage += garbage_table[complete_lines];
    garbage += (combo < 12 ? combo_table[combo] : combo_table[11]);

    return garbage;
}

bool VERSUS::is_game_clear()
{
    return false;
}

bool VERSUS::is_game_over()
{
    const board_t& game_board = board.get_board();
    for (int r = 0; r < 2; ++r) 
    {
        if (game_board[r]) return true;
    }

    return false;
}

bool VERSUS::time_and_level_update()
{
    return false;
}

int VERSUS::get_level()
{
    return 0;
}
