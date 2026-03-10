add_library(shadow_maker_test OBJECT shadow_maker_test.cpp) 

target_link_libraries(shadow_maker_test GTest::gtest)

target_include_directories(shadow_maker_test
	PUBLIC ${PROJECT_SOURCE_DIR}/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/common/include
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/window/include
)

set_target_properties(shadow_maker_test PROPERTIES CXX_STANDARD 17)
