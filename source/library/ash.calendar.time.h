#pragma once

#include <cstdint>
#include "ash.integer.h"


namespace Ash
{
	namespace Calendar
	{
		using TimeDuration = int32_t;


		class Hour : public Ash::Integer::Cycle<uint8_t, 0, 23>
		{
		public:
			using Cycle = Ash::Integer::Cycle<uint8_t, 0, 23>;

			static constexpr Type perDay = Cycle::size;

			constexpr Hour() : Cycle() {}

			template <typename VALUE>
			constexpr Hour(VALUE value) : Cycle(value) {}
		};


		class Minute : public Ash::Integer::Cycle<uint8_t, 0, 59>
		{
		public:
			using Cycle = Ash::Integer::Cycle<uint8_t, 0, 59>;

			static constexpr Type perHour = Cycle::size;

			constexpr Minute() : Cycle() {}

			template <typename VALUE>
			constexpr Minute(VALUE value) : Cycle(value) {}
		};


		class Second : public Ash::Integer::Cycle<uint8_t, 0, 59>
		{
		public:
			using Cycle = Ash::Integer::Cycle<uint8_t, 0, 59>;

			static constexpr Type perMinute = Cycle::size;

			constexpr Second() : Cycle() {}

			template <typename VALUE>
			constexpr Second(VALUE value) : Cycle(value) {}
		};


		class Time
		{
		public:
			static constexpr TimeDuration second = 1;

			static constexpr TimeDuration minute = second * Second::perMinute;

			static constexpr TimeDuration hour = minute * Minute::perHour;

			static constexpr TimeDuration day = hour * Hour::perDay;

			static constexpr TimeDuration midnight = 0;

			static constexpr TimeDuration midday = day / 2;

			constexpr Time() : m_Hour(), m_Minute(), m_Second() {}

			template <typename VALUE>
			constexpr Time(VALUE duration) : Time()
			{
				set<VALUE>(duration);
			}

			constexpr Time(Hour hour, Minute minute, Second second) : m_Hour(hour), m_Minute(minute), m_Second(second) {}

			constexpr operator TimeDuration () const { return TimeDuration(m_Hour) * hour + TimeDuration(m_Minute) * minute + TimeDuration(m_Second) * second; }

			constexpr Hour getHour() const { return m_Hour; }

			constexpr Minute getMinute() const { return m_Minute; }

			constexpr Second getSecond() const { return m_Second; }

			constexpr bool isMorning() const { return m_Hour < Time(midday).getHour(); }

			constexpr bool isAfternoon() const { return !isMorning(); }

			constexpr Time &operator ++ () { *this = *this + 1; return *this; }

			constexpr Time &operator -- () { *this = *this - 1; return *this; }

			constexpr Time operator ++ (int) { Time result = *this; ++(*this); return result; }

			constexpr Time operator -- (int) { Time result = *this; --(*this); return result; }

		protected:
			template <typename VALUE>
			void set(Ash::Integer::Cycle<VALUE, day - 1> value)
			{
				m_Hour = value / hour;
				m_Minute = value / minute;
				m_Second = value / second;
			}

		private:
			Hour   m_Hour;
			Minute m_Minute;
			Second m_Second;
		};
	}
}
