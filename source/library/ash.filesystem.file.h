#pragma once


#if defined __WIN32__

#include "ash.system.windows.filesystem.h"

namespace Ash
{
	namespace FileSystem = System::Windows::FileSystem;
}

#elif defined __linux__

#include "ash.system.linux.filesystem.h"

namespace Ash
{
	namespace FileSystem = System::Linux::FileSystem;
}

#endif
