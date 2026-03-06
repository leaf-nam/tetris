add_library(game_rule_test OBJECT game_rule_test.cpp)

target_link_libraries(game_rule_test GTest::gtest)

target_include_directories(game_rule_test 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/core/include
        ${PROJECT_SOURCE_DIR}/core/api
)

set_target_properties(game_rule_test PROPERTIES CXX_STANDARD 17)
