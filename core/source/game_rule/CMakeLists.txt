add_library(game_rule rule.cpp rule_zen.cpp rule_blitz.cpp rule_forty_lines.cpp rule_factory.cpp wall_kick.cpp action_resolver.cpp)

target_include_directories(game_rule PUBLIC ${PROJECT_SOURCE_DIR}/core/include)

set_target_properties(game_rule PROPERTIES CXX_STANDARD 17)