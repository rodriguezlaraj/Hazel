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
IncludeDir["glm"] = "Hazel/vendor/glm"
IncludeDir["stb_image"] = "Hazel/vendor/stb_image"


group "Dependencies"
	include "Hazel/vendor/GLFW"--This is effectively including the premake from the GLFW directory which is basically a new project
	include "Hazel/vendor/Glad" --This is effectively including the premake from the Glad directory which is basically a new project
	include "Hazel/vendor/imgui" --This is effectively including the premake from the ImGui directory which is basically a new project

group ""



project "Hazel"   --project
	location "Hazel"
	kind "StaticLib" --Static Library
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	--If sandbox turns out to be big, this can be done for Sandbox as well
	pchheader "hzpch.h" --sets up the right to the project and specifying that precompiled headers will be used
	pchsource "Hazel/src/hzpch.cpp" --sets up right click to this file and configuring the precompiled header
	--every h, .cpp file that is in the source directory should be included here
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}", 
		"%{IncludeDir.Glad}" ,
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}


	links
	{
		"GLFW",          --Then this library[The project linked above] is linked into our DLL (i.e. Hazel)
		"Glad",          --Then this library[The project linked above] is linked into our DLL (i.e. Hazel)
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
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
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

	--The above are not nested filters. In order to select filters simultaneously we can use the option below
	--filters { "system:windows", "configurations:Release"}
		--buildoptions "/MT"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Hazel/src",
		"Hazel/vendor",
		"%{IncludeDir.glm}"
	}
	
	links
	{
	    "Hazel"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"
