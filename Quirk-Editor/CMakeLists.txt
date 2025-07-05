
cmake_minimum_required(VERSION 3.20...4.0)
cmake_policy(SET CMP0069 NEW)

set(CURRENT_PROJ_NAME "Quirk-Editor")
add_executable(${CURRENT_PROJ_NAME})


# c++ standard to be used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# source files and include directories
include("BuildFiles/SrcFiles.cmake")
target_sources(${CURRENT_PROJ_NAME} PRIVATE ${QUIRK_EDITOR_SOURCE_FILES})
target_include_directories(${CURRENT_PROJ_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_precompile_headers(${CURRENT_PROJ_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/QkEditorpch.h")


# compiler flags and optmizations
include("BuildFiles/CompilerOptions.cmake")
enable_release_optimizations_for(${CURRENT_PROJ_NAME})
add_compiler_flags_for(${CURRENT_PROJ_NAME})


# libraries
target_link_libraries(${CURRENT_PROJ_NAME} PRIVATE Quirk)

if (WIN32)
    # Set entry point (for WinMain)
    set_target_properties(${CURRENT_PROJ_NAME} PROPERTIES
        LINK_FLAGS "/ENTRY:WinMainCRTStartup"
    )

    # Set /SUBSYSTEM per config
    set_target_properties(${CURRENT_PROJ_NAME} PROPERTIES
        LINK_FLAGS_DEBUG            "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE          "/SUBSYSTEM:WINDOWS"
        LINK_FLAGS_RELWITHDEBINFO   "/SUBSYSTEM:WINDOWS"
        LINK_FLAGS_MINSIZEREL       "/SUBSYSTEM:WINDOWS"
    )
endif()


# Find Python interpreter
find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_custom_target("Post-Build" ALL
    COMMAND ${Python3_EXECUTABLE}
        "${CMAKE_CURRENT_SOURCE_DIR}/BuildFiles/PostBuild.py"
        "$<TARGET_FILE_DIR:${CURRENT_PROJ_NAME}>"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/BuildFiles"
)

add_dependencies(${CURRENT_PROJ_NAME} "Post-Build")
