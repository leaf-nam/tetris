add_library(linux_lobby_input_handler linux_lobby_input_handler.cpp)

target_include_directories(linux_lobby_input_handler
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
