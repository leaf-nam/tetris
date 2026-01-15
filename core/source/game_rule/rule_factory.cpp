#include "game_rule/rule_factory.hpp"

using namespace std;

unique_ptr<GameRule> create_rule(const string& game_mode, Board& board)
{
    return make_unique<ZEN>(board);
}