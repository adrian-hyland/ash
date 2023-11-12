#pragma once


#if defined __WIN32__

#include "ash.system.windows.concurrency.h"

namespace Ash
{
	namespace Concurrency = System::Windows::Concurrency;
}

#elif defined __linux__

#include "ash.system.linux.concurrency.h"

namespace Ash
{
	namespace Concurrency = System::Linux::Concurrency;
}

#endif