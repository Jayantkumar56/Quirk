
set(STB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb")
set(STB_GENERATED_CPP "${CMAKE_CURRENT_BINARY_DIR}/vendor/stb/StbImpl.cpp")

file(WRITE ${STB_GENERATED_CPP}
    "#define STB_IMAGE_IMPLEMENTATION\n#include \"${STB_SOURCE_DIR}/stb_image.h\"\n"
)

add_library(stb STATIC ${STB_GENERATED_CPP})

target_include_directories(stb PUBLIC ${STB_SOURCE_DIR})

enable_release_optimizations_for(${CURRENT_PROJ_NAME})
add_compiler_flags_for(${CURRENT_PROJ_NAME})
