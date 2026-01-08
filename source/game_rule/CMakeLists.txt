add_library(game_rule game_rule.cpp wall_kick.cpp)

target_include_directories(game_rule PUBLIC ${PROJECT_SOURCE_DIR}/include)
