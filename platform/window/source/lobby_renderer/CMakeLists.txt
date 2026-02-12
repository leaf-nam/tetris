add_library(window_lobby_renderer window_lobby_renderer.cpp)

target_include_directories(window_lobby_renderer
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
