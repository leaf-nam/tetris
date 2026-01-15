add_library(engine_test engine_test.cpp) 

target_include_directories(engine_test PUBLIC ${PROJECT_SOURCE_DIR}/core/include)

set_target_properties(engine_test PROPERTIES CXX_STANDARD 17)
