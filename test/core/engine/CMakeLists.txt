add_library(engine_test solo_engine_test.cpp multi_engine_test.cpp) 

target_link_libraries(engine_test GTest::gtest)

target_include_directories(engine_test 
    PUBLIC ${PROJECT_SOURCE_DIR}/api
    PUBLIC ${PROJECT_SOURCE_DIR}/core/include
    PUBLIC ${PROJECT_SOURCE_DIR}/test/mocks
)

set_target_properties(engine_test PROPERTIES CXX_STANDARD 17)
