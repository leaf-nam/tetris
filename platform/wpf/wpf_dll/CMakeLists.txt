cmake_minimum_required(VERSION 3.20)

project(wpf_dll LANGUAGES CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    source/*.cpp
)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../
    ${CMAKE_BINARY_DIR}/tetris_build
)

# 라이브러리 추가
add_library(wpf_dll SHARED ${SOURCES})

target_link_libraries(wpf_dll PRIVATE tetris_game)

# include 경로 지정
target_include_directories(wpf_dll 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

# export 매크로 정의
target_compile_definitions(wpf_dll 
    PRIVATE DLL_EXPORT
)

# C++ 표준
target_compile_features(wpf_dll 
    PUBLIC cxx_std_17
)