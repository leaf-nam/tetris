include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_custom_target(windows-test
    COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR}/test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(linux-test
    COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR}/test
    COMMAND lcov --quiet --directory . --capture --ignore-errors mismatch --output-file coverage.info
    COMMAND lcov --quiet --remove coverage.info '/usr/*' '*/_deps/*' '*/test/*' '*/include/*'
                 --output-file coverage.filtered.info
    COMMAND genhtml coverage.filtered.info --output-directory coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

enable_testing()

add_subdirectory(core/util)
add_subdirectory(core/board)
#add_subdirectory(core/game_rule)
add_subdirectory(core/tetromino)
add_subdirectory(core/engine)
#add_subdirectory(core/tetris)

if(WIN32)
    add_subdirectory(platform/window/util)
endif()

add_executable(
    unit_test
    main_unit_test.cpp
)

target_link_libraries(
    unit_test
    $<TARGET_OBJECTS:board_test>
#    game_rule_test
    $<TARGET_OBJECTS:tetromino_test>
    $<TARGET_OBJECTS:util_test>
    $<TARGET_OBJECTS:engine_test>
#    tetris_test
    engine
    GTest::gtest_main
)

if(WIN32)
    target_link_libraries(unit_test 
        shadow_maker_test
)
endif()

include(GoogleTest)
gtest_discover_tests(unit_test)
