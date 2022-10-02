#pragma once
#include "Core.h"
#include "Events/Event.h"

#include "Window.h"

namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application(); //becuase this class will be inherited by the sandbox application
		void Run(); 
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

