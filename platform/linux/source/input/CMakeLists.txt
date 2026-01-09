add_library(input linux_input.cpp)

target_include_directories(input
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
