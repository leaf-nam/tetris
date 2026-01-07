cmake_minimum_required(VERSION 3.20)

project(dll LANGUAGES CXX)

# DLL 생성
add_library(dll SHARED
    src/WpfRendererAdapter.cpp
)

# include 경로 지정
target_include_directories(dll
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
)

# export 매크로 정의
target_compile_definitions(dll
    PRIVATE DLL_EXPORTS
)

# C++ 표준
target_compile_features(dll
    PUBLIC cxx_std_17
)