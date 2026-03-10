add_library(board board.cpp) 

target_include_directories(board PUBLIC ${PROJECT_SOURCE_DIR}/core/include)

set_target_properties(board PROPERTIES CXX_STANDARD 17)