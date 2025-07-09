

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DRACO OFF CACHE BOOL "" FORCE)
set(ASSIMP_IGNORE_GIT_HASH ON CACHE BOOL "" FORCE)


add_subdirectory("vendor/EnTT")
add_subdirectory("vendor/Glad")
add_subdirectory("vendor/glm")
include("BuildFiles/imgui.cmake")
add_subdirectory("vendor/spdlog")
include("BuildFiles/stb.cmake")
add_subdirectory("vendor/yaml-cpp")
add_subdirectory("vendor/assimp")

target_link_libraries(${CURRENT_PROJ_NAME} PUBLIC 
    "EnTT"
    "Glad"
    "glm"
    "imgui"
    "spdlog"
    "stb"
    "yaml-cpp"
    "opengl32.lib"
    "assimp"
)

# show all the third party libraries in a view "Dependencies"
set_target_properties(
    "Glad"
    "glm"
    "imgui"
    "spdlog"
    "stb"
    "yaml-cpp"
    PROPERTIES FOLDER "Dependencies"
)

set_target_properties(
    "assimp"
    "zlibstatic"
    "UpdateAssimpLibsDebugSymbolsAndDLLs"
    PROPERTIES FOLDER "Dependencies/Assimp"
)

if (MSVC)
    # Remove any previous /W flags and suppress all warnings
    target_compile_options(yaml-cpp PRIVATE "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/w>")
    # Also force it to not get /W3 from target_compile_options inside yaml-cpp
    set_target_properties(yaml-cpp PROPERTIES COMPILE_OPTIONS "")
else()
    target_compile_options(yaml-cpp PRIVATE -w)
endif()
