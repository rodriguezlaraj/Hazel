
//The client only needs to:
// 0 This means that the main only needs to exist in the engine instead of the game.
//   I am writing an application that uses the Hazel engine. How does that application start? 
// 1 include Hazel
// 2 create the sandbox class which inherits from application
// 3 write a function that returns the class

#include <hzpch.h>

#include <Hazel.h>

class Sandbox : public Hazel::Application 
{
public:
	Sandbox() 
	{
	
	}
	~Sandbox() 
	{
	
	}

};


Hazel::Application* Hazel::CreateApplication() 
{
	return new Sandbox(); //Returns an instance of Sandbox which is the application built
}


//1Pick a build system that you want to use CMAKE, Premake, etc
//2Make sure that each library that you use, that you write each in your build system
//3You can add them as a Git submodule to make sure you can update them as updates come up.
//Most of the times is a hybrid, a combination of things, there is no right way of doing it.