add_library(engine engine.cpp)

target_link_libraries(engine
	PRIVATE game_rule
	PRIVATE input
	PRIVATE board
	PRIVATE tetromino
    PRIVATE util
    PRIVATE render
)

target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/include)
