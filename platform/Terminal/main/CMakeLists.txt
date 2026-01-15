add_executable(main terminal_main.cpp)

target_include_directories(main 
    PRIVATE ${PROJECT_SOURCE_DIR}/api
    PRIVATE ${PROJECT_SOURCE_DIR}/core/include
    PRIVATE ${PROJECT_SOURCE_DIR}/platform/Terminal
)

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