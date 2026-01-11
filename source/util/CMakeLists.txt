add_library(util rand_gen.cpp timer.cpp)
add_library(path path.cpp)

target_include_directories(util PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(path PUBLIC ${PROJECT_SOURCE_DIR}/include)