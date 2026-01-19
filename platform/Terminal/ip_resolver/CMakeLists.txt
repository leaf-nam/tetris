add_library(ip_resolver terminal_ip_resolver.cpp)

target_include_directories(ip_resolver 
	PUBLIC ${PROJECT_SOURCE_DIR}/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)