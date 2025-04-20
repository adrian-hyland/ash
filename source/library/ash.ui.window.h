#pragma once

#if defined PLATFORM_WINDOWS
#include "ash.system.windows.ui.window.h"
#elif defined PLATFORM_LINUX
#include "ash.system.linux.x11.window.h"
#endif


namespace Ash
{
	namespace UI
	{
#if defined PLATFORM_WINDOWS
		using Window = Ash::System::Windows::UI::Window;
#elif defined PLATFORM_LINUX
		using Window = Ash::System::Linux::X11::Window;
#endif
	}
}
