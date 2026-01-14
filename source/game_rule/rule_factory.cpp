#include "game_rule/rule_factory.hpp"

using namespace std;

unique_ptr<GameRule> create_rule(string game_mode, Board& board)
{
    if (game_mode == "VERSUS") return make_unique<VERSUS>(board);
    return make_unique<ZEN>(board);
}