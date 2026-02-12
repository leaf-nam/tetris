add_library(web_util shadow_maker.cpp setting_storage.cpp)

target_include_directories(web_util 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/platform/web/include
)
