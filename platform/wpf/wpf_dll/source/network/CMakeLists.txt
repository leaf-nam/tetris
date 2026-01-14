add_library(wpf_network wpf_network.cpp)

target_include_directories(wpf_network PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(wpf_network PUBLIC ${PROJECT_SOURCE_DIR}/platform/wpf/wpf_dll/include)
