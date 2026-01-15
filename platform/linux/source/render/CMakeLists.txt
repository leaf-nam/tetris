add_library(render linux_render.cpp)

target_include_directories(render 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
