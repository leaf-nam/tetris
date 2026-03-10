file(GLOB RENDER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(common_render 
    ${RENDER_SOURCES}
 )

target_include_directories(common_render 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/api
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/include
)
