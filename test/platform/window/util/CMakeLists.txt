add_library(render_util_test OBJECT render_util_test.cpp) 

target_link_libraries(render_util_test GTest::gtest)

target_include_directories(render_util_test 
	PUBLIC ${PROJECT_SOURCE_DIR}/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)

set_target_properties(render_util_test PROPERTIES CXX_STANDARD 17)
