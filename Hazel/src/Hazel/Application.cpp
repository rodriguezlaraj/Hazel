#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel {

	float red = 0;

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	//Applicatoin as a singleton
	Application* Application::s_Instance = nullptr;


	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!"); //To make sure we can only have one application
		
		s_Instance = this;//Application as a singleton.

		m_Window = std::unique_ptr<Window>(Window::Create()); //Calls with default parameters
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	//There is only one function because we really want to make the window layer agnostic as to what the application needs.
	//If we had to define a function for each type of event, then the Application and the Window layer are not really decoupled.
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseClick));

		//HZ_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break; //Ensures that an event is handled only one time. The vector is traversed in reverse to handle the events that are closer to the user.
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(red, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)  //Can use directly for loop because begin and end are implemented in the LayerStack class
			{
				layer->OnUpdate();
			}

			auto [x, y] = Input::GetMousePosition();
			HZ_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnMouseClick(MouseButtonPressedEvent& e)
	{
		if (red >= 1.0) 
		{
			red = 0.0;
		}
		else 
		{
			red += 0.1;
		}

		return true;
	}

}
