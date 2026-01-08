#ifndef __GAME_RULE_HPP__
#define __GAME_RULE_HPP__

#include "board/board.hpp"

class RuleEngine
{
    private:
    int time_for_level_up[11];
    int level_game_time;
    int current_level;
    bool enable_kick;
    Board& board;

    public:
    RuleEngine(Board& board);
    void process(int user_input);
    bool is_game_over(const uint16_t* board);
    int update_score(Board& board);
    bool time_and_level_update();
    int get_level();
};

#endif
