cmake_minimum_required(VERSION 3.11)

project(
  ModooCode
  VERSION 1
  DESCRIPTION "Tetris"
  LANGUAGES CXX
)

add_subdirectory(source/board)
add_subdirectory(source/game_rule)
add_subdirectory(source/input)
add_subdirectory(source/render)    # <--- 이 줄을 추가하세요!
add_subdirectory(source/tetris)
add_subdirectory(source/tetromino)
add_subdirectory(source/util)
<<<<<<< HEAD

cmake_minimum_required(VERSION 3.11)
project(ModooCode VERSION 1)
=======
add_subdirectory(source/engine)
>>>>>>> 7f3a0260b19235bd5481043187ea35ca08b4f406
