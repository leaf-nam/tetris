add_executable(terminal_main terminal_main.cpp)

target_include_directories(terminal_main PRIVATE ${PROJECT_SOURCE_DIR}/platform/Terminal)
target_include_directories(terminal_main PRIVATE ${PROJECT_SOURCE_DIR}/include)