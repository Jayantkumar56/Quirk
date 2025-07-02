
cmake_minimum_required(VERSION 3.20...4.0)
cmake_policy(SET CMP0069 NEW)

set(CURRENT_PROJ_NAME "Glad")
add_library(${CURRENT_PROJ_NAME} STATIC src/glad.c)

set_target_properties(${CURRENT_PROJ_NAME} PROPERTIES LINKER_LANGUAGE C)

target_include_directories(${CURRENT_PROJ_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${CURRENT_PROJ_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2>                    # optimize in release mode
        $<$<CONFIG:Release>:/fp:fast>               # fast floating point math
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${CURRENT_PROJ_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3>                    # optimize in release mode
        $<$<CONFIG:Release>:-ffast-math>            # fast floating point math
    )
endif()

