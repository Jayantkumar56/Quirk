
function(enable_release_optimizations_for TARGET)
    set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${TARGET} PRIVATE
            $<$<CONFIG:Release>:/O2>                    # optimize in release mode
            $<$<CONFIG:Release>:/fp:fast>               # fast floating point math
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${TARGET} PRIVATE
            $<$<CONFIG:Release>:-O3>                    # optimize in release mode
            $<$<CONFIG:Release>:-ffast-math>            # fast floating point math
        )
    endif()
endfunction()


function(add_compiler_flags_for TARGET)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${TARGET} PRIVATE
            /W1                                         # warning flags
            /MP                                         # parallel compilation
            /Zc:preprocessor                            # for __VA_OPT__
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${TARGET} PRIVATE
            -Wall -Wextra -Wpedantic                    # warning flags
        )
    endif()

    # enable position-independent code (PIC) only on platforms that require it (Linux, macOS)
    # this is needed when linking static libraries into shared libraries on these systems
    # windows does not require PIC due to its different DLL model
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()
endfunction()
