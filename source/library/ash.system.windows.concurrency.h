#pragma once

#include <limits>
#include <windows.h>
#include <process.h>
#include "ash.callable.h"
#include "ash.timer.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace Concurrency
			{
				class Condition
				{
				public:
					inline Condition() : m_Condition(CONDITION_VARIABLE_INIT), m_Lock(SRWLOCK_INIT) {}

					inline void acquire()
					{
						::AcquireSRWLockExclusive(&m_Lock);
					}

					inline void release()
					{
						::ReleaseSRWLockExclusive(&m_Lock);
					}

					inline void signal()
					{
						::WakeConditionVariable(&m_Condition);
					}

					template
					<
						typename PREDICATE
					>
					inline void wait(PREDICATE predicate)
					{
						while (!predicate())
						{
							::SleepConditionVariableSRW(&m_Condition, &m_Lock, INFINITE, 0);
						}
					}

					template
					<
						typename PREDICATE
					>
					inline bool tryWait(PREDICATE predicate, Ash::Timer::Value duration)
					{
						Ash::Timer::Value now = Ash::Timer::getClockValue(Ash::Timer::Clock::LowResolution);

						while (!predicate())
						{
							Ash::Timer::Value elapsed = Ash::Timer::getClockValue(Ash::Timer::Clock::LowResolution) - now;
							if (elapsed > duration)
							{
								return false;
							}
							Ash::Timer::Value remaining = duration - elapsed;

							if (!::SleepConditionVariableSRW(&m_Condition, &m_Lock, remaining.as<DWORD>(Ash::Timer::Value::Milliseconds), 0))
							{
								return false;
							}
						}

						return true;
					}

				private:
					CONDITION_VARIABLE m_Condition;
					SRWLOCK            m_Lock;

					Condition(const Condition &condition) = delete;
					Condition(Condition &&condition) = delete;
					Condition &operator = (const Condition &condition) = delete;
					Condition &operator = (Condition &&condition) = delete;
				};

				class Mutex : public Condition
				{
				public:
					inline Mutex() : Condition(), m_IsLocked(false) {}

					inline bool tryAcquire()
					{
						Condition::acquire();

						bool isLocked = m_IsLocked;
						m_IsLocked = true;

						Condition::release();
						
						return !isLocked;
					}

					inline bool tryAcquire(Ash::Timer::Value duration)
					{
						Condition::acquire();

						if (!Condition::tryWait([this]() { return !m_IsLocked; }, duration))
						{
							Condition::release();
							return false;
						}

						m_IsLocked = true;

						Condition::release();
						return true;
					}

					inline void acquire()
					{
						Condition::acquire();

						Condition::wait([this]() { return !m_IsLocked; });

						m_IsLocked = true;

						Condition::release();
					}

					inline void release()
					{
						Condition::acquire();

						m_IsLocked = false;

						Condition::release();
						Condition::signal();
					}

				private:
					bool m_IsLocked;

					Mutex(const Mutex &mutex) = delete;
					Mutex(Mutex &&mutex) = delete;
					Mutex &operator = (const Mutex &mutex) = delete;
					Mutex &operator = (Mutex &&mutex) = delete;
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

					inline ~Thread()
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
					struct Function
					{
						Ash::Callable::Function<FUNCTION, ARGUMENTS...> function;
					};

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static inline Handle runFunction(FUNCTION function, ARGUMENTS &&...arguments)
					{
						using Callable = Function<FUNCTION, ARGUMENTS...>;

						Callable *callable = new Callable(Ash::Callable::Function<FUNCTION, ARGUMENTS...>(function, std::forward<ARGUMENTS>(arguments)...));

						uintptr_t handle = ::_beginthreadex(nullptr, 0, runCallable<Callable>, callable, 0, nullptr);

						if (handle == 0)
						{
							delete callable;
							return INVALID_HANDLE_VALUE;
						}

						return Handle(handle);
					}

					template
					<
						typename CALLABLE
					>
					static unsigned int __stdcall runCallable(void *param)
					{
						CALLABLE *callable = static_cast<CALLABLE *>(param);
						
						callable->function();

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
