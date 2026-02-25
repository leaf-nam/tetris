add_library(network linux_network.cpp)

target_include_directories(network
    PUBLIC ${PROJECT_SOURCE_DIR}/core/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
