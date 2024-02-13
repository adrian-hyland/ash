#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <utility>
#include "ash.callable.h"
#include "ash.timer.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace Concurrency
			{
				class Condition;

				class Mutex
				{
				public:
					inline Mutex() : m_Handle(PTHREAD_MUTEX_INITIALIZER) {}

					inline ~Mutex()
					{
						::pthread_mutex_destroy(&m_Handle);
					}

					inline bool tryAcquire()
					{
						return ::pthread_mutex_trylock(&m_Handle) == 0;
					}

					inline bool tryAcquire(Ash::Timer::Value duration)
					{
						duration = duration + Ash::Timer::getClockValue(Ash::Timer::Clock::System);

						timespec timeout = duration.as<timespec>();

						return ::pthread_mutex_timedlock(&m_Handle, &timeout) == 0;
					}

					inline void acquire()
					{
						::pthread_mutex_lock(&m_Handle);
					}

					inline void release()
					{
						::pthread_mutex_unlock(&m_Handle);
					}

				private:
					pthread_mutex_t m_Handle;

					Mutex(const Mutex &mutex) = delete;
					Mutex(Mutex &&mutex) = delete;
					Mutex &operator = (const Mutex &mutex) = delete;
					Mutex &operator = (Mutex &&mutex) = delete;

					friend Condition;
				};


				class Condition
				{
				public:
					inline Condition() : m_Handle(PTHREAD_COND_INITIALIZER), m_Mutex() {}

					inline ~Condition()
					{
						::pthread_cond_destroy(&m_Handle);
					}

					inline void acquire()
					{
						m_Mutex.acquire();
					}

					inline void release()
					{
						m_Mutex.release();
					}

					inline void signal()
					{
						::pthread_cond_signal(&m_Handle);
					}

					template
					<
						typename PREDICATE
					>
					inline void wait(PREDICATE predicate)
					{
						while (!predicate())
						{
							::pthread_cond_wait(&m_Handle, &m_Mutex.m_Handle);
						}
					}

					template
					<
						typename PREDICATE
					>
					inline bool tryWait(PREDICATE predicate, Ash::Timer::Value duration)
					{
						duration = duration + Ash::Timer::getClockValue(Ash::Timer::Clock::System);

						timespec timeout = duration.as<timespec>();

						while (!predicate())
						{
							if (::pthread_cond_timedwait(&m_Handle, &m_Mutex.m_Handle, &timeout) != 0)
							{
								return false;
							}
						}

						return true;
					}

				private:
					pthread_cond_t m_Handle;
					Mutex          m_Mutex;

					Condition(const Condition &condition) = delete;
					Condition(Condition &&condition) = delete;
					Condition &operator = (const Condition &condition) = delete;
					Condition &operator = (Condition &&condition) = delete;
				};


				class Thread
				{
				public:
					using Handle = pthread_t;

					inline Thread() : m_Handle(), m_IsSetUp(false) {}

					inline Thread(Thread &&thread) noexcept : m_Handle(thread.m_Handle), m_IsSetUp(thread.m_IsSetUp)
					{
						thread.m_IsSetUp = false;
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
							m_IsSetUp = thread.m_IsSetUp;

							thread.m_IsSetUp = false;
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
						if (m_IsSetUp)
						{
							return false;
						}

						m_IsSetUp = runFunction(m_Handle, function, std::forward<ARGUMENTS>(arguments)...);

						return m_IsSetUp;
					}

					inline bool join()
					{
						m_IsSetUp = m_IsSetUp && (::pthread_join(m_Handle, nullptr) != 0);

						return !m_IsSetUp;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static bool runDetached(FUNCTION function, ARGUMENTS &&...arguments)
					{
						Handle handle;

						if (runFunction(handle, function, std::forward<ARGUMENTS>(arguments)...))
						{
							::pthread_detach(handle);
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
					static inline bool runFunction(Handle &handle, FUNCTION function, ARGUMENTS &&...arguments)
					{
						using CallableFunction = Ash::Callable::Function<FUNCTION, ARGUMENTS...>;

						CallableFunction *callable = new CallableFunction(function, std::forward<ARGUMENTS>(arguments)...);

						if (::pthread_create(&handle, nullptr, runCallable<CallableFunction>, callable) != 0)
						{
							delete callable;
							return false;
						}

						return true;
					}

					template
					<
						typename CALLABLE
					>
					static void *runCallable(void *param)
					{
						CALLABLE *callable = static_cast<CALLABLE *>(param);
						
						(*callable)();

						delete callable;

						return nullptr;
					}

				private:
					Handle m_Handle;
					bool   m_IsSetUp;

					Thread(const Thread &thread) = delete;
					Thread &operator = (const Thread &thread) = delete;
				};
			}
		}
	}
}
