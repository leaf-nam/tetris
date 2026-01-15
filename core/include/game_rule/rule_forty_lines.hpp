#ifndef __RULE_FORTY_LINES_HPP__
#define __RULE_FORTY_LINES_HPP__

#include "game_rule/rule.hpp"
#include "board/board.hpp"

class FortyLines : public GameRule
{
    private:
    int complete_lines;

    public:
    FortyLines(Board& board);
    bool is_game_clear() override;
    bool is_game_over() override;
    int update_score() override;
    bool time_and_level_update() override;
    int get_level() override;
};

#endif
