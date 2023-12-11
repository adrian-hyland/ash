#pragma once

#if defined __WIN32__
#include "ash.system.windows.concurrency.h"
#elif defined __linux__
#include "ash.system.linux.concurrency.h"
#endif


namespace Ash
{
	namespace Concurrency
	{
#if defined __WIN32__
		using Mutex = Ash::System::Windows::Concurrency::Mutex;
		using Event = Ash::System::Windows::Concurrency::Event;
		using Semaphore = Ash::System::Windows::Concurrency::Semaphore;
		template
		<
			typename RUNNABLE,
			bool     IS_DETACHABLE = false
		>
		using Thread = Ash::System::Windows::Concurrency::Thread<RUNNABLE, IS_DETACHABLE>;
#elif defined __linux__
		using Mutex = Ash::System::Linux::Concurrency::Mutex;
		using Event = Ash::System::Linux::Concurrency::Event;
		using Semaphore = Ash::System::Linux::Concurrency::Semaphore;
		template
		<
			typename RUNNABLE,
			bool     IS_DETACHABLE = false
		>
		using Thread = Ash::System::Linux::Concurrency::Thread<RUNNABLE, IS_DETACHABLE>;
#endif
	}
}
