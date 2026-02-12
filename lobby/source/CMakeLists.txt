add_library(lobby lobby.cpp)

target_include_directories(lobby
	PUBLIC ${PROJECT_SOURCE_DIR}/core/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
	PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
	PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
)
