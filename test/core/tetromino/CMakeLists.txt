add_library(tetromino_test tetromino_test.cpp) 

target_include_directories(tetromino_test PUBLIC ${PROJECT_SOURCE_DIR}/core/include)

set_target_properties(tetromino_test PROPERTIES CXX_STANDARD 17)
