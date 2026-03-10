# platform/web/source/input/CMakeLists.txt
add_library(web_input INTERFACE)


target_include_directories(web_input INTERFACE
  ${CMAKE_SOURCE_DIR}/platform/common/include
  ${CMAKE_SOURCE_DIR}/core/include
)