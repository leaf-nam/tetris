#ifndef __GAME_RULE_HPP__
#define __GAME_RULE_HPP__

#include "board/board.hpp"
#include "input/action.hpp"
#include "game_rule/action_resolver.hpp"

class RuleEngine
{
    private:
    int time_for_level_up[11];
    int level_game_time;
    int current_level;
    bool enable_kick;
    bool enable_hold;
    bool enable_hard_drop;
    Board& board;
    ActionResolver action;

    public:
    RuleEngine(Board& board);
    void process(int user_input);
    bool is_game_over(const uint16_t* board);
    int update_score(Board& board);
    bool time_and_level_update();
    int get_level();
};

#endif
