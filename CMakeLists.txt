cmake_minimum_required(VERSION 3.10) #msvc and cxx_std_17
project(conan-gui)

# project options
option(BUILD_TESTING "enable unit test" ON)

# dependencies from conan ################################
if(EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

# helper cmake function
if(BUILD_TESTING)
  include(cmake/conangui_add_test.cmake)
endif()

add_subdirectory(interface)
add_subdirectory(qtimpl)

add_library(conangui_core STATIC src/conangui.cpp src/conan.cpp src/util.cpp)
add_library(conangui::core ALIAS conangui_core)
target_compile_features(conangui_core PUBLIC cxx_std_17)
target_include_directories(conangui_core PUBLIC src)
target_link_libraries(conangui_core PUBLIC conangui::interfaces)

# Application ############################################
set(win_app_flag )
if(MSVC)
  set(win_app_flag WIN32 src/resource.rc)#no terminal + icon
endif()
add_executable(conan-gui ${win_app_flag}  src/main.cpp)
target_link_libraries(conan-gui PRIVATE conangui::core conangui::qt)

# Installation ##########################################
install(TARGETS conan-gui DESTINATION . )

install(SCRIPT deploy/deploy_qt.cmake) 

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
include(InstallRequiredSystemLibraries)

# test ###################################################

if(NOT BUILD_TESTING)
  return()
endif()

enable_testing()

conangui_add_test(
  TARGET
  test_conangui_acceptance
  SRC
  test/test_conan_acceptance.cpp
  test/main_catch.cpp
  LIBS
  conangui::core
  COV
  src
  interfaces)

conangui_add_test(
  TARGET
  test_conan-gui-core
  SRC
  test/test_conan_api.cpp
  test/main_catch.cpp
  LIBS
  conangui::core
  COV
  src
  interfaces)
