add_library(game_rule_test game_rule_test.cpp) 

target_include_directories(game_rule_test PUBLIC ${PROJECT_SOURCE_DIR}/core/include)

set_target_properties(game_rule_test PROPERTIES CXX_STANDARD 17)
