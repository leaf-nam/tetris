cmake_minimum_required(VERSION 3.11)

project(
  ModooCode
  VERSION 1
  DESCRIPTION "Tetris"
  LANGUAGES CXX
)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(BLOCK_DIR ${CMAKE_SOURCE_DIR}/src/block)
set(BOARD_DIR ${CMAKE_SOURCE_DIR}/src/board)
set(INPUT_DIR ${CMAKE_SOURCE_DIR}/src/input)
set(RENDER_DIR ${CMAKE_SOURCE_DIR}/src/render)

add_executable (tetris
	${SRC_DIR}/main.cpp
	${RENDER_DIR}/render.cpp
	${INPUT_DIR}/input.cpp
	${BOARD_DIR}/board.cpp
	${BLOCK_DIR}/block.cpp
)

target_include_directories(tetris PUBLIC ${CMAKE_SOURCE_DIR}/include/tetris)