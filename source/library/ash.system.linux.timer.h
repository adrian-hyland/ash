#pragma once

#include <cstdint>
#include <cmath>
#include <ctime>
#include "ash.type.h"
#include "ash.real.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			class Timer
			{
			public:
				enum Clock
				{
					HighResolution,
					LowResolution
				};

				class Value
				{
				public:
					enum Unit : uint64_t
					{
						Nanoseconds  = 1,
						Microseconds = Nanoseconds  * 1000,
						Milliseconds = Microseconds * 1000,
						Seconds      = Milliseconds * 1000,
						Minutes      = Seconds * 60,
						Hours        = Minutes * 60,
						Days         = Hours * 24
					};

					template
					<
						Unit UNIT = Unit::Nanoseconds
					>
					static constexpr Unit maximum = Unit(std::numeric_limits<Unit>::max() / UNIT);

					constexpr Value() : m_Unit() {};

					template
					<
						typename VALUE = Unit,
						typename = typename Ash::Type::Check<VALUE, Ash::Type::Requirement::IsSame, struct timespec>::template
						                                  Or<Ash::Type::Requirement::IsSame, struct timeval>::template
						                                  Or<Ash::Type::Requirement::IsSame, Unit>::template
						                                  Or<Ash::Type::Requirement::IsInteger>::template
																	 Or<Ash::Type::Requirement::IsClass, Ash::Generic::Real>::template
						                                  Or<Ash::Type::Requirement::IsFloatingPoint>::IsValid
					>
					constexpr Value(VALUE value, Unit unit = Unit::Nanoseconds) : m_Unit()
					{
						if constexpr (Ash::Type::isSame<VALUE, struct timespec>)
						{
							m_Unit = Unit((value.tv_sec >= 0) ? value.tv_sec * Unit::Seconds + value.tv_nsec * Unit::Nanoseconds : 0);
						}
						else if constexpr (Ash::Type::isSame<VALUE, struct timeval>)
						{
							m_Unit = Unit((value.tv_sec >= 0) ? value.tv_sec * Unit::Seconds + value.tv_usec * Unit::Microseconds : 0);
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
						typename = typename Ash::Type::Check<VALUE, Ash::Type::Requirement::IsSame, struct timespec>::template
						                                  Or<Ash::Type::Requirement::IsSame, struct timeval>::template
						                                  Or<Ash::Type::Requirement::IsSame, Unit>::template
						                                  Or<Ash::Type::Requirement::IsInteger>::template
																	 Or<Ash::Type::Requirement::IsClass, Ash::Generic::Real>::template
						                                  Or<Ash::Type::Requirement::IsFloatingPoint>::IsValid
					>
					constexpr VALUE as(Unit unit = Unit::Nanoseconds) const
					{
						if constexpr (Ash::Type::isSame<VALUE, struct timespec>)
						{
							return { time_t(m_Unit / Unit::Seconds), long((m_Unit % Unit::Seconds) / Unit::Nanoseconds) };
						}
						else if constexpr (Ash::Type::isSame<VALUE, struct timeval>)
						{
							return { time_t(m_Unit / Unit::Seconds), long((m_Unit % Unit::Seconds) / Unit::Microseconds) };
						}
						else if constexpr (Ash::Type::isSame<VALUE, Unit> || Ash::Type::isInteger<VALUE>)
						{
							return VALUE(m_Unit / unit);
						}
						else // constexpr(Ash::Type::isClass<VALUE, Ash::Generic::Real> || Ash::Type::isFloatingPoint<VALUE>)
						{
							return VALUE(std::underlying_type_t<Unit>(m_Unit)) / unit;
						}
					}

				private:
					Unit m_Unit;
				};

				static inline Timer start(Clock clock = Clock::HighResolution)
				{
					return Timer(clock, getClockTime(getClockId(clock)));
				}

				inline Value restart()
				{
					Value startValue = m_Value;
					
					m_Value = getClockTime(getClockId(m_Clock));

					return m_Value - startValue;
				}

				inline Value getValue() const
				{
					return getClockTime(getClockId(m_Clock)) - m_Value;
				}

				inline Clock getClock() const { return m_Clock; }

				inline Value getClockResolution() const { return getClockResolution(m_Clock); }

				inline void sleepUntil(Value duration) const
				{
					Value currentDuration = getValue();

					if (currentDuration < duration)
					{
						sleep(duration - currentDuration);
					}
				}

				static inline Value getClockResolution(Clock clock)
				{
					struct timespec time = {};
				
					::clock_getres(getClockId(clock), &time);
					
					return time;
				}

				static inline void sleep(Value duration)
				{
					struct timespec wakeTime = Value(getClockTime(CLOCK_MONOTONIC) + duration).as<struct timespec>();

					while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeTime, nullptr) == EINTR)
						;
				}

			protected:
				inline Timer(Clock clock, Value value) : m_Clock(clock), m_Value(value) {}

				static inline Value getClockTime(clockid_t clockId)
				{
					struct timespec time = {};
				
					::clock_gettime(clockId, &time);

					return time;
				}

				static inline clockid_t getClockId(Clock clock) { return (clock == Clock::HighResolution) ? CLOCK_MONOTONIC_RAW : CLOCK_MONOTONIC_COARSE; }

			private:
				Clock m_Clock;
				Value m_Value;
			};
		}
	}
}
