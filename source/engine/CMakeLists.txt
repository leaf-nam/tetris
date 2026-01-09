add_library(engine engine.cpp)

target_link_libraries(engine
	PRIVATE game_rule
	PRIVATE board
	PRIVATE tetromino
    PRIVATE util
)

target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/include)
