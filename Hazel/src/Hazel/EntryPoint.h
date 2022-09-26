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
	
	auto app = Hazel::CreateApplication();
	app->Run();
	//Sandbox* sandbox = new Sandbox(); //Allocated on the heap. This can be very large and we want to control the lifetime of this object explicitely
	delete app;
}

#else
#error Hazel only support Windows!
#endif//#ifdef HZ_PLATFORM_WINDOWS
