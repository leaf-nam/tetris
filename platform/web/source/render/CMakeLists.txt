# platform/web/source/render/CMakeLists.txt
add_library(web_render INTERFACE)

target_include_directories(web_render INTERFACE
  ${CMAKE_SOURCE_DIR}/platform/common/include
  ${CMAKE_SOURCE_DIR}/core/include
)