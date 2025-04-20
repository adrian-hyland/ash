#pragma once

#if defined PLATFORM_WINDOWS
#include "ash.system.windows.ui.screen.h"
#elif defined PLATFORM_LINUX
#include "ash.system.linux.x11.screen.h"
#endif


namespace Ash
{
	namespace UI
	{
#if defined PLATFORM_WINDOWS
		using Screen = Ash::System::Windows::UI::Screen;
#elif defined PLATFORM_LINUX
		using Screen = Ash::System::Linux::X11::Screen;
#endif
	}
}
