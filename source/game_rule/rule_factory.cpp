#include "game_rule/rule_factory.hpp"

using namespace std;

GameRule* create_rule(string game_mode, Board& board)
{
    return &ZEN(board);
}