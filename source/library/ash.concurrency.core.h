#pragma once

#if defined PLATFORM_WINDOWS
#include "ash.system.windows.concurrency.h"
#elif defined PLATFORM_LINUX
#include "ash.system.linux.concurrency.h"
#endif


namespace Ash
{
	namespace Concurrency
	{
#if defined PLATFORM_WINDOWS
		using Condition = Ash::System::Windows::Concurrency::Condition;
		using Mutex = Ash::System::Windows::Concurrency::Mutex;
		using Thread = Ash::System::Windows::Concurrency::Thread;
		using Process = Ash::System::Windows::Concurrency::Process;
#elif defined PLATFORM_LINUX
		using Condition = Ash::System::Linux::Concurrency::Condition;
		using Mutex = Ash::System::Linux::Concurrency::Mutex;
		using Thread = Ash::System::Linux::Concurrency::Thread;
		using Process = Ash::System::Linux::Concurrency::Process;
#endif

		namespace Lock
		{
			template
			<
				typename LOCK,
				typename = Ash::Type::IsClass<LOCK, Ash::Concurrency::Generic::Lock>
			>
			class Scope
			{
			public:
				using Lock = LOCK;

				constexpr Scope() : m_Lock(nullptr) {}

				[[nodiscard]]
				Scope(Lock &lock) : m_Lock(&lock) { m_Lock->acquire(); }

				Scope(Scope &&scope) : m_Lock(scope.m_Lock) { scope.m_Lock = nullptr; }

				~Scope()
				{
					if (m_Lock != nullptr)
					{
						m_Lock->release();
					}
				}

				Scope &operator = (Scope &&scope)
				{
					m_Lock = scope.m_Lock;
					scope.m_Lock = nullptr;
				}

			private:
				Lock *m_Lock;

				Scope(const Scope &scope) = delete;
				Scope &operator = (const Scope &scope) = delete;
			};
		}

		class Event
		{
		public:
			enum Reset
			{
				Manual,
				Automatic
			};

			inline Event(Reset reset = Manual, bool value = false) : m_Condition(), m_Reset(reset), m_Value(value) {}

			inline void reset()
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				m_Value = false;
			}

			inline void signal()
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				m_Value = true;
				m_Condition.signal();
			}

			inline bool tryWait()
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				bool acquired = m_Value;
				if (m_Reset == Reset::Automatic)
				{
					m_Value = false;
				}
				return acquired;
			}

			inline bool tryWait(Ash::Timer::Value duration)
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				if (!m_Condition.tryWait([this]() { return m_Value; }, duration))
				{
					return false;
				}
				else if (m_Reset == Reset::Automatic)
				{
					m_Value = false;
				}

				return true;
			}

			inline void wait()
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				m_Condition.wait([this]() { return m_Value; });
				if (m_Reset == Reset::Automatic)
				{
					m_Value = false;
				}
			}

		private:
			Ash::Concurrency::Condition m_Condition;
			Reset                       m_Reset;
			bool                        m_Value;

			Event(const Event &event) = delete;
			Event(Event &&event) = delete;
			Event &operator = (const Event &event) = delete;
			Event &operator = (Event &&event) = delete;
		};


		class Semaphore : public Ash::Concurrency::Generic::Lock
		{
		public:
			using Count = uintmax_t;

			inline Semaphore(Count count = 0) : m_Condition(), m_Count(count) {}

			inline bool tryAcquire()
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				bool acquired = (m_Count > 0);
				if (acquired)
				{
					m_Count--;
				}
				return acquired;
			}

			inline bool tryAcquire(Ash::Timer::Value duration)
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				if (!m_Condition.tryWait([this]() { return m_Count > 0; }, duration))
				{
					return false;
				}

				m_Count--;
				return true;
			}

			inline void acquire()
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				m_Condition.wait([this]() { return m_Count > 0; });
				m_Count--;
			}

			inline void release(Count count = 1)
			{
				Ash::Concurrency::Lock::Scope lock(m_Condition);

				for (Count n = 0; n < count; n++)
				{
					m_Count++;
					m_Condition.signal();
				}
			}

		private:
			Ash::Concurrency::Condition m_Condition;
			Count                       m_Count;

			Semaphore(const Semaphore &semaphore) = delete;
			Semaphore(Semaphore &&semaphore) = delete;
			Semaphore &operator = (const Semaphore &semaphore) = delete;
			Semaphore &operator = (Semaphore &&semaphore) = delete;
		};
	}
}
