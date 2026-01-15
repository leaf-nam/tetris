add_library(render wpf_renderer.cpp)

target_include_directories(render 
	PUBLIC ${PROJECT_SOURCE_DIR}/api
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)

target_include_directories(render 
	PUBLIC ${PROJECT_SOURCE_DIR}/platform/wpf/wpf_dll/include
)
