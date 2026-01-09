cmake_minimum_required(VERSION 3.11)

project(
  ModooCode
  VERSION 1
  DESCRIPTION "Tetris"
  LANGUAGES CXX
)

add_subdirectory(source/board)
add_subdirectory(source/game_rule)
add_subdirectory(source/tetromino)
add_subdirectory(source/util)
add_subdirectory(source/engine)
if (UNIX)
    add_subdirectory(platform/linux/source/input)
    add_subdirectory(platform/linux/source/render)
endif()
if (WIN32)
    add_subdirectory(platform/Terminal/input)
    add_subdirectory(platform/Terminal/render)

    # 실행파일(terminal_main.exe)
    add_subdirectory(platform/Terminal/main)
endif()