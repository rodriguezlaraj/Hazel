#pragma once



#ifdef HZ_PLATFORM_WINDOWS
	//If we are on windows, we will be creating a DLL from Hazel project 
	//This DLL will be then used by our application (Sandbox).
	//Core gets included in both project through the Hazel.h file.
	//In this case, this file functions as the header for the definition and export of the classes
	// and also as the header for the import of the clasess depending on where it is called.
	#ifdef  HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif //#ifdef  HZ_BUILD_DLL
#else
	#error Hazel only support Windows!
#endif//#ifdef HZ_PLATFORM_WINDOWS


#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)
