cmake_minimum_required(VERSION 3.11)

project(
  ModooCode
  VERSION 1
  DESCRIPTION "Tetris"
  LANGUAGES CXX
)

option(USE_WPF "Build WPF UI" OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(source/board)
add_subdirectory(source/game_rule)
add_subdirectory(source/tetromino)
add_subdirectory(source/util)
add_subdirectory(source/engine)

# 리눅스 터미널 UI 빌드
if (UNIX)
    add_subdirectory(platform/linux/source/path)
    add_subdirectory(platform/linux/source/input)
    add_subdirectory(platform/linux/source/render)
    add_subdirectory(platform/linux/source/network)
    add_subdirectory(source/tetris)

# 윈도우 빌드
elseif (WIN32)

    # WPF UI 빌드 -> wpf_dll.dll 생성
    if (USE_WPF)
        add_subdirectory(platform/wpf/wpf_dll/source/input)
        add_subdirectory(platform/wpf/wpf_dll/source/render)
        add_subdirectory(platform/wpf/wpf_dll/source/callback)

    # 윈도우 터미널 UI 빌드 -> terminal_main.exe 생성
    else()
        add_subdirectory(platform/Terminal/input)
        add_subdirectory(platform/Terminal/render)
        add_subdirectory(platform/Terminal/network)
        add_subdirectory(platform/Terminal/main)
    endif()

endif()
