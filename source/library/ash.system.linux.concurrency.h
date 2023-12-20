#pragma once

#include <utility>
#include <semaphore.h>
#include <pthread.h>
#include "ash.callable.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace Concurrency
			{
				class Mutex
				{
				public:
					using Handle = pthread_mutex_t;

					inline Mutex() : m_Handle(), m_IsSetUp(false)
					{
						create();
					}

					inline Mutex(Mutex &&mutex) noexcept : m_Handle(mutex.m_Handle), m_IsSetUp(mutex.m_IsSetUp)
					{
						mutex.m_IsSetUp = false; 
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
							m_IsSetUp = mutex.m_IsSetUp;

							mutex.m_IsSetUp = false;
						}

						return *this;
					}

					inline bool isSetUp() { return m_IsSetUp; } 

					inline operator Handle *() { return m_IsSetUp ? &m_Handle : nullptr; }

					inline bool acquire()
					{
						return m_IsSetUp && (::pthread_mutex_lock(&m_Handle) == 0);
					}

					inline bool release()
					{
						return m_IsSetUp && (::pthread_mutex_unlock(&m_Handle) == 0);
					}

				protected:
					inline void create()
					{
						m_IsSetUp = m_IsSetUp || (::pthread_mutex_init(&m_Handle, nullptr) == 0);
					}

					inline void destroy()
					{
						if (m_IsSetUp)
						{
							::pthread_mutex_unlock(&m_Handle);
							::pthread_mutex_destroy(&m_Handle);
							m_IsSetUp = false;
						}
					}

				private:
					Handle m_Handle;
					bool   m_IsSetUp;

					Mutex(const Mutex &mutex) = delete;
					Mutex &operator = (const Mutex &mutex) = delete;
				};


				class Condition
				{
				public:
					using Handle = pthread_cond_t;

					inline Condition() : m_Mutex(), m_Handle(), m_IsSetUp(false)
					{
						create();
					}

					inline Condition(Condition &&condition) noexcept : m_Mutex(std::move(condition.m_Mutex)), m_Handle(condition.m_Handle), m_IsSetUp(condition.m_IsSetUp)
					{
						condition.m_IsSetUp = false; 
					}

					virtual inline ~Condition()
					{
						destroy();
					}

					inline Condition &operator = (Condition &&condition) noexcept
					{
						if (this != &condition)
						{
							destroy();

							m_Mutex = std::move(condition.m_Mutex);
							m_Handle = condition.m_Handle;
							m_IsSetUp = condition.m_IsSetUp;

							condition.m_IsSetUp = false;
						}

						return *this;
					}

					inline bool isSetUp() const { return m_IsSetUp; }

					inline operator Handle *() { return m_IsSetUp ? &m_Handle : nullptr; }

					inline bool acquire()
					{
						return m_IsSetUp && m_Mutex.acquire();
					}

					inline bool signal()
					{
						return m_IsSetUp && (::pthread_cond_signal(&m_Handle) == 0);
					}

					inline bool wait()
					{
						return m_IsSetUp && (::pthread_cond_wait(&m_Handle, m_Mutex) == 0);
					}

					inline bool release()
					{
						return m_IsSetUp && m_Mutex.release();
					}

				protected:
					inline void create()
					{
						m_IsSetUp = m_IsSetUp || (m_Mutex.isSetUp() && (::pthread_cond_init(&m_Handle, nullptr) == 0));
					}

					inline void destroy()
					{
						if (m_IsSetUp)
						{
							::pthread_cond_signal(&m_Handle);
							::pthread_cond_destroy(&m_Handle);
							m_IsSetUp = false;
						}
					}

				private:
					Mutex  m_Mutex;
					Handle m_Handle;
					bool   m_IsSetUp;

					Condition(const Condition &condition) = delete;
					Condition &operator = (const Condition &condition) = delete;
				};


				class Event : public Condition
				{
				public:
					enum Reset
					{
						Manual,
						Automatic
					};

					inline Event(Reset reset = Manual, bool value = false) : Condition(), m_Reset(reset), m_Value(value) {}

					inline Event(Event &&event) noexcept : Condition(std::move(event)), m_Reset(event.m_Reset), m_Value(event.m_Value)
					{
						event.m_Value = false;
					}

					inline Event &operator = (Event &&event) noexcept
					{
						if (this != &event)
						{
							Condition::operator = (std::move(event));

							m_Reset = event.m_Reset;
							m_Value = event.m_Value;

							event.m_Value = false;
						}

						return *this;
					}

					inline bool reset()
					{
						if (!Condition::acquire())
						{
							return false;
						}

						m_Value = false;

						return Condition::release();
					}

					inline bool signal()
					{
						if (!Condition::acquire())
						{
							return false;
						}

						if (Condition::signal())
						{
							m_Value = true;
						}

						return Condition::release();
					}

					inline bool tryWait()
					{
						if (!Condition::acquire())
						{
							return false;
						}

						bool acquired = m_Value;

						m_Value = false;

						return Condition::release() && acquired;
					}

					inline bool wait()
					{
						if (!Condition::acquire())
						{
							return false;
						}

						while (!m_Value)
						{
							if (!Condition::wait())
							{
								Condition::release();
								return false;
							}
						}

						if (m_Reset == Reset::Automatic)
						{
							m_Value = false;
						}

						return Condition::release();
					}

				private:
					Reset m_Reset;
					bool  m_Value;

					Event(const Event &event) = delete;
					Event &operator = (const Event &event) = delete;
				};


				class Semaphore
				{
				public:
					using Handle = sem_t;

					using Count = unsigned int;

					inline Semaphore(Count count = 0) : m_Handle(), m_IsSetUp(false)
					{
						create(count);
					}

					inline Semaphore(Semaphore &&semaphore) noexcept : m_Handle(semaphore.m_Handle), m_IsSetUp(semaphore.m_IsSetUp)
					{
						semaphore.m_IsSetUp = false;
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
							m_IsSetUp = semaphore.m_IsSetUp;

							semaphore.m_IsSetUp = false;
						}

						return *this;
					}

					inline bool isSetUp() { return m_IsSetUp; } 

					inline operator Handle *() { return m_IsSetUp ? &m_Handle : nullptr; }

					inline bool acquire()
					{
						return m_IsSetUp && (::sem_wait(&m_Handle) == 0);
					}

					inline bool release()
					{
						return m_IsSetUp && (::sem_post(&m_Handle) == 0);
					}

				protected:
					inline void create(Count count)
					{
						m_IsSetUp = m_IsSetUp || (::sem_init(&m_Handle, 0, count) == 0);
					}

					inline void destroy()
					{
						if (m_IsSetUp)
						{
							::sem_destroy(&m_Handle);
							m_IsSetUp = false;
						}
					}

				private:
					Handle m_Handle;
					bool   m_IsSetUp;

					Semaphore(const Semaphore &semaphore) = delete;
					Semaphore &operator = (const Semaphore &semaphore) = delete;
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
						if (!join())
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
							pthread_detach(handle);
							return true;
						}
						return false;
					}

				protected:
					template
					<
						typename CALLABLE
					>
					class Context : public CALLABLE
					{
					public:
						using Callable = CALLABLE;

						template
						<
							typename FUNCTION,
							typename ...ARGUMENTS
						>
						Context(FUNCTION function, ARGUMENTS &&...arguments) : Callable(function, std::forward<ARGUMENTS>(arguments)...), m_Event() {}

						inline bool signal() { return m_Event.signal(); }

						inline bool wait() { return m_Event.wait(); }

					private:
						Event m_Event;
					};

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static inline bool runFunction(Handle &handle, FUNCTION function, ARGUMENTS &&...arguments)
					{
						using CallableFunction = Ash::Callable::Function<FUNCTION, ARGUMENTS...>;

						Context<CallableFunction> context(function, std::forward<ARGUMENTS>(arguments)...);

						if (::pthread_create(&handle, nullptr, runCallable<CallableFunction>, &context) != 0)
						{
							return false;
						}

						context.wait();
						return true;
					}

					template
					<
						typename CALLABLE
					>
					static void *runCallable(void *param)
					{
						Context<CALLABLE> *context = static_cast<Context<CALLABLE> *>(param);
						CALLABLE callable = std::move(*context);
						
						if (context->signal())
						{
							callable();
						}

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
