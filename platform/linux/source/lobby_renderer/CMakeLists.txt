add_library(linux_lobby_renderer linux_lobby_renderer.cpp)

target_include_directories(linux_lobby_renderer
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
