add_executable(tetris main.cpp)


target_link_libraries(tetris
    render
    network
    input
)

target_link_options(tetris PRIVATE 
    "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
)

# 실행 파일 최상위로 내보내기
set_target_properties(tetris PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    SUFFIX ".html"
)

