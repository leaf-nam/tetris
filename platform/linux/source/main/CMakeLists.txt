add_executable(tetris
	tetris.cpp
)

target_include_directories(tetris 
    PRIVATE ${PROJECT_SOURCE_DIR}/core/include
    PRIVATE ${PROJECT_SOURCE_DIR}/lobby/api
    PRIVATE ${PROJECT_SOURCE_DIR}/lobby/include
    PRIVATE ${PROJECT_SOURCE_DIR}/menu/api
    PRIVATE ${PROJECT_SOURCE_DIR}/menu/include
    PRIVATE ${PROJECT_SOURCE_DIR}/setting/api
    PRIVATE ${PROJECT_SOURCE_DIR}/setting/include
    PRIVATE ${PROJECT_SOURCE_DIR}/platform/common/api
    PRIVATE ${PROJECT_SOURCE_DIR}/platform/common/include
	PRIVATE ${PROJECT_SOURCE_DIR}/platform/linux/include
)

target_link_libraries(tetris
    PRIVATE
        engine
        menu
        setting
        input
        network
        linux_render
        common_render
        linux_util
        lobby
        linux_lobby_network
        linux_lobby_renderer
        linux_lobby_input_handler
)
