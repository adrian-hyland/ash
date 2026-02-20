#pragma once

#include "ash.error.h"
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
				[[nodiscard]]
				virtual Ash::Error::Value onClose() { return Ash::Error::none; }

				[[nodiscard]]
				virtual Ash::Error::Value onResize(Ash::UI::Size size) { return Ash::Error::none; }

				[[nodiscard]]
				virtual Ash::Error::Value onMouse(Ash::UI::Position position, Ash::UI::Mouse::Button buttonState, Ash::UI::Mouse::Button buttonPressed) { return Ash::Error::none; }

				[[nodiscard]]
				virtual Ash::Error::Value onMouseWheel(Ash::UI::Position position, Ash::UI::Mouse::Wheel wheel) { return Ash::Error::none; }

				[[nodiscard]]
				virtual Ash::Error::Value onKey(Ash::UI::Keyboard::State keyState, Ash::UI::Keyboard::Key key) { return Ash::Error::none; }

				[[nodiscard]]
				virtual Ash::Error::Value onKeyCharacter(Ash::Unicode::Character character) { return Ash::Error::none; }
			};
		}
	}
}
