add_library(window_lobby_network window_lobby_network.cpp)

target_include_directories(window_lobby_network
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
