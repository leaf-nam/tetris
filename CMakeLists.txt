cmake_minimum_required(VERSION 3.11)

project(
  ModooCode
  VERSION 1
  DESCRIPTION "Tetris"
  LANGUAGES CXX
)

add_subdirectory(source/board)
add_subdirectory(source/game_rule)
add_subdirectory(source/tetris)
add_subdirectory(source/tetromino)
add_subdirectory(source/util)
add_subdirectory(source/engine)
if (UNIX)
    add_subdirectory(platform/linux/source/input)
    add_subdirectory(platform/linux/source/render)
    add_subdirectory(platform/linux/source/path)
else (WIN32)
    add_subdirectory(platform/windows/source/path)
endif()
