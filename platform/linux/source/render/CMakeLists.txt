file(GLOB ALL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(linux_render 
    ${ALL_SOURCES}
)

target_include_directories(linux_render
    PUBLIC ${PROJECT_SOURCE_DIR}/core/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/menu/api
    PUBLIC ${PROJECT_SOURCE_DIR}/menu/include
    PUBLIC ${PROJECT_SOURCE_DIR}/setting/api
    PUBLIC ${PROJECT_SOURCE_DIR}/setting/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/api
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
