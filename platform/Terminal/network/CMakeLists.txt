add_library(network "terminal-network.cpp")

target_include_directories(network PUBLIC ${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)