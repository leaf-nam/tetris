add_library(input wpf_input_handler.cpp)

target_include_directories(input 
	PUBLIC ${PROJECT_SOURCE_DIR}/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)

target_include_directories(input 
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/wpf/wpf_dll/include
)
