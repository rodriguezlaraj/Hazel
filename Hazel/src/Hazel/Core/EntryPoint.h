#pragma once

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
	HZ_CORE_WARN("Initialized Log!");
	int a = 5;
	HZ_INFO("Hello Log! Var={0}", a);
	auto app = Hazel::CreateApplication();
	app->Run();
	
	delete app;
}

#else
#error Hazel only supports Windows!
#endif//#ifdef HZ_PLATFORM_WINDOWS
