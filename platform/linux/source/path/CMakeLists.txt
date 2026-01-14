add_library(path linux_path.cpp)

target_include_directories(path
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
