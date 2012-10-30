require "Helium"

solution "Tools"

Helium.DoDefaultSolutionSettings()

defines
{
	"HELIUM_TOOLS=1",
}

dofile "Shared.lua"

local prefix = solution().name .. '.'

project( prefix .. "Inspect" )
	uuid "D4D7F216-5EE6-4252-BF25-0698C1BD30CD"

	Helium.DoModuleProjectSettings( ".", "HELIUM", "Inspect", "INSPECT" )

	files
	{
		"Inspect/**",
	}

    pchheader( "InspectPch.h" )
    pchsource( "Inspect/InspectPch.cpp" )

	configuration "SharedLib"
		links
		{
			prefix .. "Platform",
			prefix .. "Foundation",
			prefix .. "Reflect",
			prefix .. "Math",
		}

project( prefix .. "SceneGraph" )
	uuid "4EAB668E-2382-40d9-AA6B-7FB0BB163E7F"

	Helium.DoModuleProjectSettings( ".", "HELIUM", "SceneGraph", "SCENE_GRAPH" )

	files
	{
		"SceneGraph/*",
	}

	configuration "SharedLib"
		links
		{
			prefix .. "Platform",
			prefix .. "Foundation",
			prefix .. "Reflect",
			prefix .. "Inspect",
			prefix .. "Math",
			prefix .. "Engine",
			prefix .. "EngineJobs",
			prefix .. "Windowing",
			prefix .. "Rendering",
			prefix .. "GraphicsTypes",
			prefix .. "GraphicsJobs",
			prefix .. "Graphics",
			prefix .. "Framework",
			prefix .. "WindowingWin",
			prefix .. "RenderingD3D9",
			prefix .. "PcSupport",
			prefix .. "PreprocessingPc",
			prefix .. "EditorSupport",
			"d3d9",
		}

project( prefix .. "Editor" )
	uuid "A5CAC2F6-62BC-4EF3-A752-887F89C64812"
	kind "ConsoleApp"

	defines
	{
		"HELIUM_MODULE_HEAP_FUNCTION=GetEditorDefaultHeap",
	}
	files
	{
		"Editor/**.h",
		"Editor/**.c",
		"Editor/**.cpp",
		"Editor/Editor.rc",
	}

    pchheader( "EditorPch.h" )
    pchsource( "Editor/EditorPch.cpp" )

	Helium.DoDefaultProjectSettings()

	links
	{
		prefix .. "Platform",
		prefix .. "Foundation",
		prefix .. "Reflect",
		prefix .. "Math",
		prefix .. "Inspect",
		prefix .. "Engine",
		prefix .. "EngineJobs",
		prefix .. "Windowing",
		prefix .. "Rendering",
		prefix .. "GraphicsTypes",
		prefix .. "GraphicsJobs",
		prefix .. "Graphics",
		prefix .. "Framework",
		prefix .. "WindowingWin",
		prefix .. "RenderingD3D9",
		prefix .. "PcSupport",
		prefix .. "PreprocessingPc",
		prefix .. "EditorSupport",
		prefix .. "SceneGraph",
		"expat",
		"zlib",
		"libclient",
		"librpc",
		"libsupp",
		"d3d9",
		"ws2_32",
	}
	flags
	{
		"WinMain"
	}

	-- per architecture
	configuration { "windows", "x32" }
		libdirs
		{
			"Dependencies/wxWidgets/lib/vc_dll",
		}
	configuration { "windows", "x64" }
		libdirs
		{
			"Dependencies/wxWidgets/lib/vc_x64_dll",
		}
		
	-- per configuration
	configuration { "windows", "Debug" }
		links
		{
			"wxmsw29ud"
		}
	configuration { "windows", "not Debug" }
		links
		{
			"wxmsw29u"
		}
		
	-- per architecture, per configuration
	configuration { "windows", "x32", "Debug" }
		libdirs
		{
			"Dependencies/p4api/lib/" .. _ACTION .. "/Win32/Debug",
		}
	configuration { "windows", "x32", "not Debug" }
		libdirs
		{
			"Dependencies/p4api/lib/" .. _ACTION .. "/Win32/Release",
		}
	configuration { "windows", "x64", "Debug" }
		libdirs
		{
			"Dependencies/p4api/lib/" .. _ACTION .. "/x64/Debug",
		}
	configuration { "windows", "x64", "not Debug" }
		libdirs
		{
			"Dependencies/p4api/lib/" .. _ACTION .. "/x64/Release",
		}
