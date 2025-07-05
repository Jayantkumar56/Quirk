
cmake_minimum_required(VERSION 3.20...4.0)
cmake_policy(SET CMP0069 NEW)

set(CURRENT_PROJ_NAME "Quirk")
add_library(${CURRENT_PROJ_NAME} STATIC)


# c++ standard to be used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# source files and include directories
include("BuildFiles/SrcFiles.cmake")
target_sources(${CURRENT_PROJ_NAME} PRIVATE ${QUIRK_SOURCE_FILES})
target_include_directories(${CURRENT_PROJ_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src" 
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/OpenGL-Registry/api/GL"
)

target_precompile_headers(${CURRENT_PROJ_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/Qkpch.h")


# compiler flags and optmizations
include("BuildFiles/CompilerOptions.cmake")
enable_release_optimizations_for(${CURRENT_PROJ_NAME})
add_compiler_flags_for(${CURRENT_PROJ_NAME})


target_compile_definitions(${CURRENT_PROJ_NAME} PUBLIC
    $<$<CONFIG:Debug>:QK_DEBUG>
    $<$<CONFIG:Release>:QK_RELEASE>
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${CURRENT_PROJ_NAME} PUBLIC "QK_PLATFORM_WINDOWS")
endif()


# third party libraries
include("BuildFiles/ExternalLibraries.cmake")
