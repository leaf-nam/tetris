add_library(network linux_network.cpp)

target_include_directories(network
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
