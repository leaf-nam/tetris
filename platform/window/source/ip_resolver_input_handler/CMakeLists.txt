add_library(ip_resolver_input_handler window_ip_resolver_input_handler.cpp)

target_include_directories(ip_resolver_input_handler 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
