#pragma once

#include "ash.type.h"
#include "ash.real.h"
#if defined __WIN32__
#include "ash.system.windows.clock.h"
#elif defined __linux__
#include "ash.system.linux.clock.h"
#endif


namespace Ash
{
#if defined __WIN32__
	using Clock = System::Windows::Clock;
#elif defined __linux__
	using Clock = System::Linux::Clock;
#endif

	class Timer : public Clock
	{
	public:
		class Value
		{
		public:
			using Unit = uint64_t;

			static constexpr Unit Nanoseconds  = 1;
			static constexpr Unit Microseconds = Nanoseconds  * 1000;
			static constexpr Unit Milliseconds = Microseconds * 1000;
			static constexpr Unit Seconds      = Milliseconds * 1000;
			static constexpr Unit Minutes      = Seconds * 60;
			static constexpr Unit Hours        = Minutes * 60;
			static constexpr Unit Days         = Hours * 24;

			template
			<
				Unit UNIT = Nanoseconds
			>
			static constexpr Unit maximum = Unit(std::numeric_limits<Unit>::max() / UNIT);

			constexpr Value() : m_Unit() {};

			template
			<
				typename VALUE = Unit,
				typename = typename Ash::Type::Check<VALUE, Ash::Type::Requirement::IsSame, timespec>::template
																Or<Ash::Type::Requirement::IsSame, timeval>::template
																Or<Ash::Type::Requirement::IsSame, Unit>::template
																Or<Ash::Type::Requirement::IsInteger>::template
																Or<Ash::Type::Requirement::IsClass, Ash::Generic::Real>::template
																Or<Ash::Type::Requirement::IsFloatingPoint>::IsValid
			>
			constexpr Value(VALUE value, Unit unit = Nanoseconds) : m_Unit()
			{
				if constexpr (Ash::Type::isSame<VALUE, timespec>)
				{
					m_Unit = Unit((value.tv_sec >= 0) ? value.tv_sec * Seconds + value.tv_nsec * Nanoseconds : 0);
				}
				else if constexpr (Ash::Type::isSame<VALUE, timeval>)
				{
					m_Unit = Unit((value.tv_sec >= 0) ? value.tv_sec * Seconds + value.tv_usec * Microseconds : 0);
				}
				else if constexpr (Ash::Type::isSame<VALUE, Unit> || Ash::Type::isInteger<VALUE>)
				{
					m_Unit = Unit((value > 0) ? value * unit : 0);
				}
				else // constexpr(Ash::Type::isClass<VALUE, Ash::Generic::Real> || Ash::Type::isFloatingPoint<VALUE>)
				{
					m_Unit = Unit((value > 0) ? std::llround(value * unit) : 0);
				}
			}

			constexpr operator Unit () const { return m_Unit; }

			template
			<
				typename VALUE = Unit,
				typename = typename Ash::Type::Check<VALUE, Ash::Type::Requirement::IsSame, timespec>::template
																Or<Ash::Type::Requirement::IsSame, timeval>::template
																Or<Ash::Type::Requirement::IsSame, Unit>::template
																Or<Ash::Type::Requirement::IsInteger>::template
																Or<Ash::Type::Requirement::IsClass, Ash::Generic::Real>::template
																Or<Ash::Type::Requirement::IsFloatingPoint>::IsValid
			>
			constexpr VALUE as(Unit unit = Nanoseconds) const
			{
				if constexpr (Ash::Type::isSame<VALUE, timespec>)
				{
					return { decltype(VALUE::tv_sec)(m_Unit / Seconds), decltype(VALUE::tv_nsec)((m_Unit % Seconds) / Nanoseconds) };
				}
				else if constexpr (Ash::Type::isSame<VALUE, timeval>)
				{
					return { decltype(VALUE::tv_sec)(m_Unit / Seconds), decltype(VALUE::tv_usec)((m_Unit % Seconds) / Microseconds) };
				}
				else if constexpr (Ash::Type::isSame<VALUE, Unit> || Ash::Type::isInteger<VALUE>)
				{
					return VALUE(m_Unit / unit);
				}
				else // constexpr(Ash::Type::isClass<VALUE, Ash::Generic::Real> || Ash::Type::isFloatingPoint<VALUE>)
				{
					return VALUE(Unit(m_Unit)) / Unit(unit);
				}
			}

		private:
			Unit m_Unit;
		};

		static inline Timer start(Type type = Type::HighResolution)
		{
			return Timer(type);
		}

		inline void restart()
		{
			m_Start = getTick();
		}

		inline Value getValue() const
		{
			return Value(getTick() - m_Start, Value::Unit(getTickDuration()));
		}

		inline Value getResolution() const { return Value(getTickResolution(), Value::Unit(getTickDuration())); }

		inline void sleepUntil(Value duration) const
		{
			Value currentDuration = getValue();

			if (currentDuration < duration)
			{
				sleep(duration - currentDuration);
			}
		}

		static inline void sleepFor(Value duration)
		{
			sleep(duration);
		}

		static inline Value getClockValue(Type type)
		{
			return Value(Clock(type).getTick(), Ash::Timer::Value::Unit(Clock(type).getTickDuration()));
		}

	protected:
		inline Timer(Type type) : Clock(type), m_Start(getTick()) {}

	private:
		Clock::Tick m_Start;
	};
}
