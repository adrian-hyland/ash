#pragma once

#include "ash.test.h"
#include "ash.calendar.time.h"


namespace Ash
{
	namespace Test
	{
		namespace Calendar
		{
			constexpr Ash::Test::Assertion hour()
			{
				TEST_IS_EQ(Ash::Calendar::Hour::hoursPerDay, 24);
				TEST_IS_EQ(Ash::Calendar::Hour::minValue, 0);
				TEST_IS_EQ(Ash::Calendar::Hour::maxValue, 23);

				Ash::Calendar::Hour expected = 0;
				for (Ash::Calendar::TimeDuration hour = -Ash::Calendar::Hour::hoursPerDay; hour <= Ash::Calendar::Hour::hoursPerDay; hour++)
				{
					TEST_IS_EQ(Ash::Calendar::Hour(hour), expected++);
				}

				return {};
			}

			constexpr Ash::Test::Assertion minute()
			{
				TEST_IS_EQ(Ash::Calendar::Minute::minutesPerHour, 60);
				TEST_IS_EQ(Ash::Calendar::Minute::minValue, 0);
				TEST_IS_EQ(Ash::Calendar::Minute::maxValue, 59);

				Ash::Calendar::Minute expected = 0;
				for (Ash::Calendar::TimeDuration minute = -Ash::Calendar::Minute::minutesPerHour; minute <= Ash::Calendar::Minute::minutesPerHour; minute++)
				{
					TEST_IS_EQ(Ash::Calendar::Minute(minute), expected++);
				}

				return {};
			}

			constexpr Ash::Test::Assertion second()
			{
				TEST_IS_EQ(Ash::Calendar::Second::secondsPerMinute, 60);
				TEST_IS_EQ(Ash::Calendar::Second::minValue, 0);
				TEST_IS_EQ(Ash::Calendar::Second::maxValue, 59);

				Ash::Calendar::Second expected = 0;
				for (Ash::Calendar::TimeDuration second = -Ash::Calendar::Second::secondsPerMinute; second <= Ash::Calendar::Second::secondsPerMinute; second++)
				{
					TEST_IS_EQ(Ash::Calendar::Second(second), expected++);
				}

				return {};
			}

			constexpr Ash::Test::Assertion time()
			{
				TEST_IS_EQ(Ash::Calendar::Time::seconds, 1);
				TEST_IS_EQ(Ash::Calendar::Time::minutes, 60);
				TEST_IS_EQ(Ash::Calendar::Time::hours, 60 * 60);
				TEST_IS_EQ(Ash::Calendar::Time::days, 24 * 60 * 60);
				TEST_IS_EQ(Ash::Calendar::Time::midnight, 0);
				TEST_IS_EQ(Ash::Calendar::Time::midday, 12 * 60 * 60);

				Ash::Calendar::Hour expectedHour = 0;
				Ash::Calendar::Minute expectedMinute = 0;
				Ash::Calendar::Second expectedSecond = 0;
				for (Ash::Calendar::TimeDuration time = -Ash::Calendar::Time::days; time <= Ash::Calendar::Time::days; time++)
				{
					TEST_IS_EQ(Ash::Calendar::Time(time).getHour(), expectedHour);
					TEST_IS_EQ(Ash::Calendar::Time(time).getMinute(), expectedMinute);
					TEST_IS_EQ(Ash::Calendar::Time(time).getSecond(), expectedSecond);
					if (++expectedSecond == 0)
					{
						if (++expectedMinute == 0)
						{
							++expectedHour;
						}
					}
				}

				return {};
			}

			constexpr Ash::Test::Assertion getHour()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::getRange())
				{
					Ash::Calendar::Time time = Ash::Calendar::Time::hours * hour;
					TEST_IS_EQ(time.getHour(), hour);
				}

				return {};
			}

			constexpr Ash::Test::Assertion getMinute()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::getRange())
				{
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::getRange())
					{
						Ash::Calendar::Time time = Ash::Calendar::Time::hours * hour + Ash::Calendar::Time::minutes * minute;
						TEST_IS_EQ(time.getHour(), hour);
						TEST_IS_EQ(time.getMinute(), minute);
					}
				}

				return {};
			}

			constexpr Ash::Test::Assertion getSecond()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::getRange())
				{
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::getRange())
					{
						for (Ash::Calendar::Second second : Ash::Calendar::Second::getRange())
						{
							Ash::Calendar::Time time = Ash::Calendar::Time::hours * hour + Ash::Calendar::Time::minutes * minute + Ash::Calendar::Time::seconds * second;
							TEST_IS_EQ(time.getHour(), hour);
							TEST_IS_EQ(time.getMinute(), minute);
							TEST_IS_EQ(time.getSecond(), second);
						}
					}
				}

				return {};
			}

			constexpr Ash::Test::Assertion isMorning()
			{
				TEST_IS_TRUE(Ash::Calendar::Time(0, 0, 0).isMorning());
				TEST_IS_TRUE(Ash::Calendar::Time(11, 59, 59).isMorning());
				TEST_IS_FALSE(Ash::Calendar::Time(12, 0, 0).isMorning());
				TEST_IS_FALSE(Ash::Calendar::Time(23, 59, 59).isMorning());
				return {};
			}

			constexpr Ash::Test::Assertion isAfternoon()
			{
				TEST_IS_FALSE(Ash::Calendar::Time(0, 0, 0).isAfternoon());
				TEST_IS_FALSE(Ash::Calendar::Time(11, 59, 59).isAfternoon());
				TEST_IS_TRUE(Ash::Calendar::Time(12, 0, 0).isAfternoon());
				TEST_IS_TRUE(Ash::Calendar::Time(23, 59, 59).isAfternoon());
				return {};
			}
		}

		TEST_UNIT
		(
			testTime,
			TEST_CASE(Ash::Test::Calendar::hour),
			TEST_CASE(Ash::Test::Calendar::minute),
			TEST_CASE(Ash::Test::Calendar::second),
			TEST_CASE(Ash::Test::Calendar::time),
			TEST_CASE(Ash::Test::Calendar::getHour),
			TEST_CASE(Ash::Test::Calendar::getMinute),
			TEST_CASE(Ash::Test::Calendar::getSecond),
			TEST_CASE(Ash::Test::Calendar::isMorning),
			TEST_CASE(Ash::Test::Calendar::isAfternoon),
		);
	}
}
