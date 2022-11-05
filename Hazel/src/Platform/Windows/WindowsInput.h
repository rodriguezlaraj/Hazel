#pragma once

#include "Hazel/Core/Input.h"

namespace Hazel {

	//Not exported to DLL
	class WindowsInput : public Input
	{
	protected:
        virtual bool IsKeyPressedImpl(KeyCode key) override;

        virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}