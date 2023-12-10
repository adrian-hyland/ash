#pragma once

#include <cstdint>
#include <cmath>
#include <windows.h>
#include "ash.type.h"
#include "ash.real.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			class Timer
			{
			public:
				enum Clock
				{
					HighResolution,
					LowResolution,
					ProcessTime,
					ThreadTime
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
							return { decltype(VALUE::tv_sec)(m_Unit / Unit::Seconds), decltype(VALUE::tv_nsec)((m_Unit % Unit::Seconds) / Unit::Nanoseconds) };
						}
						else if constexpr (Ash::Type::isSame<VALUE, struct timeval>)
						{
							return { decltype(VALUE::tv_sec)(m_Unit / Unit::Seconds), decltype(VALUE::tv_usec)((m_Unit % Unit::Seconds) / Unit::Microseconds) };
						}
						else if constexpr (Ash::Type::isSame<VALUE, Unit> || Ash::Type::isInteger<VALUE>)
						{
							return VALUE(m_Unit / unit);
						}
						else // constexpr(Ash::Type::isClass<VALUE, Ash::Generic::Real> || Ash::Type::isFloatingPoint<VALUE>)
						{
							return VALUE(std::underlying_type_t<Unit>(m_Unit)) / std::underlying_type_t<Unit>(unit);
						}
					}

				private:
					Unit m_Unit;
				};

				static inline Timer start(Clock clock = Clock::HighResolution)
				{
					Value startValue;

					if (clock == Clock::HighResolution)
					{
						static Value resolution;

						if (resolution == 0)
						{
							resolution = getClockResolution(clock);
						}

						LARGE_INTEGER counter;
						::QueryPerformanceCounter(&counter);
						startValue = Value(counter.QuadPart, resolution);
					}
					else if (clock == Clock::LowResolution)
					{
						startValue = Value(::GetTickCount64(), Value::Unit::Milliseconds);
					}
					else
					{
						FILETIME creationTime;
						FILETIME exitTime;
						FILETIME kernelTime;
						FILETIME userTime;

						if (clock == Clock::ProcessTime)
						{
							::GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime);
						}
						else // (clock == Clock::ThreadTime)
						{
							::GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime);
						}

						ULARGE_INTEGER kernelTimeValue;
						ULARGE_INTEGER userTimeValue;

						kernelTimeValue.LowPart = kernelTime.dwLowDateTime;
						kernelTimeValue.HighPart = kernelTime.dwHighDateTime;

						userTimeValue.LowPart = userTime.dwLowDateTime;
						userTimeValue.HighPart = userTime.dwHighDateTime;

						startValue = Value((kernelTimeValue.QuadPart + userTimeValue.QuadPart) * 100, Value::Unit::Nanoseconds);
					}

					return Timer(clock, startValue);
				}

				inline Value restart()
				{
					Timer timer = start(m_Clock);

					Value value = timer.m_Value - m_Value;
					
					m_Value = timer.m_Value;

					return value;
				}

				inline Value getValue() const
				{
					return start(m_Clock).m_Value - m_Value;
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
					if (clock == Clock::HighResolution)
					{
						LARGE_INTEGER frequency;
						QueryPerformanceFrequency(&frequency);
						return Value::Unit::Seconds / frequency.QuadPart;
					}
					else if (clock == Clock::LowResolution)
					{
						return Value(10, Value::Unit::Milliseconds);
					}
					else // (clock == Clock::ProcessTime) || (clock == Clock::ThreadTime)
					{
						return Value(100, Value::Unit::Nanoseconds);
					}
				}

				static inline void sleep(Value value)
				{
					::Sleep(value.as(Value::Unit::Milliseconds));
				}

			protected:
				inline Timer(Clock clock, Value value) : m_Clock(clock), m_Value(value) {}

			private:
				Clock m_Clock;
				Value m_Value;
			};
		}
	}
}
