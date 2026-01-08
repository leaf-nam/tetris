add_executable(tetris
	tetris.cpp
)

target_link_libraries(tetris
	PRIVATE engine
)
