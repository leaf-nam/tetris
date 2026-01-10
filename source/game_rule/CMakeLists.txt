add_library(game_rule rule.cpp rule_zen.cpp rule_blitz.cpp rule_forty_lines.cpp rule_factory.cpp)

target_include_directories(game_rule PUBLIC ${PROJECT_SOURCE_DIR}/include)
