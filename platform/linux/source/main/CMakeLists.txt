add_executable(tetris
	tetris.cpp
)

target_include_directories(tetris 
    PRIVATE ${PROJECT_SOURCE_DIR}/api
    PRIVATE ${PROJECT_SOURCE_DIR}/core/include
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)

target_link_libraries(tetris
    PRIVATE
        engine
        input
        render
        network
)