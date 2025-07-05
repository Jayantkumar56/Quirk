
add_subdirectory("vendor/EnTT")
add_subdirectory("vendor/Glad")
add_subdirectory("vendor/glm")
include("BuildFiles/imgui.cmake")
add_subdirectory("vendor/spdlog")
include("BuildFiles/stb.cmake")
add_subdirectory("vendor/yaml-cpp")

target_link_libraries(${CURRENT_PROJ_NAME} PUBLIC 
    "EnTT"
    "Glad"
    "glm"
    "imgui"
    "spdlog"
    "stb"
    "yaml-cpp"
    "opengl32.lib"
)

# show all the third party libraries in a view "Dependencies"
set_target_properties(
    "Glad"
    "glm"
    "imgui"
    "spdlog"
    "stb"
    "yaml-cpp"
    "yaml-cpp-parse"
    "yaml-cpp-read"
    "yaml-cpp-sandbox"
    PROPERTIES FOLDER "Dependencies"
)

if (MSVC)
    # Remove any previous /W flags and suppress all warnings
    target_compile_options(yaml-cpp PRIVATE "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/w>")
    # Also force it to not get /W3 from target_compile_options inside yaml-cpp
    set_target_properties(yaml-cpp PROPERTIES COMPILE_OPTIONS "")
else()
    target_compile_options(yaml-cpp PRIVATE -w)
endif()
