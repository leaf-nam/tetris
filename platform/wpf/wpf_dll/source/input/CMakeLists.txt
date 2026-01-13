add_library(wpf_input wpf_input_handler.cpp)

target_include_directories(wpf_input PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(wpf_input PUBLIC ${PROJECT_SOURCE_DIR}/platform/wpf/wpf_dll/include)
