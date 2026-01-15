#ifndef __RULE_VERSUS_HPP__
#define __RULE_VERSUS_HPP__

#include <array>
#include "game_rule/rule.hpp"
#include "board/board.hpp"

class VERSUS : public GameRule
{
private:
    int combo;
    std::array<int, 12> combo_table;
    std::array<int, MINO_KIND + 1> garbage_table;

public:
    VERSUS(Board& board);
    bool is_game_clear() override;
    bool is_game_over() override;
    int update_score() override;
    bool time_and_level_update() override;
    int get_level() override;
};

#endif
