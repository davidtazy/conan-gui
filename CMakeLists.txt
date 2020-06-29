cmake_minimum_required(VERSION 3.7.0)
project(conan-gui)

# dependencies from conan #########
if(EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

# modern C++ flags######################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# qt ui
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5Widgets REQUIRED)

# library
add_library(conangui_core STATIC src/qt/main_view.ui src/qt/main_view.h
                                 src/qt/main_view.cpp resource/resource.qrc)
target_link_libraries(conangui_core PUBLIC Qt5::Widgets)

# Application ###################
add_executable(conan-gui src/main.cpp)
target_link_libraries(conan-gui PRIVATE conangui_core
)# $<TARGET_OBJECTS:conangui_core>)

# test ########
return()
find_package(Catch2 REQUIRED)

add_executable(test_conan-gui test/test_game_logic.cpp test/main_catch.cpp)
target_compile_definitions(
  test_conan-gui PUBLIC UNITEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")
target_link_libraries(test_conan-gui Catch2::Catch2)

# test coverage with kcov and ctest ######################

find_program(KCOV kcov)
message("kcov: ${KCOV}")
if(KCOV)
  enable_testing()

  add_test(
    NAME test_conan-gui_coverage
    COMMAND
      ${KCOV} --exclude-pattern=/usr/
      --include-path=${CMAKE_CURRENT_LIST_DIR}/src cov
      $<TARGET_FILE:test_conan-gui>)
endif()
