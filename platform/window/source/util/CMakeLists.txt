add_library(window_util shadow_maker.cpp)

target_include_directories(window_util 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/api
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
