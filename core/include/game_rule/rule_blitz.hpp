#ifndef __RULE_BLITZ_HPP__
#define __RULE_BLITZ_HPP__

#include "game_rule/rule.hpp"
#include "board/board.hpp"

class BLITZ : public GameRule
{
    private:
    int time;
    int score;

    public:
    BLITZ(Board& board);
    bool is_game_clear() override;
    bool is_game_over() override;
    int update_score() override;
    bool time_and_level_update() override;
    int get_level() override;
};

#endif
