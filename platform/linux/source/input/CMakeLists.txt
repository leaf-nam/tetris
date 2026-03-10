file(GLOB ALL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(input 
    ${ALL_SOURCES}
)

target_include_directories(input 
    PUBLIC ${PROJECT_SOURCE_DIR}/core/api
    PUBLIC ${PROJECT_SOURCE_DIR}/menu/api
    PUBLIC ${PROJECT_SOURCE_DIR}/setting/api
    PUBLIC ${PROJECT_SOURCE_DIR}/lobby/api
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/api
)
