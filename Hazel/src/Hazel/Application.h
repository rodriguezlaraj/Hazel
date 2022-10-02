#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Hazel {
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application(); //becuase this class will be inherited by the sandbox application
		void Run(); 
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

