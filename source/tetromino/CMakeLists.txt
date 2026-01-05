add_library(tetromino tetromino.cpp tetromino_shape.cpp)

target_include_directories(tetromino PUBLIC ${PROJECT_SOURCE_DIR}/include)
