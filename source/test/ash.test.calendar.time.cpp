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
				TEST_IS_EQ(Ash::Calendar::Hour::minimum, 0);
				TEST_IS_EQ(Ash::Calendar::Hour::maximum, 23);

				Ash::Calendar::Hour expected = 0;
				for (Ash::Calendar::Hour hour : Ash::Iterate<Ash::Calendar::Hour::Type>::between(-Ash::Calendar::Hour::perDay, Ash::Calendar::Hour::perDay))
				{
					TEST_IS_EQ(hour, expected++);
				}

				return {};
			}

			static Ash::Test::Assertion minute()
			{
				TEST_IS_EQ(Ash::Calendar::Minute::perHour, 60);
				TEST_IS_EQ(Ash::Calendar::Minute::minimum, 0);
				TEST_IS_EQ(Ash::Calendar::Minute::maximum, 59);

				Ash::Calendar::Minute expected = 0;
				for (Ash::Calendar::Minute minute : Ash::Iterate<Ash::Calendar::Minute::Type>::between(-Ash::Calendar::Minute::perHour, Ash::Calendar::Minute::perHour))
				{
					TEST_IS_EQ(minute, expected++);
				}

				return {};
			}

			static Ash::Test::Assertion second()
			{
				TEST_IS_EQ(Ash::Calendar::Second::perMinute, 60);
				TEST_IS_EQ(Ash::Calendar::Second::minimum, 0);
				TEST_IS_EQ(Ash::Calendar::Second::maximum, 59);

				Ash::Calendar::Second expected = 0;
				for (Ash::Calendar::Second second : Ash::Iterate<Ash::Calendar::Second::Type>::between(-Ash::Calendar::Second::perMinute, Ash::Calendar::Second::perMinute))
				{
					TEST_IS_EQ(second, expected++);
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
				for (Ash::Calendar::Time time : Ash::Iterate<Ash::Calendar::TimeDuration::Type>::between(-Ash::Calendar::Time::day, Ash::Calendar::Time::day))
				{
					TEST_IS_EQ(time.getHour(), expectedHour);
					TEST_IS_EQ(time.getMinute(), expectedMinute);
					TEST_IS_EQ(time.getSecond(), expectedSecond);
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

			static Ash::Test::Assertion validateHourMinuteSecond()
			{
				TEST_IS_EQ(Ash::Calendar::Time::validateHourMinuteSecond(-1, 0, 0), Ash::Calendar::Error::invalidHour);
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::iterate())
				{
					TEST_IS_EQ(Ash::Calendar::Time::validateHourMinuteSecond(hour, -1, 0), Ash::Calendar::Error::invalidMinute);
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::iterate())
					{
						TEST_IS_EQ(Ash::Calendar::Time::validateHourMinuteSecond(hour, minute, -1), Ash::Calendar::Error::invalidSecond);
						for (Ash::Calendar::Second second : Ash::Calendar::Second::iterate())
						{
							TEST_IS_EQ(Ash::Calendar::Time::validateHourMinuteSecond(hour, minute, second), Ash::Error::none);
						}
						TEST_IS_EQ(Ash::Calendar::Time::validateHourMinuteSecond(hour, minute, 60), Ash::Calendar::Error::invalidSecond);
					}
					TEST_IS_EQ(Ash::Calendar::Time::validateHourMinuteSecond(hour, 60, 0), Ash::Calendar::Error::invalidMinute);
				}
				TEST_IS_EQ(Ash::Calendar::Time::validateHourMinuteSecond(24, 0, 0), Ash::Calendar::Error::invalidHour);

				return {};
			}

			static Ash::Test::Assertion setHourMinuteSecond()
			{
				Ash::Calendar::Time time;
				Ash::Calendar::TimeDuration expected = 0;

				TEST_IS_EQ(time.setHourMinuteSecond(-1, 0, 0), Ash::Calendar::Error::invalidHour);
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::iterate())
				{
					TEST_IS_EQ(time.setHourMinuteSecond(hour, -1, 0), Ash::Calendar::Error::invalidMinute);
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::iterate())
					{
						TEST_IS_EQ(time.setHourMinuteSecond(hour, minute, -1), Ash::Calendar::Error::invalidSecond);
						for (Ash::Calendar::Second second : Ash::Calendar::Second::iterate())
						{
							TEST_IS_EQ(time.setHourMinuteSecond(hour, minute, second), Ash::Error::none);
							TEST_IS_EQ(Ash::Calendar::TimeDuration(time), expected++);
						}
						TEST_IS_EQ(time.setHourMinuteSecond(hour, minute, 60), Ash::Calendar::Error::invalidSecond);
					}
					TEST_IS_EQ(time.setHourMinuteSecond(hour, 60, 0), Ash::Calendar::Error::invalidMinute);
				}
				TEST_IS_EQ(time.setHourMinuteSecond(24, 0, 0), Ash::Calendar::Error::invalidHour);

				return {};
			}

			static Ash::Test::Assertion withHourMinuteSecond()
			{
				Ash::Calendar::Time time;
				Ash::Calendar::TimeDuration expected = 0;

				try
				{
					time = Ash::Calendar::Time::withHourMinuteSecond(-1, 0, 0);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidHour);
				}
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::iterate())
				{
					try
					{
						time = Ash::Calendar::Time::withHourMinuteSecond(hour, -1, 0);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMinute);
					}
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::iterate())
					{
						try
						{
							time = Ash::Calendar::Time::withHourMinuteSecond(hour, minute, -1);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidSecond);
						}
						for (Ash::Calendar::Second second : Ash::Calendar::Second::iterate())
						{
							time = Ash::Calendar::Time::withHourMinuteSecond(hour, minute, second);
							TEST_IS_EQ(Ash::Calendar::TimeDuration(time), expected++);
						}
						try
						{
							time = Ash::Calendar::Time::withHourMinuteSecond(hour, minute, 60);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidSecond);
						}
					}
					try
					{
						time = Ash::Calendar::Time::withHourMinuteSecond(hour, 60, 0);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMinute);
					}
				}
				try
				{
					time = Ash::Calendar::Time::withHourMinuteSecond(24, 0, 0);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidHour);
				}

				return {};
			}

			static Ash::Test::Assertion getHour()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::iterate())
				{
					Ash::Calendar::Time time = Ash::Calendar::Time::hour * hour;
					TEST_IS_EQ(time.getHour(), hour);
				}

				return {};
			}

			static Ash::Test::Assertion getMinute()
			{
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::iterate())
				{
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::iterate())
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
				for (Ash::Calendar::Hour hour : Ash::Calendar::Hour::iterate())
				{
					for (Ash::Calendar::Minute minute : Ash::Calendar::Minute::iterate())
					{
						for (Ash::Calendar::Second second : Ash::Calendar::Second::iterate())
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
				TEST_IS_TRUE(Ash::Calendar::Time::withHourMinuteSecond(0, 0, 0).isMorning());
				TEST_IS_TRUE(Ash::Calendar::Time::withHourMinuteSecond(11, 59, 59).isMorning());
				TEST_IS_FALSE(Ash::Calendar::Time::withHourMinuteSecond(12, 0, 0).isMorning());
				TEST_IS_FALSE(Ash::Calendar::Time::withHourMinuteSecond(23, 59, 59).isMorning());
				return {};
			}

			static Ash::Test::Assertion isAfternoon()
			{
				TEST_IS_FALSE(Ash::Calendar::Time::withHourMinuteSecond(0, 0, 0).isAfternoon());
				TEST_IS_FALSE(Ash::Calendar::Time::withHourMinuteSecond(11, 59, 59).isAfternoon());
				TEST_IS_TRUE(Ash::Calendar::Time::withHourMinuteSecond(12, 0, 0).isAfternoon());
				TEST_IS_TRUE(Ash::Calendar::Time::withHourMinuteSecond(23, 59, 59).isAfternoon());
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
			TEST_CASE(Ash::Test::Calendar::validateHourMinuteSecond),
			TEST_CASE(Ash::Test::Calendar::setHourMinuteSecond),
			TEST_CASE(Ash::Test::Calendar::withHourMinuteSecond),
			TEST_CASE(Ash::Test::Calendar::getHour),
			TEST_CASE(Ash::Test::Calendar::getMinute),
			TEST_CASE(Ash::Test::Calendar::getSecond),
			TEST_CASE(Ash::Test::Calendar::isMorning),
			TEST_CASE(Ash::Test::Calendar::isAfternoon)
		);
	}
}
