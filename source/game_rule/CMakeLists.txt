add_library(game_rule game_rule.cpp)

target_include_directories(game_rule PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_target_properties(game_rule PROPERTIES CXX_STANDARD 17)