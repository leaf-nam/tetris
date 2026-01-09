add_library(linux_render linux_render.cpp)

target_include_directories(linux_render 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
