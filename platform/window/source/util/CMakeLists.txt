add_library(shadow_maker_test shadow_maker.cpp)

target_include_directories(shadow_maker_test 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)
