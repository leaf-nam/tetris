#include "game_rule/rule_forty_lines.hpp"

#include "util/action.hpp"

FortyLines::FortyLines(Board& board) : GameRule(board), complete_lines(0) {}

int FortyLines::update_score()
{
    return complete_lines; // FortyLines에서는 score가 삭제한 line 수로 대체됨
}

bool FortyLines::is_game_clear() { return complete_lines < 40; }

bool FortyLines::is_game_over()
{
    const BoardT& game_board = board.get_board();
    for (int r = 0; r < 2; ++r) {
        if (game_board[r] != 0) return true;
    }

    return false;
}

bool FortyLines::time_and_level_update()
{
    return false; // FortyLines 모드에서는 시간에 따른 level 승급 없음
}

int FortyLines::get_level()
{
    return 0; // FortyLines 모드에서는 level 시스템 없음
}
