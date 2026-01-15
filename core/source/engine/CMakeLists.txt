add_library(engine engine.cpp solo_engine.cpp multi_engine.cpp)

target_include_directories(engine 
	PUBLIC ${PROJECT_SOURCE_DIR}/api	
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)

target_link_libraries(engine
    PRIVATE game_rule
	PRIVATE board
	PRIVATE tetromino
    PRIVATE util
)
