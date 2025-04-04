#pragma once

#include "ash.type.h"
#include "ash.real.h"
#if defined PLATFORM_WINDOWS
#include "ash.system.windows.clock.h"
#elif defined PLATFORM_LINUX
#include "ash.system.linux.clock.h"
#endif


namespace Ash
{
#if defined PLATFORM_WINDOWS
	using Clock = System::Windows::Clock;
#elif defined PLATFORM_LINUX
	using Clock = System::Linux::Clock;
#endif

	class Timer : public Clock
	{
	public:
		class Value
		{
		public:
			using Unit = uint64_t;

			static constexpr Unit nanosecond  = 1;
			static constexpr Unit microsecond = nanosecond  * 1000;
			static constexpr Unit millisecond = microsecond * 1000;
			static constexpr Unit second      = millisecond * 1000;
			static constexpr Unit minute      = second * 60;
			static constexpr Unit hour        = minute * 60;
			static constexpr Unit day         = hour * 24;

			template
			<
				Unit UNIT = nanosecond
			>
			static constexpr Unit maximum = Unit(std::numeric_limits<Unit>::max() / UNIT);

			constexpr Value() : m_Unit() {};

			template
			<
				typename VALUE = Unit,
				typename = typename Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<VALUE, timespec>,
				                                          Ash::Type::Requirement::IsSame<VALUE, timeval>>::IsValid
			>
			constexpr Value(VALUE value) : m_Unit()
			{
				if constexpr (Ash::Type::isSame<VALUE, timespec>)
				{
					m_Unit = Unit((value.tv_sec >= 0) ? value.tv_sec * second + value.tv_nsec * nanosecond : 0);
				}
				else // constexpr (Ash::Type::isSame<VALUE, timeval>)
				{
					m_Unit = Unit((value.tv_sec >= 0) ? value.tv_sec * second + value.tv_usec * microsecond : 0);
				}
			}

			template
			<
				typename VALUE = Unit,
				typename = typename Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<VALUE>,
				                                          Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>,
				                                          Ash::Type::Requirement::IsFloatingPoint<VALUE>>::IsValid
			>
			constexpr Value(VALUE value, Unit unit = nanosecond) : m_Unit()
			{
				if constexpr (Ash::Type::isInteger<VALUE>)
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
				typename = typename Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<VALUE, timespec>,
				                                          Ash::Type::Requirement::IsSame<VALUE, timeval>>::IsValid
			>
			constexpr VALUE as() const
			{
				if constexpr (Ash::Type::isSame<VALUE, timespec>)
				{
					return { decltype(VALUE::tv_sec)(m_Unit / second), decltype(VALUE::tv_nsec)((m_Unit % second) / nanosecond) };
				}
				else // constexpr (Ash::Type::isSame<VALUE, timeval>)
				{
					return { decltype(VALUE::tv_sec)(m_Unit / second), decltype(VALUE::tv_usec)((m_Unit % second) / microsecond) };
				}
			}

			template
			<
				typename VALUE = Unit,
				typename = typename Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<VALUE>,
				                                          Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>,
				                                          Ash::Type::Requirement::IsFloatingPoint<VALUE>>::IsValid
			>
			constexpr VALUE as(Unit unit = nanosecond) const
			{
				if constexpr (Ash::Type::isInteger<VALUE>)
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

		inline Timer(Type type = Type::HighResolution) : Clock(type), m_Start(getTick()) {}

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

	private:
		Clock::Tick m_Start;
	};
}
