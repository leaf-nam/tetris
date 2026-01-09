add_library(linux_input linux_input.cpp)

target_include_directories(linux_input 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
