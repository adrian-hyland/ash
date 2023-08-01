#pragma once

#include "ash.test.h"
#include "ash.calendar.datetime.h"


namespace Ash
{
	namespace Test
	{
		namespace Calendar
		{
			constexpr Ash::Test::Assertion dateTime()
			{
				for (Ash::Calendar::TimeDuration dstOffset = 0; dstOffset <= Ash::Calendar::DateTime::daylightSaving; dstOffset = dstOffset + Ash::Calendar::Time::hour)
				{
					for (Ash::Calendar::TimeDuration utcOffset = -12 * Ash::Calendar::Time::hour; utcOffset <= 12 * Ash::Calendar::Time::hour; utcOffset = utcOffset + Ash::Calendar::Time::hour)
					{
						Ash::Calendar::Date utcDate;

						TEST_IS_TRUE(utcDate.setYearMonthDay(-Ash::Calendar::GregorianCycle::yearsPerCycle, Ash::Calendar::Month::January, 1));

						for (Ash::Calendar::Date date = utcDate; date < -utcDate; date++)
						{
							Ash::Calendar::DateTime dateTime(date, Ash::Calendar::Time::midnight, utcOffset, dstOffset);

							TEST_IS_EQ(dateTime.getLocalDate(), date - ((utcOffset + dstOffset < 0) ? 1 : 0));
							TEST_IS_EQ(dateTime.getLocalTime(), Ash::Calendar::Time(utcOffset + dstOffset));

							TEST_IS_EQ(dateTime.getUtcDate(), date);
							TEST_IS_EQ(dateTime.getUtcTime(), Ash::Calendar::Time::midnight);
						}

						TEST_IS_TRUE(utcDate.setYearMonthDay(-Ash::Calendar::GregorianCycle::yearsPerCycle, Ash::Calendar::Month::December, 31));

						for (Ash::Calendar::Date date = utcDate; date < -utcDate; date++)
						{
							Ash::Calendar::DateTime dateTime(date, Ash::Calendar::Time(Ash::Calendar::Time::midnight - 1), utcOffset, dstOffset);

							TEST_IS_EQ(dateTime.getLocalDate(), date + ((utcOffset + dstOffset > 0) ? 1 : 0));
							TEST_IS_EQ(dateTime.getLocalTime(), Ash::Calendar::Time(utcOffset + dstOffset - 1));

							TEST_IS_EQ(dateTime.getUtcDate(), date);
							TEST_IS_EQ(dateTime.getUtcTime(), Ash::Calendar::Time(Ash::Calendar::Time::midnight - 1));
						}
					}
				}

				return {};
			}
		}

		TEST_UNIT
		(
			testDateTime,
			TEST_CASE(Ash::Test::Calendar::dateTime),
		);
	}
}
