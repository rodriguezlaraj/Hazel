
//The client only needs to:
// 0 This means that the main only needs to exist in the engine instead of the game.
//   I am writing an application that uses the Hazel engine. How does that application start? 
// 1 include Hazel
// 2 create the sandbox class which inherits from application
// 3 write a function that returns the class

#include <hzpch.h>

#include <Hazel.h>

#include "imgui/imgui.h"


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	
	}

	void OnUpdate() override
	{
		//HZ_INFO("ExampleLayer::Update");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		//Calling from the Sandbox was a problem because of Hazel's dynamic linking.
		//Hazel's DLL only imports the functions that are actually used from ImGui. 
		//Because these functions are not used, they are not part of the DLL.
		//We could create a .def file to make sure all functions in ImGui are exported. 
		//This is laborious and hard to mantain. It was for this reason that Hazel became static library.
		//ImGui::Begin("Test");
		ImGui::Text("Hello World From the Actual Game!");
		static bool showWindow2 = true;
		ImGui::ShowAboutWindow(&showWindow2);
		//ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		//HZ_TRACE("{0}", event);
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("Tab key is pressed (event)!");
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}

	}

};

class Sandbox : public Hazel::Application 
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new Hazel::ImGuiLayer());
	
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