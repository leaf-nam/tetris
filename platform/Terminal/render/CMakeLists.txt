add_library(terminal_render terminal_renderer.cpp)

target_include_directories(terminal_render PRIVATE ${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)