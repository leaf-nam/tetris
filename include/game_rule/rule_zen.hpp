#ifndef __GAME_RULE_HPP__
#define __GAME_RULE_HPP__

#include "game_rule/rule.hpp"
#include "board/board.hpp"

class ZEN : public GameRule
{
    private:
    int time_for_level_up[11];
    int level_game_time;
    int current_level;

    public:
    ZEN(Board& board);
    bool is_game_clear() override;
    bool is_game_over() override;
    int update_score() override;
    bool time_and_level_update() override;
    int get_level() override;
};

#endif
