add_executable(terminal_main terminal_main.cpp)

target_include_directories(terminal_main PRIVATE ${PROJECT_SOURCE_DIR}/platform/Terminal)
target_include_directories(terminal_main PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_options(terminal_main PRIVATE /VERBOSE:LIB)

target_link_libraries(terminal_main
    PRIVATE
        engine
        board
        game_rule
        tetromino
        util
        terminal_input
        terminal_render
)