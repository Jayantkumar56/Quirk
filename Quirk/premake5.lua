group "Dependencies"
include "vendor/Glad"
include "vendor/imgui"
include "vendor/yaml-cpp"
group ""

project "Quirk"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("../bin/" .. "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" .. "/%{prj.name}")
    objdir ("../bin-int/" .. "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/**.hpp",
        "vendor/**.h",
        "vendor/**.inl",
        "vendor/stb_image/**.cpp",
        "vendor/stb_image/**.h"
    }

	pchheader "Qkpch.h"
	pchsource "src/Qkpch.cpp"

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "vendor/Glad/include",
        "vendor/imgui",
        "vendor/glm",
        "vendor/OpenGL",
        "vendor/stb_image",
        "vendor/entt/include",
        "vendor/yaml-cpp/include"
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
        defines "QK_DEBUG" 
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "QK_RELEASE"
        runtime "Release"
        symbols "On"
        optimize "On"

    filter "configurations:Dist"
        defines "QK_DIST"
        runtime "Release"
        symbols "Off"
        optimize "Full"
