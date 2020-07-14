cmake_minimum_required(VERSION 3.10) #msvc and cxx_std_17
project(conan-gui-qtimpl)

# set flags
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5Widgets REQUIRED CONFIG)

# set(CMAKE_VERBOSE_MAKEFILE ON)

#
# qt library (implement ui and process)
#
add_library(
  conangui_qt STATIC
  src/qt/process.cpp
  src/qt/main_view.ui
  src/qt/main_view.h
  src/qt/remote_list_model.cpp
  src/qt/graphic_event_loop.cpp
  src/qt/main_view.cpp
  src/qt/settings.cpp
  resource/resource.qrc)

target_compile_features(conangui_qt PUBLIC cxx_std_17)
target_include_directories(conangui_qt PUBLIC src)
target_link_libraries(conangui_qt PUBLIC Qt5::Widgets conangui::interfaces)
add_library(conangui::qt ALIAS conangui_qt)
