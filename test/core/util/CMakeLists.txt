add_library(util_test OBJECT util_test.cpp) 

target_link_libraries(util_test GTest::gtest)

target_include_directories(util_test PUBLIC ${PROJECT_SOURCE_DIR}/core/include)

set_target_properties(util_test PROPERTIES CXX_STANDARD 17)
