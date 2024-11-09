#pragma once

#if defined __WIN32__
#include "ash.system.windows.filesystem.h"
#elif defined __linux__
#include "ash.system.linux.filesystem.h"
#endif


namespace Ash
{
	namespace FileSystem
	{
#if defined __WIN32__
		using Path = Ash::System::Windows::FileSystem::Path;
		using File = Ash::System::Windows::FileSystem::File;
#elif defined __linux__
		using Path = Ash::System::Linux::FileSystem::Path;
		using File = Ash::System::Linux::FileSystem::File;
#endif
	}
}
