add_executable(tetris
	tetris.cpp
)

target_link_libraries(tetris
	PRIVATE engine
)

if (UNIX)
    target_include_directories(tetris PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include)
endif()
