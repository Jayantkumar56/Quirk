
# c/c++ runtime library
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-static-libstdc++ -static-libgcc)
endif()
