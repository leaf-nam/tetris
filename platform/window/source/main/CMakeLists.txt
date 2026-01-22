add_executable(tetris
	tetris.cpp
 "../render/menu.cpp")

target_include_directories(tetris 
    PRIVATE ${PROJECT_SOURCE_DIR}/api
    PRIVATE ${PROJECT_SOURCE_DIR}/core/include
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)

target_link_libraries(tetris
    PRIVATE
        engine
        input
        render
        network
)
