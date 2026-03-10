cmake_minimum_required(VERSION 3.26)

add_executable(tetris
  tetris.cpp
)

target_link_libraries(tetris PRIVATE
  board
  game_rule
  tetromino
  util
  engine
  lobby
  menu
  setting
  web_input
  web_render
  web_network
)

target_include_directories(tetris PRIVATE
  ${CMAKE_SOURCE_DIR}/core/include
  ${CMAKE_SOURCE_DIR}/lobby/include
  ${CMAKE_SOURCE_DIR}/menu/include
  ${CMAKE_SOURCE_DIR}/setting/include
  ${CMAKE_SOURCE_DIR}/platform/common/include
)

set(SHELL_FILE "${CMAKE_CURRENT_LIST_DIR}/shell.html")

target_link_options(tetris PRIVATE
  "SHELL:--shell-file=${SHELL_FILE}"
  "SHELL:-sALLOW_MEMORY_GROWTH=1"
  "SHELL:-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','HEAP32']"
  "SHELL:-sEXPORTED_FUNCTIONS=['_main','_game_start','_game_reset','_web_set_key','_get_board_ptr','_get_active_shape_ptr','_get_meta_ptr','_get_hold_shape_ptr','_get_hold_meta_ptr','_get_next_shapes_ptr','_get_next_types_ptr']"
)

set_target_properties(tetris PROPERTIES
  OUTPUT_NAME "index"
  SUFFIX ".html"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

set(RENDERER_JS "${CMAKE_SOURCE_DIR}/platform/web/source/render/renderer.js")

add_custom_command(TARGET tetris POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${RENDERER_JS}"
          "${CMAKE_BINARY_DIR}/renderer.js"
  COMMENT "Copy renderer.js to build root"
)
