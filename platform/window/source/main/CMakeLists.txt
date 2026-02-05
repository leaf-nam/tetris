add_executable(tetris
	tetris.cpp
)

target_include_directories(tetris 
    PRIVATE ${PROJECT_SOURCE_DIR}/api
    PRIVATE ${PROJECT_SOURCE_DIR}/core/include
    PRIVATE ${PROJECT_SOURCE_DIR}/menu/api
    PRIVATE ${PROJECT_SOURCE_DIR}/menu/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/api
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/include
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)

target_link_libraries(tetris
    PRIVATE
        engine
        menu
        input
        network
        common_render
        window_render
        window_util
        ip_resolver
        ip_resolver_network
        ip_resolver_renderer
        ip_resolver_input_handler
)
