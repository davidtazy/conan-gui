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
install(TARGETS conan-gui DESTINATION . COMPONENT applications)

install(SCRIPT deploy/deploy_qt.cmake ) 
install(DIRECTORY ${CMAKE_BINARY_DIR}/qt_deploy/ DESTINATION . COMPONENT applications)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT applications)
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

#
# CPACK macros below here
#
set (CPACK_PACKAGE_NAME ${PROJECT_NAME})
set (CPACK_PACKAGE_VENDOR "davidtazy")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "desktop interface on conan")
set (CPACK_PACKAGE_VERSION "0.0.0")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "0")
set (CPACK_PACKAGE_VERSION_PATCH "0")
set (CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/license")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/src/logo-conan.ico")

# Define components and their display names
set (CPACK_COMPONENTS_ALL applications )
set (CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "conan-gui Applications")

# Human readable component descriptions
set (CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
  "desktop interface on conan")

# Define groups
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")


# Define NSIS installation types
set(CPACK_ALL_INSTALL_TYPES Full )
set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)
 
# Must be after the last CPACK macros
include(CPack)
