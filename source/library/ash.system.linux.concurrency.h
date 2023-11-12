#pragma once

#include <utility>
#include <pthread.h>

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


				template
				<
					typename RUNNABLE,
					bool     IS_DETACHABLE = false
				>
				class Thread
				{
				public:
					using Handle = pthread_t;

					using Runnable = RUNNABLE;

					template
					<
						typename ...ARGS
					>
					inline Thread(ARGS ...args) : m_Runnable(std::forward<ARGS>(args)...), m_Handle(), m_IsAttached(false) {}

					inline Thread(Thread &&thread) noexcept : m_Runnable(std::move(thread.m_Runnable)), m_Handle(thread.m_Handle), m_IsAttached(thread.m_IsAttached)
					{
						thread.m_IsAttached = false;
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
							m_IsAttached = thread.m_IsAttached;

							thread.m_IsAttached = false;
						}

						return *this;
					}

					inline bool start()
					{
						m_IsAttached = join() && (::pthread_create(&m_Handle, nullptr, run, &m_Runnable) == 0);

						return m_IsAttached;
					}

					inline bool join()
					{
						m_IsAttached = m_IsAttached && (::pthread_join(m_Handle, nullptr) != 0);

						return !m_IsAttached;
					}

					template
					<
						typename ...ARGS
					>
					static bool start(ARGS ...args)
					{
						return Thread<Runnable, true>(std::forward<ARGS>(args)...).start();
					}

				protected:
					static void *run(void *param)
					{
						static_cast<Runnable *>(param)->run();

						return nullptr;
					}

				private:
					Runnable m_Runnable;
					Handle   m_Handle;
					bool     m_IsAttached;

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
					using Handle = pthread_t;

					using Runnable = RUNNABLE;

					template
					<
						typename ...ARGS
					>
					inline Thread(ARGS ...args) : m_DetachEvent(), m_Detachable(&m_DetachEvent, std::forward<ARGS>(args)...) {}

					inline Thread(Thread &&thread) noexcept : m_DetachEvent(std::move(thread.m_DetachEvent)), m_Detachable(std::move(thread.m_Detachable)) {}

					inline Thread &operator = (Thread &&thread) noexcept
					{
						if (this != &thread)
						{
							m_DetachEvent = std::move(thread.m_DetachEvent);
							m_Detachable = std::move(thread.m_Detachable);
						}

						return *this;
					}

					inline bool start()
					{
						Handle handle;
						bool ok;
						
						ok = m_DetachEvent.reset();
						
						ok = ok && (::pthread_create(&handle, nullptr, run, &m_Detachable) == 0);

						ok = ok && m_DetachEvent.wait();

						return ok;
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
							return (::pthread_detach(::pthread_self()) == 0) && m_Event->signal();
						}

					private:
						Event *m_Event;

						Detachable(const Detachable &detachable) = delete;
						Detachable &operator = (const Detachable &detachable) = delete;
					};

					static void *run(void *param)
					{
						Detachable runnable(std::move(*static_cast<Detachable *>(param)));
						if (runnable.detach())
						{
							runnable.run();
						}

						return nullptr;
					}

				private:
					Event      m_DetachEvent;
					Detachable m_Detachable;

					Thread(const Thread &thread) = delete;
					Thread &operator = (const Thread &thread) = delete;
				};
			}
		}
	}
}
