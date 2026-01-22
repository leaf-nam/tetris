add_library(ip_resolver_network window_ip_resolver_network.cpp)

target_include_directories(ip_resolver_network 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
