cmake_minimum_required(VERSION 3.7.0)
project(tetris)


########### dependencies from cmake #########
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

########### modern C++ flags######################
set (CMAKE_CXX_STANDARD             17)
set (CMAKE_CXX_STANDARD_REQUIRED    ON)
set (CMAKE_CXX_EXTENSIONS           OFF)



########### Tetris Application ###################

add_executable(tetris src/main.cpp)


######## test ########

find_package(Catch2 REQUIRED)

add_executable(test_tetris  test/test_game_logic.cpp test/main_catch.cpp)
target_compile_definitions(test_tetris PUBLIC UNITEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")
target_link_libraries(test_tetris  Catch2::Catch2 )


######### test coverage with kcov and ctest ######################

find_program(KCOV kcov)
message("kcov: ${KCOV}")
if( KCOV )
    enable_testing()

    add_test(
        NAME tetris_coverage 
        COMMAND ${KCOV} --exclude-pattern=/usr/ --include-path=${CMAKE_CURRENT_LIST_DIR}/src  cov $<TARGET_FILE:test_tetris>  
        )
endif()
