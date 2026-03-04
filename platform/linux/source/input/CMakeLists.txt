add_library(input linux_input.cpp)

target_include_directories(input 
    PUBLIC ${PROJECT_SOURCE_DIR}/core/api
    PUBLIC ${PROJECT_SOURCE_DIR}/menu/api
    PUBLIC ${PROJECT_SOURCE_DIR}/setting/api
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/linux/include
)
