#ifndef __RULE_FACTORY_HPP__
#define __RULE_FACTORY_HPP__

#include "game_rule/rule.hpp"
#include "game_rule/rule_zen.hpp"
#include "game_rule/rule_versus.hpp"

#include <memory>

std::unique_ptr<GameRule> create_rule(const std::string& game_mode, Board& board);

#endif