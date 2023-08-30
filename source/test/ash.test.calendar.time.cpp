#include "ash.calendar.time.h"
#include "ash.test.calendar.time.h"


namespace Ash
{
	namespace Test
	{
		namespace Calendar
		{
			static Ash::Test::Assertion hour()
			{
				TEST_IS_EQ(Ash::Calendar::Hour::perDay, 24);
				TEST_IS_EQ(Ash::Calendar::Hour::minValue, 0);
				TEST_IS_EQ(Ash::Calendar::Hour::maxValue, 23);

				Ash::Calendar::Hour expected = 0;
				for (Ash::Calendar::TimeDuration hour = -Ash::Calendar::Hour::perDay; hour <= Ash::Calendar::Hour::perDay; hour++)
				{
					TEST_IS_EQ(Ash::Calendar::Hour(hour), expected++);
				}

				return {};
			}

			static Ash::Test::Assertion minute()
			{
				TEST_IS_EQ(Ash::Calendar::Minute::perHour, 60);
				TEST_IS_EQ(Ash::Calendar::Minute::minValue, 0);
				TEST_IS_EQ(Ash::Calendar::Minute::maxValue, 59);

				Ash::Calendar::Minute expected = 0;
				for (Ash::Calendar::TimeDuration minute = -Ash::Calendar::Minute::perHour; minute <= Ash::Calendar::Minute::perHour; minute++)
				{
					TEST_IS_EQ(Ash::Calendar::Minute(minute), expected++);
				}

				return {};
			}

			static Ash::Test::Assertion second()
			{
				TEST_IS_EQ(Ash::Calendar::Second::perMinute, 60);
				TEST_IS_EQ(Ash::Calendar::Second::minValue, 0);
				TEST_IS_EQ(Ash::Calendar::Second::maxValue, 59);

				Ash::Calendar::Second expected = 0;
				for (Ash::Calendar::TimeDuration second = -Ash::Calendar::Second::perMinute; second <= Ash::Calendar::Second::perMinute; second++)
				{
					TEST_IS_EQ(Ash::Calendar::Second(second), expected++);
				}

				return {};
			}

			static Ash::Test::Assertion time()
			{
				TEST_IS_EQ(Ash::Calendar::Time::second, 1);
				TEST_IS_EQ(Ash::Calendar::Time::minute, 60);
				TEST_IS_EQ(Ash::Calendar::Time::hour, 60 * 60);
				TEST_IS_EQ(Ash::Calendar::Time::day, 24 * 60 * 60);
				TEST_IS_EQ(Ash::Calendar::Time::midnight, 0);
				TEST_IS_EQ(Ash::Calendar::Time::midday, 12 * 60 * 60);

				Ash::Calendar::Hour expectedHour = 0;
				Ash::Calendar::Minute expectedMinute = 0;
				Ash::Calendar::Second expectedSecond = 0;
				for (Ash::Calendar::TimeDuration time = -Ash::Calendar::Time::day; time <= Ash::Calendar::Time::day; time++)
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

			static Ash::Test::Assertion getHour()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::getRange())
				{
					Ash::Calendar::Time time = Ash::Calendar::Time::hour * hour;
					TEST_IS_EQ(time.getHour(), hour);
				}

				return {};
			}

			static Ash::Test::Assertion getMinute()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::getRange())
				{
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::getRange())
					{
						Ash::Calendar::Time time = Ash::Calendar::Time::hour * hour + Ash::Calendar::Time::minute * minute;
						TEST_IS_EQ(time.getHour(), hour);
						TEST_IS_EQ(time.getMinute(), minute);
					}
				}

				return {};
			}

			static Ash::Test::Assertion getSecond()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::getRange())
				{
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::getRange())
					{
						for (Ash::Calendar::Second second : Ash::Calendar::Second::getRange())
						{
							Ash::Calendar::Time time = Ash::Calendar::Time::hour * hour + Ash::Calendar::Time::minute * minute + Ash::Calendar::Time::second * second;
							TEST_IS_EQ(time.getHour(), hour);
							TEST_IS_EQ(time.getMinute(), minute);
							TEST_IS_EQ(time.getSecond(), second);
						}
					}
				}

				return {};
			}

			static Ash::Test::Assertion isMorning()
			{
				TEST_IS_TRUE(Ash::Calendar::Time(0, 0, 0).isMorning());
				TEST_IS_TRUE(Ash::Calendar::Time(11, 59, 59).isMorning());
				TEST_IS_FALSE(Ash::Calendar::Time(12, 0, 0).isMorning());
				TEST_IS_FALSE(Ash::Calendar::Time(23, 59, 59).isMorning());
				return {};
			}

			static Ash::Test::Assertion isAfternoon()
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
