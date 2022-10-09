workspace "Hazel"     --name of solution
	architecture "x64"   --platform

	configurations
	{
		"Debug",   --use to develop
		"Release", --like a faster debug
		"Dist"     --final distribution
	}





outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"

include "Hazel/vendor/GLFW" --This is effectively including the premake from the GLFW directory which is basically a new project

project "Hazel"   --project
	location "Hazel"
	kind "SharedLib" --Dynamic Library
	language "C++"

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
		"%{IncludeDir.GLFW}" 
	}


	links
	{
		"GLFW",          --Then this library[The project linked above] is linked into our DLL (i.e. Hazel)
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")

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
		buildoptions "/MDd"
		symbols "On"
		--staticruntime "off"
		--runtime "Debug"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		buildoptions "/MD"
		optimize "On"
		--staticruntime "off"
		--runtime "Release"

	filter "configurations:Dist"
		defines "HZ_DIST"
		buildoptions "/MD"
		optimize "On"
		--staticruntime "off"
		--runtime "Release"

	--The above are not nested filters. In order to select filters simultaneously we can use the option below
	--filters { "system:windows", "configurations:Release"}
		--buildoptions "/MT"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		staticruntime "On"
		systemversion "latest"

		defines{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		buildoptions "/MD"
		optimize "On"