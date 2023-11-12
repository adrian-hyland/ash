#pragma once


#if defined __WIN32__

#include "ash.system.windows.timer.h"

namespace Ash
{
	using Timer = System::Windows::Timer;
}

#elif defined __linux__

#include "ash.system.linux.timer.h"

namespace Ash
{
	using Timer = System::Linux::Timer;
}

#endif
