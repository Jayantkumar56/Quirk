workspace "Quirk"
    architecture "x64"
    startproject "Quirk-Editor"

configurations
{
    "Debug",
    "Release",
    "Dist"
}

flags { "MultiProcessorCompile" }

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

include "Quirk/vendor/Glad"
include "Quirk/vendor/imgui"
include "Quirk/vendor/yaml-cpp"

project "Quirk"
    location "Quirk"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/**.hpp",
        "%{prj.name}/vendor/**.h",
        "%{prj.name}/vendor/**.inl",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h"
    }

	pchheader "Qkpch.h"
	pchsource "Quirk/src/Qkpch.cpp"

    includedirs
    {
        "Quirk/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Imgui}",
        "%{IncludeDir.Glm}",
        "%{IncludeDir.Opengl}",
        "%{IncludeDir.Stb_Image}",
        "%{IncludeDir.Entt}",
        "%{IncludeDir.Yaml_CPP}"
    }

    links
    {
        "Glad",
        "imgui",
        "opengl32.lib",
        "yaml-cpp"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "QK_PLATFORM_WINDOWS",
            "_WINDLL"
        }


    filter "configurations:Debug"
        defines { 
            "QK_DEBUG" 
        }

        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "QK_RELEASE"
        runtime "Release"
        symbols "On"

    filter "configurations:Dist"
        defines "QK_DIST"
        runtime "Release"
        symbols "Off"

project "Quirk-Editor"
    location "Quirk-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "Quirk/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Imgui}",
        "%{IncludeDir.Glm}",
        "%{IncludeDir.Opengl}",
        "%{IncludeDir.Stb_Image}",
        "%{IncludeDir.Entt}",
        "%{IncludeDir.Yaml_CPP}"
    }

    links
    {
        "Quirk"
    }

    filter "system:windows"
        systemversion "latest"
        entrypoint "WinMainCRTStartup"

        defines
        {
            "QK_PLATFORM_WINDOWS",
            "_WINDLL"
        }

    filter "configurations:Debug"
        defines { 
            "QK_DEBUG" 
        }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "QK_RELEASE"
        runtime "Release"
        symbols "On"

    filter "configurations:Dist"
        defines "QK_DIST"
        runtime "Release"
        symbols "Off"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Sandbox/src",
        "Quirk/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Imgui}",
        "%{IncludeDir.Glm}",
        "%{IncludeDir.Opengl}",
        "%{IncludeDir.Stb_Image}",
        "%{IncludeDir.Entt}"
    }

    links
    {
        "Quirk"
    }

    filter "system:windows"
        systemversion "latest"
        entrypoint "WinMainCRTStartup"

        defines
        {
            "QK_PLATFORM_WINDOWS",
            "_WINDLL"
        }

    filter "configurations:Debug"
        defines { 
            "QK_DEBUG" 
        }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "QK_RELEASE"
        runtime "Release"
        symbols "On"

    filter "configurations:Dist"
        defines "QK_DIST"
        runtime "Release"
        symbols "Off"
