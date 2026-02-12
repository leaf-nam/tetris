add_library(engine engine.cpp solo_engine.cpp multi_engine.cpp)

target_include_directories(engine 
	PUBLIC ${PROJECT_SOURCE_DIR}/core/api	
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
	PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api	
	PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
	PUBLIC ${PROJECT_SOURCE_DIR}/setting/include
)

target_link_libraries(engine
    PUBLIC 
		game_rule
		board
		tetromino
		util
)
