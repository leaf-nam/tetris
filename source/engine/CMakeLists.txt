add_library(Engine Engine.cpp)

target_link_libraries(Engine
	PRIVATE game_rule
	PRIVATE console_input
	PRIVATE board
	PRIVATE tetromino
    PRIVATE util
)

target_include_directories(Engine PUBLIC ${PROJECT_SOURCE_DIR}/include)
