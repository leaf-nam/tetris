add_library(util rand_gen.cpp timer.cpp)

target_include_directories(util PUBLIC ${PROJECT_SOURCE_DIR}/include)
