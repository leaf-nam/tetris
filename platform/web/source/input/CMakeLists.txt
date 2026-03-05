# platform/web/source/input/CMakeLists.txt
add_library(web_input INTERFACE)

# 나중에 C++ 구현이 생기면 INTERFACE -> STATIC으로 바꾸고 cpp 추가하면 됨.
# add_library(web_input STATIC input_xxx.cpp)

target_include_directories(web_input INTERFACE
  ${CMAKE_SOURCE_DIR}/platform/common/include
  ${CMAKE_SOURCE_DIR}/core/include
)