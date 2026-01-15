add_library(engine_wrapper SHARED engine_wrapper.cpp)

target_include_directories(engine_wrapper 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PRIVATE ${PROJECT_SOURCE_DIR}/platform/wpf/wpf_dll/include
)

target_link_libraries(engine_wrapper
    PRIVATE
        engine
        board
        game_rule
        tetromino
        util
        wpf_input
        wpf_render
)