#pragma once

#include <limits>
#include <windows.h>
#include <process.h>


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
						typename RUNNABLE,
						typename ...ARGS
					>
					inline bool run(ARGS ...args)
					{
						if (!join())
						{
							return false;
						}

						m_Handle = startRunnable<RUNNABLE>(std::forward<ARGS>(args)...);

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
						typename RUNNABLE,
						typename ...ARGS
					>
					static inline bool runDetached(ARGS ...args)
					{
						Handle handle = startRunnable<RUNNABLE>(std::forward<ARGS>(args)...);
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
						typename RUNNABLE
					>
					class ThreadRunnable : public RUNNABLE
					{
					public:
						template
						<
							typename ...ARGS
						>
						ThreadRunnable(ARGS ...args) : RUNNABLE(args...), m_Event() {}

						inline bool signal() { return m_Event.signal(); }

						inline bool wait() { return m_Event.wait(); }

					private:
						Event m_Event;
					};

					template
					<
						typename RUNNABLE,
						typename ...ARGS
					>
					static inline Handle startRunnable(ARGS ...args)
					{
						ThreadRunnable<RUNNABLE> threadRunnable(std::forward<ARGS>(args)...);

						uintptr_t handle = ::_beginthreadex(nullptr, 0, startThreadRunnable<RUNNABLE>, &threadRunnable, 0, nullptr);
						if (handle == 0)
						{
							return INVALID_HANDLE_VALUE;
						}

						threadRunnable.wait();
						return Handle(handle);
					}

					template
					<
						typename RUNNABLE
					>
					static __stdcall unsigned int startThreadRunnable(void *param)
					{
						ThreadRunnable<RUNNABLE> *threadRunnable = static_cast<ThreadRunnable<RUNNABLE> *>(param);
						RUNNABLE runnable = std::move(*threadRunnable);
						
						if (threadRunnable->signal())
						{
							runnable.run();
						}

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
