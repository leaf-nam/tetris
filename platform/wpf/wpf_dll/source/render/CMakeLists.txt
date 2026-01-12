add_library(wpf_render wpf_renderer.cpp)

target_include_directories(wpf_render PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(wpf_render PUBLIC ${PROJECT_SOURCE_DIR}/platform/wpf/wpf_dll/include)
