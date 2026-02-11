add_library(window_lobby_input_handler window_lobby_input_handler.cpp)

target_include_directories(window_lobby_input_handler
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
