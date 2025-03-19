#pragma once

#include "ash.ui.core.h"
#include "ash.ui.generic.h"
#include "ash.ui.keyboard.h"
#include "ash.ui.mouse.h"

#if defined __WIN32__
#include "ash.system.windows.ui.screen.h"
#include "ash.system.windows.ui.window.h"
#elif defined __linux__
#include "ash.system.linux.x11.screen.h"
#include "ash.system.linux.x11.window.h"
#endif


namespace Ash
{
	namespace UI
	{
#if defined __WIN32__
		using Screen = Ash::System::Windows::UI::Screen;
		using Window = Ash::System::Windows::UI::Window;
#elif defined __linux__
		using Screen = Ash::System::Linux::X11::Screen;
		using Window = Ash::System::Linux::X11::Window;
#endif
	}
}
