cmake_minimum_required(VERSION 3.15)
project(tetris LANGUAGES CXX)

# C++ 표준
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 실행 파일 생성
add_executable(tetris
    src/main.cpp
    src/board/board.cpp
    src/block/block.cpp
)

# include 경로 지정 (핵심)
target_include_directories(tetris
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
