add_library(ip_resolver_renderer window_ip_resolver_renderer.cpp)

target_include_directories(ip_resolver_renderer 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
