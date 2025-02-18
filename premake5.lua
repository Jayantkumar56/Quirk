workspace "Quirk"
    architecture   "x64"
    startproject   "Quirk-Editor"
    configurations { "Debug", "Release", "Dist" }
    flags          { "MultiProcessorCompile"    }

    filter "configurations:Release"
        flags { "LinkTimeOptimization" }
    filter "configurations:Dist"
        flags { "LinkTimeOptimization" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"]    = "Quirk/vendor/spdlog/include"
IncludeDir["Glad"]      = "Quirk/vendor/Glad/include"
IncludeDir["Imgui"]     = "Quirk/vendor/imgui"
IncludeDir["Glm"]       = "Quirk/vendor/glm"
IncludeDir["Opengl"]    = "Quirk/vendor/OpenGL"
IncludeDir["Stb_Image"] = "Quirk/vendor/stb_image"
IncludeDir["Entt"]      = "Quirk/vendor/entt/include"
IncludeDir["Yaml_CPP"]  = "Quirk/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"]  = "Quirk/vendor/ImGuizmo"

include "Quirk"
include "Quirk-Editor"
