#pragma once
#include "Core.h"


#include "Window.h"

#include "Hazel/LayerStack.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Hazel/Core/Timestep.h"

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel {
	class Application
	{
	public:
		Application();
		virtual ~Application(); //becuase this class will be inherited by the sandbox application
		void Run(); 

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);


		inline Window& GetWindow() { return *m_Window; }
		
		//There is only going to be one application and wherever we want to access the applicatio because it has important information
		inline static Application& Get() { return *s_Instance; } 
	private:
		bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseClick(MouseButtonPressedEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
        bool m_Minimized = false;
		LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
	private:
		//There is only going to be one application and wherever we want to access the applicatio because it has important information
		static Application* s_Instance;
		
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

