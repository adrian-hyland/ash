#pragma once

#if defined PLATFORM_WINDOWS
#include "ash.system.windows.filesystem.h"
#elif defined PLATFORM_LINUX
#include "ash.system.linux.filesystem.h"
#endif


namespace Ash
{
	namespace FileSystem
	{
#if defined PLATFORM_WINDOWS
		using Path = Ash::System::Windows::FileSystem::Path;
		using File = Ash::System::Windows::FileSystem::File;
#elif defined PLATFORM_LINUX
		using Path = Ash::System::Linux::FileSystem::Path;
		using File = Ash::System::Linux::FileSystem::File;
#endif
	}
}
