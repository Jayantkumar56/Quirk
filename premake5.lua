workspace "Quirk"
    architecture "x64"
    startproject "Sandbox"

configurations
{
    "Debug",
    "Release",
    "Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
        "%{prj.name}/vendor/**.inl"
    }

	pchheader "Qkpch.h"
	pchsource "Quirk/src/Qkpch.cpp"

    includedirs
    {
        "Quirk/src"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "QK_PLATFORM_WINDOWS",
            "_WINDLL"
        }


    filter "configurations:Debug"
        defines { 
            "QK_ENABLE_ASSERTS", 
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
        symbols "On"

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
        "Quirk/src"
    }

    links
    {
        "Quirk"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
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

    filter "configurations:Dist"
        defines "QK_DIST"
        runtime "Release"
        symbols "On"
