add_executable(main "terminal-main.cpp")

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/platform/Terminal)

target_link_libraries(main
    PRIVATE
        engine
        board
        game_rule
        tetromino
        util
        input
        render
        network
)