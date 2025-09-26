#pragma once

#include <cstdint>
#include "ash.calendar.error.h"
#include "ash.integer.h"


namespace Ash
{
	namespace Calendar
	{
		class Hour : public Ash::Integer::Cycle<0, 23>
		{
		public:
			using Cycle = Ash::Integer::Cycle<0, 23>;

			static constexpr Type perDay = Cycle::size;

			constexpr Hour() : Cycle() {}

			constexpr Hour(const Cycle &value) : Cycle(value) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Hour(VALUE value) : Cycle(value) {}
		};


		class Minute : public Ash::Integer::Cycle<0, 59>
		{
		public:
			using Cycle = Ash::Integer::Cycle<0, 59>;

			static constexpr Type perHour = Cycle::size;

			constexpr Minute() : Cycle() {}

			constexpr Minute(const Cycle &value) : Cycle(value) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Minute(VALUE value) : Cycle(value) {}
		};


		class Second : public Ash::Integer::Cycle<0, 59>
		{
		public:
			using Cycle = Ash::Integer::Cycle<0, 59>;

			static constexpr Type perMinute = Cycle::size;

			constexpr Second() : Cycle() {}

			constexpr Second(const Cycle &value) : Cycle(value) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Second(VALUE value) : Cycle(value) {}
		};


		using TimeDuration = Ash::Integer::Cycle<0, Hour::perDay * Minute::perHour * Second::perMinute - 1>;


		class Time
		{
		public:
			static constexpr TimeDuration::Type second = 1;

			static constexpr TimeDuration::Type minute = second * Second::perMinute;

			static constexpr TimeDuration::Type hour = minute * Minute::perHour;

			static constexpr TimeDuration::Type day = hour * Hour::perDay;

			static constexpr TimeDuration::Type midnight = 0;

			static constexpr TimeDuration::Type midday = day / 2;

			constexpr Time() : m_Hour(), m_Minute(), m_Second() {}

			template
			<
				typename SECONDS,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<SECONDS>, Ash::Type::Requirement::IsClass<SECONDS, TimeDuration>>::IsValid
			>
			constexpr Time(SECONDS seconds) : Time()
			{
				set(seconds);
			}

			template
			<
				typename HOUR,
				typename MINUTE,
				typename SECOND,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<HOUR>, Ash::Type::Requirement::IsClass<HOUR, Hour>>::IsValid,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MINUTE>, Ash::Type::Requirement::IsClass<MINUTE, Minute>>::IsValid,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<SECOND>, Ash::Type::Requirement::IsClass<SECOND, Second>>::IsValid
			>
			static constexpr Time withHourMinuteSecond(HOUR hour, MINUTE minute, SECOND second)
			{
				Time time;

				time.setHourMinuteSecond(hour, minute, second).throwOnError();

				return time;
			}

			template
			<
				typename SECONDS,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<SECONDS>, Ash::Type::Requirement::IsClass<SECONDS, TimeDuration>>::IsValid
			>
			constexpr void set(SECONDS seconds)
			{
				TimeDuration duration = seconds;
				m_Hour = duration / hour;
				m_Minute = duration / minute;
				m_Second = duration / second;
			}

			template
			<
				typename HOUR,
				typename MINUTE,
				typename SECOND,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<HOUR>, Ash::Type::Requirement::IsClass<HOUR, Hour>>::IsValid,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MINUTE>, Ash::Type::Requirement::IsClass<MINUTE, Minute>>::IsValid,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<SECOND>, Ash::Type::Requirement::IsClass<SECOND, Second>>::IsValid
			>
			constexpr Ash::Error::Value setHourMinuteSecond(HOUR hour, MINUTE minute, SECOND second)
			{
				Ash::Error::Value error = validateHourMinuteSecond(hour, minute, second);
				if (error == Ash::Error::none)
				{
					m_Hour = hour;
					m_Minute = minute;
					m_Second = second;
				}
				return error;
			}

			template
			<
				typename HOUR,
				typename MINUTE,
				typename SECOND,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<HOUR>, Ash::Type::Requirement::IsClass<HOUR, Hour>>::IsValid,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MINUTE>, Ash::Type::Requirement::IsClass<MINUTE, Minute>>::IsValid,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<SECOND>, Ash::Type::Requirement::IsClass<SECOND, Second>>::IsValid
			>
			static constexpr Ash::Error::Value validateHourMinuteSecond(HOUR hour, MINUTE minute, SECOND second)
			{
				if (!isHourValid(hour))
				{
					return Ash::Calendar::Error::invalidHour;
				}
				else if (!isMinuteValid(minute))
				{
					return Ash::Calendar::Error::invalidMinute;
				}
				else if (!isSecondValid(second))
				{
					return Ash::Calendar::Error::invalidSecond;
				}
				else
				{
					return Ash::Error::none;
				}
			}

			constexpr operator TimeDuration () const { return TimeDuration::Value(m_Hour) * hour + TimeDuration::Value(m_Minute) * minute + TimeDuration::Value(m_Second) * second; }

			constexpr Hour getHour() const { return m_Hour; }

			constexpr Minute getMinute() const { return m_Minute; }

			constexpr Second getSecond() const { return m_Second; }

			constexpr bool isMorning() const { return m_Hour < Time(midday).getHour(); }

			constexpr bool isAfternoon() const { return !isMorning(); }

			constexpr Time &operator ++ () { *this = TimeDuration(*this) + 1; return *this; }

			constexpr Time &operator -- () { *this = TimeDuration(*this) - 1; return *this; }

			constexpr Time operator ++ (int) { Time result = *this; ++(*this); return result; }

			constexpr Time operator -- (int) { Time result = *this; --(*this); return result; }

		protected:
			template
			<
				typename HOUR,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<HOUR>, Ash::Type::Requirement::IsClass<HOUR, Hour>>::IsValid
			>
			static constexpr bool isHourValid(HOUR hour)
			{
				return (hour >= Hour::minimum) && (hour <= Hour::maximum);
			}

			template
			<
				typename MINUTE,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MINUTE>, Ash::Type::Requirement::IsClass<MINUTE, Minute>>::IsValid
			>
			static constexpr bool isMinuteValid(MINUTE minute)
			{
				return (minute >= Minute::minimum) && (minute <= Minute::maximum);
			}

			template
			<
				typename SECOND,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<SECOND>, Ash::Type::Requirement::IsClass<SECOND, Second>>::IsValid
			>
			static constexpr bool isSecondValid(SECOND second)
			{
				return (second >= Second::minimum) && (second <= Second::maximum);
			}

		private:
			Hour   m_Hour;
			Minute m_Minute;
			Second m_Second;
		};
	}
}
