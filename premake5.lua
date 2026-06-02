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

    -- Src/**.h and Src/**.cpp glob recursively, so the Event/ subfolder is
    -- already covered; no separate entries needed.
    files
    {
        "%{prj.name}/Src/**.h",
        "%{prj.name}/Src/**.cpp"
    }

    -- Includes are written relative to Src (e.g. "ChaosX/Event/Event.h"),
    -- so Src is the only project include root we need beyond spdlog.
    includedirs
    {
        "%{prj.name}/Lib/spdlog/include",
        "%{prj.name}/Src"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        -- premake's newest action (vs2022) defaults to the v143 toolset, but
        -- VS2026 ships v145. Pin it so generated projects target the installed
        -- toolset instead of one that isn't present.
        toolset "msc-v145"
        -- spdlog's bundled fmt static-asserts that MSVC compiles with /utf-8.
        -- Kept here (not as a manual VS project edit) so regenerating doesn't lose it.
        buildoptions { "/utf-8" }
        defines
        {
            "CS_PLATFORM_WINDOWS",
            "CS_BUILD_DLL"
        }

        -- Use a project-relative path: %{wks.location} expands to an absolute
        -- path containing a space ("ChaosX Engine"), which the unquoted
        -- {MKDIR}/{COPY} tokens choke on. "../bin/..." stays space-free.
        postbuildcommands
        {
            ("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
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
        "ChaosX/Src"
    }

    links { "ChaosX" }  -- Link the ChaosX DLL

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        toolset "msc-v145"
        -- Sandbox pulls in spdlog through ChaosX.h, so it needs /utf-8 too.
        buildoptions { "/utf-8" }
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
