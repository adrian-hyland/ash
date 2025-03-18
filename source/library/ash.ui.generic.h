#pragma once

#include "ash.ui.core.h"
#include "ash.ui.keyboard.h"
#include "ash.ui.mouse.h"


namespace Ash
{
	namespace UI
	{
		namespace Generic
		{
			class Window
			{
			public:
				virtual bool onClose() { return true; }

				virtual bool onResize(Ash::UI::Size size) { return true; }

				virtual bool onMouse(Ash::UI::Position position, Ash::UI::Mouse::Button buttonState, Ash::UI::Mouse::Button buttonPressed) { return true; }

				virtual bool onMouseWheel(Ash::UI::Position position, Ash::UI::Mouse::Wheel wheel) { return true; }

				virtual bool onKey(Ash::UI::Keyboard::State keyState, Ash::UI::Keyboard::Key key) { return true; }

				virtual bool onKeyCharacter(Ash::Unicode::Character character) { return true; }
			};
		}
	}
}
