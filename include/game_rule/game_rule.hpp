#ifndef __GAME_RULE_HPP__
#define __GAME_RULE_HPP__

#include "board/board.hpp"

class RuleEngine
{
    public:
    bool is_game_over(const uint16_t* board);
    int update_score(Board& board);
};

#endif