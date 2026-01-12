#ifndef __RULE_HPP__
#define __RULE_HPP__

#include <string>
#include <memory>
#include "util/action.hpp"
#include "board/board.hpp"
#include "game_rule/action_resolver.hpp"

class GameRule
{
    protected:
    ActionResolver action;
    Board& board;
    bool enable_kick, enable_hold;

    public:
    GameRule(Board& board);
    void process(int user_input);
    virtual bool is_game_clear() = 0;
    virtual bool is_game_over() = 0;
    virtual int get_level() = 0;
    virtual int update_score() = 0;
    virtual bool time_and_level_update() = 0;
};

#endif