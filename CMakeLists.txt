cmake_minimum_required(VERSION 3.11)

project(
  ModooCode
  VERSION 1
  DESCRIPTION "Tetris"
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "MSVC Compiler Version: ${MSVC_VERSION}")
add_subdirectory(source/board)
add_subdirectory(source/game_rule)
add_subdirectory(source/tetromino)
add_subdirectory(source/util)
add_subdirectory(source/engine)

if (UNIX)
    add_subdirectory(platform/linux/source/input)
    add_subdirectory(platform/linux/source/render)
    add_subdirectory(platform/linux/source/path)
    add_subdirectory(platform/linux/source/network)

    # 실행파일(linux)
    add_subdirectory(source/tetris)
elseif (WIN32)
    add_subdirectory(platform/Terminal/input)
    add_subdirectory(platform/Terminal/render)
    add_subdirectory(platform/windows/source/path)

    # 실행파일(terminal_main.exe)
    add_subdirectory(platform/Terminal/main)
endif()
