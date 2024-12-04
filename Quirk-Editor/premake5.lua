project "Quirk-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"
    
    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "../Quirk/src",
        "../%{IncludeDir.spdlog}",
        "../%{IncludeDir.Imgui}",
        "../%{IncludeDir.Glm}",
        "../%{IncludeDir.Opengl}",
        "../%{IncludeDir.Stb_Image}",
        "../%{IncludeDir.Entt}",
        "../%{IncludeDir.Yaml_CPP}",
        "../%{IncludeDir.ImGuizmo}",
        "../%{IncludeDir.Box2d}"
    }

    links { "Quirk" }

    filter "system:windows"
        systemversion "latest"
        entrypoint "WinMainCRTStartup"

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
        optimize "On"

    filter "configurations:Dist"
        defines "QK_DIST"
        runtime "Release"
        symbols "Off"
        optimize "Full"
