add_library(render terminal_renderer.cpp)

# include 폴더가 프로젝트 루트 근처에 있다면 상대 경로로 지정
target_include_directories(render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)