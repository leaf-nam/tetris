add_executable(tetris
	tetris.cpp
)

target_link_libraries(tetris
<<<<<<< HEAD
	PRIVATE render
	PRIVATE game_rule
	PRIVATE console_input
	PRIVATE board
	PRIVATE tetromino
    PRIVATE util
=======
	PRIVATE Engine
>>>>>>> 7f3a0260b19235bd5481043187ea35ca08b4f406
)
