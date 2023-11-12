#pragma once

#include <windows.h>
#include "ash.system.windows.timer.h"


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

				
				template
				<
					typename RUNNABLE,
					bool     IS_DETACHABLE = false
				>
				class Thread
				{
				public:
					using Handle = HANDLE;

					using Runnable = RUNNABLE;

					template
					<
						typename ...ARGS
					>
					inline Thread(ARGS ...args) : m_Runnable(std::forward<ARGS>(args)...), m_Handle(INVALID_HANDLE_VALUE) {}

					inline Thread(Thread &&thread) noexcept : m_Runnable(std::move(thread.m_Runnable)), m_Handle(thread.m_Handle)
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

							m_Runnable = std::move(thread.m_Runnable);
							m_Handle = thread.m_Handle;

							thread.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					inline bool start()
					{
						if (!join())
						{
							return false;
						}

						m_Handle = (Handle)::_beginthreadex(nullptr, 0, run, &m_Runnable, 0, nullptr);

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
						typename ...ARGS
					>
					static inline bool start(ARGS ...args)
					{
						return Thread<Runnable, true>(std::forward<ARGS>(args)...).start();
					}

				protected:
					static __stdcall unsigned int run(void *param)
					{
						static_cast<Runnable *>(param)->run();

						::_endthreadex(0);

						return 0;
					}

				private:
					Runnable m_Runnable;
					Handle   m_Handle;

					Thread(const Thread &thread) = delete;
					Thread &operator = (const Thread &thread) = delete;
				};


				template
				<
					typename RUNNABLE
				>
				class Thread<RUNNABLE, true>
				{
				public:
					using Handle = HANDLE;

					using Runnable = RUNNABLE;

					template
					<
						typename ...ARGS
					>
					inline Thread(ARGS ...args) : m_Handle(INVALID_HANDLE_VALUE), m_DetachEvent(), m_Detachable(&m_DetachEvent, std::forward<ARGS>(args)...) {}

					inline Thread(Thread &&thread) noexcept : m_Handle(thread.m_Handle), m_DetachEvent(std::move(thread.m_DetachEvent)), m_Detachable(std::move(thread.m_Detachable))
					{
						thread.m_Handle = INVALID_HANDLE_VALUE;
					}

					inline Thread &operator = (Thread &&thread) noexcept
					{
						if (this != &thread)
						{
							m_Handle = thread.m_Handle;
							m_DetachEvent = std::move(thread.m_DetachEvent);
							m_Detachable = std::move(thread.m_Detachable);

							thread.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					inline bool start()
					{
						if (!m_DetachEvent.reset())
						{
							return false;
						}
						
						m_Handle = (Handle)_beginthreadex(nullptr, 0, run, this, 0, nullptr);

						return (m_Handle != INVALID_HANDLE_VALUE) && m_DetachEvent.wait();
					}

				protected:
					class Detachable : public Runnable
					{
					public:
						template
						<
							typename ...ARGS
						>
						inline Detachable(Event *event, ARGS ...args) : Runnable(std::forward<ARGS>(args)...), m_Event(event) {}

						inline Detachable(Detachable &&detachable) noexcept : Runnable(std::move(detachable)), m_Event(std::move(detachable.m_Event)) {}

						inline Detachable &operator = (Detachable &&detachable) noexcept
						{
							if (this != detachable)
							{
								Runnable::operator = (std::move(detachable));
								m_Event = std::move(detachable.m_Event);
							}

							return *this;
						}

						inline bool detach()
						{
							return m_Event->signal();
						}

					private:
						Event *m_Event;

						Detachable(const Detachable &detachable) = delete;
						Detachable &operator = (const Detachable &detachable) = delete;
					};

					static __stdcall unsigned int run(void *param)
					{
						Thread *thread = static_cast<Thread *>(param);
						Handle handle = thread->m_Handle;
						thread->m_Handle = INVALID_HANDLE_VALUE;
						Detachable runnable = std::move(thread->m_Detachable);

						if (runnable.detach())
						{
							runnable.run();
						}

						::_endthreadex(0);
						::CloseHandle(handle);

						return 0;
					}

				private:
					Handle     m_Handle;
					Event      m_DetachEvent;
					Detachable m_Detachable;

					Thread(const Thread &thread) = delete;
					Thread &operator = (const Thread &thread) = delete;
				};
			}
		}
	}
}
