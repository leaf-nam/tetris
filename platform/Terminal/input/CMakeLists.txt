add_library(terminal_input terminal_input.cpp)

target_include_directories(terminal_input PRIVATE ${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)