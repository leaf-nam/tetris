# platform/web/source/network/CMakeLists.txt
add_library(web_network
  web_lobby_network.cpp
)

target_include_directories(web_network
  PUBLIC ${CMAKE_SOURCE_DIR}/platform/common/include
  PUBLIC ${CMAKE_SOURCE_DIR}/core/include
  PUBLIC ${CMAKE_SOURCE_DIR}/lobby/api
  PUBLIC ${CMAKE_SOURCE_DIR}/lobby/include
  PUBLIC ${CMAKE_SOURCE_DIR}/platform/web/include
)
