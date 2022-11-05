#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {
	

    class ImGuiLayer : public Layer
	{
	public:
		

		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		//ImGuiLayer does not need OnEvent because the events get directly to it from GLFW throught the window
		//and are handled internally.

		//Layer need to be able to do their own ImGui rendering
		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}