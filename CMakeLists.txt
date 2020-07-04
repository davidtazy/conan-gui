cmake_minimum_required(VERSION 3.7.0)
project(conan-gui)

# dependencies from conan #########
if(EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

add_subdirectory(interface)
add_subdirectory(qtimpl)

add_library(conangui_core STATIC src/Conan.cpp)
add_library(conangui::core ALIAS conangui_core)
target_compile_features(conangui_core PUBLIC cxx_std_17)
target_link_libraries(conangui_core PUBLIC conangui::interfaces)

# Application ###################
add_executable(conan-gui src/main.cpp)
target_link_libraries(conan-gui PRIVATE conangui::core conangui::qt)

# test ########
find_package(Catch2 REQUIRED)
add_executable(test_conan-gui-core test/test_game_logic.cpp test/main_catch.cpp)
target_compile_features(test_conan-gui-core PUBLIC cxx_std_17)

target_compile_definitions(
  test_conan-gui-core PUBLIC UNITEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")
target_link_libraries(test_conan-gui-core PRIVATE conangui::core Catch2::Catch2)

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
      $<TARGET_FILE:test_conan-gui-core>)
endif()
