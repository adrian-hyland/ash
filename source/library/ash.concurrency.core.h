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
		using Condition = Ash::System::Windows::Concurrency::Condition;
		using Mutex = Ash::System::Windows::Concurrency::Mutex;
		using Thread = Ash::System::Windows::Concurrency::Thread;
#elif defined __linux__
		using Condition = Ash::System::Linux::Concurrency::Condition;
		using Mutex = Ash::System::Linux::Concurrency::Mutex;
		using Thread = Ash::System::Linux::Concurrency::Thread;
#endif

		class Event : public Condition
		{
		public:
			enum Reset
			{
				Manual,
				Automatic
			};

			inline Event(Reset reset = Manual, bool value = false) : Condition(), m_Reset(reset), m_Value(value) {}

			inline void reset()
			{
				Condition::acquire();

				m_Value = false;

				Condition::release();
			}

			inline void signal()
			{
				Condition::acquire();

				m_Value = true;

				Condition::signal();
				Condition::release();
			}

			inline bool tryWait()
			{
				Condition::acquire();

				bool acquired = m_Value;
				if (m_Reset == Reset::Automatic)
				{
					m_Value = false;
				}

				Condition::release();
				
				return acquired;
			}

			inline bool tryWait(Ash::Timer::Value duration)
			{
				Condition::acquire();

				if (!Condition::tryWait([this]() { return m_Value; }, duration))
				{
					Condition::release();
					return false;
				}

				if (m_Reset == Reset::Automatic)
				{
					m_Value = false;
				}

				Condition::release();
				return true;
			}

			inline void wait()
			{
				Condition::acquire();

				Condition::wait([this]() { return m_Value; });

				if (m_Reset == Reset::Automatic)
				{
					m_Value = false;
				}

				Condition::release();
			}

		private:
			Reset m_Reset;
			bool  m_Value;

			Event(const Event &event) = delete;
			Event(Event &&event) = delete;
			Event &operator = (const Event &event) = delete;
			Event &operator = (Event &&event) = delete;
		};


		class Semaphore : public Condition
		{
		public:
			using Count = uintmax_t;

			inline Semaphore(Count count = 0) : Condition(), m_Count(count) {}

			inline bool tryAcquire()
			{
				Condition::acquire();

				bool acquired = (m_Count > 0);
				if (acquired)
				{
					m_Count--;
				}

				Condition::release();
				
				return acquired;
			}

			inline bool tryAcquire(Ash::Timer::Value duration)
			{
				Condition::acquire();

				if (!Condition::tryWait([this]() { return m_Count > 0; }, duration))
				{
					Condition::release();
					return false;
				}

				m_Count--;

				Condition::release();
				return true;
			}

			inline void acquire()
			{
				Condition::acquire();

				Condition::wait([this]() { return m_Count > 0; });

				m_Count--;

				Condition::release();
			}

			inline void release(Count count = 1)
			{
				Condition::acquire();

				for (Count n = 0; n < count; n++)
				{
					m_Count++;
					Condition::signal();
				}

				Condition::release();
			}

		private:
			Count m_Count;

			Semaphore(const Semaphore &semaphore) = delete;
			Semaphore(Semaphore &&semaphore) = delete;
			Semaphore &operator = (const Semaphore &semaphore) = delete;
			Semaphore &operator = (Semaphore &&semaphore) = delete;
		};
	}
}
