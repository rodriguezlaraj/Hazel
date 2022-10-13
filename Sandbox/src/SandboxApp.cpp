
//The client only needs to:
// 0 This means that the main only needs to exist in the engine instead of the game.
//   I am writing an application that uses the Hazel engine. How does that application start? 
// 1 include Hazel
// 2 create the sandbox class which inherits from application
// 3 write a function that returns the class

#include <hzpch.h>

#include <Hazel.h>


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
		PushOverlay(new Hazel::ImGuiLayer());
	
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