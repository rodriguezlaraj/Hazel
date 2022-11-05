#include "hzpch.h"
#include "Hazel/Core/Application.h"

#include "Hazel/Core/Log.h"

#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Core/Input.h"

#include <glfw/glfw3.h>

namespace Hazel {

	float red = 0;
	
	//Applicatoin as a singleton
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
        HZ_PROFILE_FUNCTION();
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!"); //To make sure we can only have one application
		
		s_Instance = this;//Application as a singleton.

        m_Window = Window::Create();
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

        
	}

	Application::~Application()
	{
        HZ_PROFILE_FUNCTION();
        Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
        HZ_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
        HZ_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	//There is only one function because we really want to make the window layer agnostic as to what the application needs.
	//If we had to define a function for each type of event, then the Application and the Window layer are not really decoupled.
	void Application::OnEvent(Event& e)
	{
        HZ_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(Application::OnMouseClick));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

	

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
            (*it)->OnEvent(e);
			if (e.Handled)
				break; //Ensures that an event is handled only one time. The vector is traversed in reverse to handle the events that are closer to the user.
		}
	}

	void Application::Run()
	{
        HZ_PROFILE_FUNCTION();
		while (m_Running)
		{
            HZ_PROFILE_SCOPE("RunLoop");
            //TODO: This can use QueryPerformanceCounter from windows. //This should be per platform because each platform gives time. We should not call glfw here.
            float time = (float)glfwGetTime(); 
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;


            //Only update if the window is not minimized
            if (!m_Minimized)
            {
                {
                    HZ_PROFILE_SCOPE("RunLoop");
                    //I assume that if the virtual method is not implemented by the specific layer below, then the function is just not called.
                    //Go through all the layers OnUpdate
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(timestep);
                }

                //Takes care of ImGui Rendering
                m_ImGuiLayer->Begin(); //It only requires 1 begin
                {
                    HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");
                    //Go through all the layers for OnImGuiRender
                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender(); //Each layer can have its own Rendering.

                    m_ImGuiLayer->End();  //It only requires 1 end.
                }
            }


			



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
        HZ_PROFILE_FUNCTION();
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
       
		if (red >= 1.0f) 
		{
			red = 0.0f;
		}
		else 
		{
			red += 0.1f;
		}

		return true;
	}

}
