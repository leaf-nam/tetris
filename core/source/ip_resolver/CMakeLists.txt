add_library(ip_resolver ip_resolver.cpp)

target_include_directories(ip_resolver 
	PUBLIC ${PROJECT_SOURCE_DIR}/api	
	PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)
