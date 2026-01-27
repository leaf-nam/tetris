file(GLOB RENDER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(render 
    ${RENDER_SOURCES}

    "../util/shadow_maker.cpp" 
    "../util/setting_storage.cpp"
 )

target_include_directories(render 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
