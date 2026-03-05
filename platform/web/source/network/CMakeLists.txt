# platform/web/source/network/CMakeLists.txt
add_library(web_network INTERFACE)

target_include_directories(web_network INTERFACE
  ${CMAKE_SOURCE_DIR}/platform/common/include
  ${CMAKE_SOURCE_DIR}/core/include
)