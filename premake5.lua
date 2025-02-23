workspace "ChaosX"
    architecture "x64"

    configurations { "Debug", "Release", "Dist" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ChaosX"
    location "ChaosX"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")   -- Output path for DLL
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")   -- Intermediate object files

    pchheader "cspch.h"
    pchsource "ChaosX/Src/cspch.cpp"
    files 
    {
        "%{prj.name}/Src/**.h",
        "%{prj.name}/Src/**.cpp",
        "%{prj.name}/Src/Event/**.h",   -- Include Event folder header files
        "%{prj.name}/Src/Event/**.cpp"  -- Include Event folder source files
    }

    includedirs
    {
        "%{prj.name}/Lib/spdlog/include",
        "%{prj.name}/Src",   -- For ChaosX core headers
        "%{prj.name}/Src/Event"  -- For ChaosX event headers (if you want to include them directly)
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        defines 
        { 
            "CS_PLATFORM_WINDOWS",
            "CS_BUILD_DLL"
        }

        postbuildcommands 
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "CS_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "CS_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CS_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"  -- Generates an EXE
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")   -- Output path for EXE
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")   -- Intermediate object files

    files 
    {
        "%{prj.name}/Src/**.h",
        "%{prj.name}/Src/**.cpp"
    }

    includedirs
    {
        "ChaosX/Lib/spdlog/include",
        "ChaosX/Src",    -- For ChaosX core headers
        "ChaosX/Event"   -- For ChaosX event headers (if needed)
    }

    links { "ChaosX" }  -- Link the ChaosX DLL

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        defines 
        { 
            "CS_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CS_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "CS_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CS_DIST"
        optimize "On"
