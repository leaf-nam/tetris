add_executable(tetris
	tetris.cpp
)

target_link_libraries(tetris
	PRIVATE render
	PRIVATE game_rule
	PRIVATE console_input
	PRIVATE board
	PRIVATE tetromino
    PRIVATE util
	PRIVATE Engine
)

if (UNIX)
    target_include_directories(tetris PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include)
endif()
