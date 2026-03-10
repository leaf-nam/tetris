add_library(linux_lobby_network linux_lobby_network.cpp)

target_include_directories(linux_lobby_network
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
