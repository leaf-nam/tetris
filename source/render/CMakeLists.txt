add_library(render terminal_renderer.cpp)

target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
