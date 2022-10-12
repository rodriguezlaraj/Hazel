workspace "Hazel"     --name of solution
	architecture "x64"   --platform

	configurations
	{
		"Debug",   --use to develop
		"Release", --like a faster debug
		"Dist"     --final distribution
	}

startproject "Sandbox"



outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"


group "Dependencies"
	include "Hazel/vendor/GLFW"--This is effectively including the premake from the GLFW directory which is basically a new project
	include "Hazel/vendor/Glad" --This is effectively including the premake from the Glad directory which is basically a new project
	include "Hazel/vendor/imgui" --This is effectively including the premake from the ImGui directory which is basically a new project

group ""



project "Hazel"   --project
	location "Hazel"
	kind "SharedLib" --Dynamic Library
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	--If sandbox turns out to be big, this can be done for Sandbox as well
	pchheader "hzpch.h" --sets up the right to the project and specifying that precompiled headers will be used
	pchsource "Hazel/src/hzpch.cpp" --sets up right click to this file and configuring the precompiled header
	--every h, .cpp file that is in the source directory should be included here
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}", 
		"%{IncludeDir.Glad}" ,
		"%{IncludeDir.ImGui}" 
	}


	links
	{
		"GLFW",          --Then this library[The project linked above] is linked into our DLL (i.e. Hazel)
		"Glad",          --Then this library[The project linked above] is linked into our DLL (i.e. Hazel)
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			--("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") 
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"") --commit 7bb1d5077fe719ad0097ef7e6788af0d9f434680

		}

	
--If anyone is having issues with unresolved externals related to glfw3 when building, 
--I found the issue to be in properties -> C/C++ -> Code Generation -> Runtime library. 
-- Switching it from Multi-threaded debug (/MTd) to Multithreaded DLL(/MD) or Multithreaded 
--Debug DLL (/MDd) did the trick.  
--To make the premake file do what you want, they keywords are staticruntime and runtime.  
--for /MD you want staticruntime "off" and runtime "Release", and for /MDd you want staticruntime 
--"off" and runtime "Debug".  You may also need to get rid of any buildoptions that override this 
--(I had buildoptions "/MT" which overrode /MD).
--/MD   Multithreaded DLL
--staticruntime "off"
--runtime "Release"

---MDd  Multithreaded Debug DLL
--staticruntime "off"
--runtime "Debug"
	
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		--buildoptions "/MDd"
		runtime "Debug"
		symbols "On"


	filter "configurations:Release"
		defines "HZ_RELEASE"
		--buildoptions "/MD"
		runtime "Release"
		optimize "On"


	filter "configurations:Dist"
		defines "HZ_DIST"
		--buildoptions "/MD"
		runtime "Release"
		optimize "On"

	--The above are not nested filters. In order to select filters simultaneously we can use the option below
	--filters { "system:windows", "configurations:Release"}
		--buildoptions "/MT"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src"
	}
	
	links
	{
	    "Hazel"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"