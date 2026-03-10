add_library(board_test board_test.cpp) 

target_link_libraries(board_test GTest::gtest)

target_include_directories(board_test PUBLIC ${PROJECT_SOURCE_DIR}/core/include)

set_target_properties(board_test PROPERTIES CXX_STANDARD 17)
