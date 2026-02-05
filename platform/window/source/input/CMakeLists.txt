add_library(input window_input.cpp)

target_include_directories(input 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/menu/api
    PUBLIC ${PROJECT_SOURCE_DIR}/menu/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
