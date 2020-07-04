cmake_minimum_required(VERSION 3.2)
project(conan-gui-interface)

#
# interface definition library
#
add_library(conangui_interfaces INTERFACE)
target_include_directories(conangui_interfaces INTERFACE include/)
add_library(conangui::interfaces ALIAS conangui_interfaces)
