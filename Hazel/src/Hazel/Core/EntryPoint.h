#pragma once
#include "Hazel/Core/Core.h"

#ifdef HZ_PLATFORM_WINDOWS

//Something that returns a hazel applicatoin pointer
//A function that we will define somewhere else that will return that application for us.
//This CreateApplication is defined in the client.
//It is defined somewhere else becuase it is called extern
extern Hazel::Application* Hazel::CreateApplication();


//Wwe want this to be win main on windows because we want to retain some information about 
//how the program was run 
int main(int argc, char** argv)
{
	Hazel::Log::Init();

    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	auto app = Hazel::CreateApplication();
    HZ_PROFILE_END_SESSION();


    HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	app->Run();
    HZ_PROFILE_END_SESSION();
	
    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Shutdown.json");
	delete app;
    HZ_PROFILE_END_SESSION();
}

#else
#error Hazel only supports Windows!
#endif//#ifdef HZ_PLATFORM_WINDOWS
