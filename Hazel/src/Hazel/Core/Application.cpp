#include "hzpch.h"
#include "Application.h"

#include "Hazel/Core/Log.h"

#include "Hazel/Renderer/Renderer.h"

#include "Input.h"

#include <glfw/glfw3.h>

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

        Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

        
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
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

	

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
            //TODO: This can use QueryPerformanceCounter from windows. //This should be per platform because each platform gives time. We should not call glfw here.
            float time = (float)glfwGetTime(); 
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;


            //Only update if the window is not minimized
            if (!m_Minimized)
            {
                //I assume that if the virtual method is not implemented by the specific layer below, then the function is just not called.
                //Go through all the layers OnUpdate
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }


			//Takes care of ImGui Rendering
			m_ImGuiLayer->Begin(); //It only requires 1 begin

			//Go through all the layers for OnImGuiRender
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender(); //Each layer can have its own Rendering.

			m_ImGuiLayer->End();  //It only requires 1 end.


			//auto [x, y] = Input::GetMousePosition();
			//static float xx = 0;
			//static float yy = 0;
			//if( (xx != x) || (yy != y))
			//{
			//	HZ_CORE_TRACE("{0}, {1}", x, y);
			//	yy = y;
			//	xx = x;
			//}
			

			m_Window->OnUpdate();
		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
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
