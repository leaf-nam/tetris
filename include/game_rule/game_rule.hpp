#ifndef __GAME_RULE_HPP__
#define __GAME_RULE_HPP__

#include <unordered_map>
#include "board/board.hpp"
#include "game_rule/wall_kick.hpp"
#include "input/action.hpp"

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
    const std::pair<int, int>* get_kick_table(int mino_type, int curr_rot, int rot_dir);
    bool is_game_over(const uint16_t* board);
    int update_score(Board& board);
    bool time_and_level_update();
    int get_level();
};

#endif
