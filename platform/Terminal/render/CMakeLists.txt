add_library(render terminal_renderer.cpp)

target_include_directories(render 
	PUBLIC ${PROJECT_SOURCE_DIR}/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)