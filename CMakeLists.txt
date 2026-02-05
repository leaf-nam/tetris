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
set_property(DIRECTORY PROPERTY
  CMAKE_CXX_CLANG_TIDY
  "clang-tidy;-warnings-as-errors=*"
)

option(USE_WPF "Build WPF UI" OFF)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Coverage enabled")

    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

add_subdirectory(core/source/board)
add_subdirectory(core/source/game_rule)
add_subdirectory(core/source/tetromino)
add_subdirectory(core/source/util)
add_subdirectory(core/source/engine)
add_subdirectory(core/source/ip_resolver)
add_subdirectory(menu/source)

# 리눅스 터미널 UI 빌드
if (UNIX)
    add_subdirectory(platform/linux/source/input)
    add_subdirectory(platform/linux/source/render)
    add_subdirectory(platform/linux/source/network)
    add_subdirectory(platform/linux/source/main)
    set_target_properties(tetris PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
# 윈도우 빌드
elseif (WIN32)

    # WPF UI 빌드 -> wpf_dll.dll 생성
    if (USE_WPF)
        add_subdirectory(platform/wpf/wpf_dll/source/input)
        add_subdirectory(platform/wpf/wpf_dll/source/render)
        add_subdirectory(platform/wpf/wpf_dll/source/callback)
    # 윈도우 터미널 UI 빌드 -> window_main.exe 생성
    else()
        add_subdirectory(platform/common/source/render)
        add_subdirectory(platform/window/source/input)
        add_subdirectory(platform/window/source/render)
        add_subdirectory(platform/window/source/network)
        add_subdirectory(platform/window/source/ip_resolver_network)
        add_subdirectory(platform/window/source/ip_resolver_renderer)
        add_subdirectory(platform/window/source/ip_resolver_input_handler)
        add_subdirectory(platform/window/source/util)
        add_subdirectory(platform/window/source/main)
        #터미널 구조에 맞게 수정했습니다. 
    # 윈도우 터미널 UI 빌드 -> tetris.exe 생성
    endif()

endif()

add_subdirectory(test)
