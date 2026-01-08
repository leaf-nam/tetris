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
