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

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
			glClearColor(red, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			//I assume that if the virtual method is not implemented by the specific layer below, then the function is just not called.

			//Go through all the layers OnUpdate
			for (Layer* layer : m_LayerStack)  //Can use directly for loop because begin and end are implemented in the LayerStack class
			{
				layer->OnUpdate();
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
