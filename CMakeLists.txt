
cmake_minimum_required(VERSION 3.20...4.0)
cmake_policy(SET CMP0069 NEW)

project("Quirk-Engine" CXX)

include("BuildFiles/BuildSettings.cmake")
include("BuildFiles/CompilerOptions.cmake")

add_subdirectory("Quirk")
add_subdirectory("Quirk-Editor")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Quirk-Editor")
