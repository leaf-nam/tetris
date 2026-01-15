add_library(input terminal_input.cpp)

target_include_directories(input 
	PUBLIC ${PROJECT_SOURCE_DIR}/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)