#pragma once

#include <limits>
#include <windows.h>
#include <process.h>
#include "ash.callable.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace Concurrency
			{
				class Mutex
				{
				public:
					using Handle = HANDLE;

					inline Mutex() : m_Handle(INVALID_HANDLE_VALUE)
					{
						create();
					}

					inline Mutex(Mutex &&mutex) noexcept : m_Handle(mutex.m_Handle)
					{
						mutex.m_Handle = INVALID_HANDLE_VALUE; 
					}

					virtual inline ~Mutex()
					{
						destroy();
					}

					inline Mutex &operator = (Mutex &&mutex) noexcept
					{
						if (this != &mutex)
						{
							destroy();

							m_Handle = mutex.m_Handle;

							mutex.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					inline bool isSetUp() { return m_Handle != INVALID_HANDLE_VALUE; } 

					inline operator Handle *() { return (m_Handle != INVALID_HANDLE_VALUE) ? &m_Handle : nullptr; }

					inline bool acquire()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && (::WaitForSingleObject(m_Handle, INFINITE) == WAIT_OBJECT_0);
					}

					inline bool release()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && (::ReleaseMutex(m_Handle) == 0);
					}

				protected:
					inline void create()
					{
						if (m_Handle == INVALID_HANDLE_VALUE)
						{
							m_Handle = ::CreateMutexA(nullptr, false, nullptr);
						}
					}

					inline void destroy()
					{
						if (m_Handle != INVALID_HANDLE_VALUE)
						{
							::CloseHandle(m_Handle);
							m_Handle = INVALID_HANDLE_VALUE;
						}
					}

				private:
					Handle m_Handle;
				};


				class Event
				{
				public:
					using Handle = HANDLE;

					enum Reset
					{
						Manual,
						Automatic
					};

					inline Event(Reset reset = Reset::Manual, bool value = false) : m_Handle(INVALID_HANDLE_VALUE)
					{
						create(reset, value);
					}

					inline Event(Event &&event) noexcept : m_Handle(event.m_Handle)
					{
						event.m_Handle = INVALID_HANDLE_VALUE;
					}

					virtual inline ~Event()
					{
						destroy();
					}

					inline Event &operator = (Event &&event) noexcept
					{
						if (this != &event)
						{
							destroy();

							m_Handle = event.m_Handle;

							event.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					inline bool isSetUp() const { return m_Handle != INVALID_HANDLE_VALUE; }

					inline operator Handle *() { return (m_Handle != INVALID_HANDLE_VALUE) ? &m_Handle : nullptr; }

					inline bool reset()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && ::ResetEvent(m_Handle);
					}

					inline bool signal()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && ::SetEvent(m_Handle);
					}

					inline bool tryWait()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && (::WaitForSingleObject(m_Handle, 0) == WAIT_OBJECT_0);
					}

					inline bool wait()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && (::WaitForSingleObject(m_Handle, INFINITE) == WAIT_OBJECT_0);
					}

				protected:
					inline void create(Reset reset, bool value)
					{
						if (m_Handle == INVALID_HANDLE_VALUE)
						{
							m_Handle = ::CreateEventA(nullptr, reset == Reset::Manual, value, nullptr);
						}
					}

					inline void destroy()
					{
						if (m_Handle != INVALID_HANDLE_VALUE)
						{
							::CloseHandle(m_Handle);
							m_Handle = INVALID_HANDLE_VALUE;
						}
					}

				private:
					Handle m_Handle;
				};


				class Semaphore
				{
				public:
					using Handle = HANDLE;

					using Count = LONG;

					inline Semaphore(Count count = 0) : m_Handle(INVALID_HANDLE_VALUE)
					{
						create(count);
					}

					inline Semaphore(Semaphore &&semaphore) noexcept : m_Handle(semaphore.m_Handle)
					{
						semaphore.m_Handle = INVALID_HANDLE_VALUE; 
					}

					virtual inline ~Semaphore()
					{
						destroy();
					}

					inline Semaphore &operator = (Semaphore &&semaphore) noexcept
					{
						if (this != &semaphore)
						{
							destroy();

							m_Handle = semaphore.m_Handle;

							semaphore.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					inline bool isSetUp() { return m_Handle != INVALID_HANDLE_VALUE; } 

					inline operator Handle *() { return (m_Handle != INVALID_HANDLE_VALUE) ? &m_Handle : nullptr; }

					inline bool acquire()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && (::WaitForSingleObject(m_Handle, INFINITE) == WAIT_OBJECT_0);
					}

					inline bool release()
					{
						return (m_Handle != INVALID_HANDLE_VALUE) && (::ReleaseSemaphore(m_Handle, 1, nullptr) == 0);
					}

				protected:
					inline void create(Count count)
					{
						if (m_Handle == INVALID_HANDLE_VALUE)
						{
							m_Handle = ::CreateSemaphoreA(nullptr, count, std::numeric_limits<Count>::max(), nullptr);
						}
					}

					inline void destroy()
					{
						if (m_Handle != INVALID_HANDLE_VALUE)
						{
							::CloseHandle(m_Handle);
							m_Handle = INVALID_HANDLE_VALUE;
						}
					}

				private:
					Handle m_Handle;
				};
				

				class Thread
				{
				public:
					using Handle = HANDLE;

					inline Thread() : m_Handle(INVALID_HANDLE_VALUE) {}

					inline Thread(Thread &&thread) noexcept : m_Handle(thread.m_Handle)
					{
						thread.m_Handle = INVALID_HANDLE_VALUE;
					}

					virtual inline ~Thread()
					{
						join();
					}

					inline Thread &operator = (Thread &&thread) noexcept
					{
						if (this != &thread)
						{
							join();

							m_Handle = thread.m_Handle;

							thread.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					inline bool run(FUNCTION function, ARGUMENTS &&...arguments)
					{
						if (m_Handle != INVALID_HANDLE_VALUE)
						{
							return false;
						}

						m_Handle = runFunction(function, std::forward<ARGUMENTS>(arguments)...);

						return m_Handle != INVALID_HANDLE_VALUE;
					}

					inline bool join()
					{
						if ((m_Handle != INVALID_HANDLE_VALUE) && (WaitForSingleObject(m_Handle, INFINITE) == WAIT_OBJECT_0))
						{
							::CloseHandle(m_Handle);
							m_Handle = INVALID_HANDLE_VALUE;
						}

						return m_Handle == INVALID_HANDLE_VALUE;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static bool runDetached(FUNCTION function, ARGUMENTS &&...arguments)
					{
						Handle handle = runFunction(function, std::forward<ARGUMENTS>(arguments)...);
						if (handle != INVALID_HANDLE_VALUE)
						{
							::CloseHandle(handle);
							return true;
						}
						return false;
					}

				protected:
					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static inline Handle runFunction(FUNCTION function, ARGUMENTS &&...arguments)
					{
						using CallableFunction = Ash::Callable::Function<FUNCTION, ARGUMENTS...>;

						CallableFunction *callable = new CallableFunction(function, std::forward<ARGUMENTS>(arguments)...);

						uintptr_t handle = ::_beginthreadex(nullptr, 0, runCallable<CallableFunction>, callable, 0, nullptr);

						return (handle != 0) ? Handle(handle) : INVALID_HANDLE_VALUE;
					}

					template
					<
						typename CALLABLE
					>
					static __stdcall unsigned int runCallable(void *param)
					{
						CALLABLE *callable = static_cast<CALLABLE *>(param);
						
						(*callable)();

						delete callable;

						::_endthreadex(0);
						return 0;
					}

				private:
					Handle m_Handle;

					Thread(const Thread &thread) = delete;
					Thread &operator = (const Thread &thread) = delete;
				};
			}
		}
	}
}
