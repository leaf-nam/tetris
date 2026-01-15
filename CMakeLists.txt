cmake_minimum_required(VERSION 3.26)

project(
  ModooCode
  VERSION 1
  DESCRIPTION "Tetrissen"
  LANGUAGES CXX
)

# 윈도우 std::byte 오류 해결용
if (WIN32)
    add_compile_definitions(_HAS_STD_BYTE=0)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -warnings-as-errors=*;
)

option(USE_WPF "Build WPF UI" OFF)

add_subdirectory(core/source/board)
add_subdirectory(core/source/game_rule)
add_subdirectory(core/source/tetromino)
add_subdirectory(core/source/util)
add_subdirectory(core/source/engine)

# 리눅스 터미널 UI 빌드
if (UNIX)
    add_subdirectory(platform/linux/source/input)
    add_subdirectory(platform/linux/source/render)
    add_subdirectory(platform/linux/source/network)
    add_subdirectory(platform/linux/source/main)

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
