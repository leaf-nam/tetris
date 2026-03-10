add_library(tetromino tetromino.cpp tetromino_shape.cpp tetromino_queue.cpp)

target_include_directories(tetromino PUBLIC ${PROJECT_SOURCE_DIR}/core/include)
