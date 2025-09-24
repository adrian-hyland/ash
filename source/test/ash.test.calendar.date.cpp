#include <limits>
#include "ash.calendar.date.h"
#include "ash.test.calendar.date.h"


namespace Ash
{
	namespace Test
	{
		namespace Calendar
		{
			static Ash::Test::Assertion weekday()
			{
				TEST_IS_EQ(Ash::Calendar::Weekday::daysPerWeek, 7);

				TEST_IS_EQ(Ash::Calendar::Weekday(1), Ash::Calendar::Weekday::monday);
				TEST_IS_EQ(Ash::Calendar::Weekday(2), Ash::Calendar::Weekday::tuesday);
				TEST_IS_EQ(Ash::Calendar::Weekday(3), Ash::Calendar::Weekday::wednesday);
				TEST_IS_EQ(Ash::Calendar::Weekday(4), Ash::Calendar::Weekday::thursday);
				TEST_IS_EQ(Ash::Calendar::Weekday(5), Ash::Calendar::Weekday::friday);
				TEST_IS_EQ(Ash::Calendar::Weekday(6), Ash::Calendar::Weekday::saturday);
				TEST_IS_EQ(Ash::Calendar::Weekday(7), Ash::Calendar::Weekday::sunday);

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion year()
			{
				using Calendar = CALENDAR;
				using Year = typename Calendar::Year;

				Year year = 0;
				TEST_IS_TRUE(year++.isLeapYear());
				TEST_IS_TRUE((--year).isLeapYear());
				TEST_IS_TRUE(year--.isLeapYear());
				TEST_IS_TRUE((++year).isLeapYear());
				TEST_IS_FALSE((++year).isLeapYear());
				TEST_IS_FALSE(year++.isLeapYear());
				TEST_IS_FALSE((--year).isLeapYear());
				TEST_IS_FALSE(year--.isLeapYear());
				
				year = 4;
				TEST_IS_TRUE(year++.isLeapYear());
				TEST_IS_TRUE((--year).isLeapYear());
				TEST_IS_TRUE(year--.isLeapYear());
				TEST_IS_TRUE((++year).isLeapYear());
				TEST_IS_FALSE((++year).isLeapYear());
				TEST_IS_FALSE(year++.isLeapYear());
				TEST_IS_FALSE((--year).isLeapYear());
				TEST_IS_FALSE(year--.isLeapYear());

				year = 100;
				if constexpr (Ash::Type::isSame<Calendar, Ash::Calendar::Julian>)
				{
					TEST_IS_TRUE(year++.isLeapYear());
					TEST_IS_TRUE((--year).isLeapYear());
					TEST_IS_TRUE(year--.isLeapYear());
					TEST_IS_TRUE((++year).isLeapYear());
				}
				else
				{
					TEST_IS_FALSE(year++.isLeapYear());
					TEST_IS_FALSE((--year).isLeapYear());
					TEST_IS_FALSE(year--.isLeapYear());
					TEST_IS_FALSE((++year).isLeapYear());
				}
				TEST_IS_FALSE((++year).isLeapYear());
				TEST_IS_FALSE(year++.isLeapYear());
				TEST_IS_FALSE((--year).isLeapYear());
				TEST_IS_FALSE(year--.isLeapYear());

				year = 400;
				TEST_IS_TRUE(year++.isLeapYear());
				TEST_IS_TRUE((--year).isLeapYear());
				TEST_IS_TRUE(year--.isLeapYear());
				TEST_IS_TRUE((++year).isLeapYear());
				TEST_IS_FALSE((++year).isLeapYear());
				TEST_IS_FALSE(year++.isLeapYear());
				TEST_IS_FALSE((--year).isLeapYear());
				TEST_IS_FALSE(year--.isLeapYear());

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion month()
			{
				using Calendar = CALENDAR;
				using Year = typename Calendar::Year;

				TEST_IS_EQ(Ash::Calendar::Month::monthsPerYear, 12);

				TEST_IS_EQ(Ash::Calendar::Month(1), Ash::Calendar::Month::january);
				TEST_IS_EQ(Ash::Calendar::Month(2), Ash::Calendar::Month::february);
				TEST_IS_EQ(Ash::Calendar::Month(3), Ash::Calendar::Month::march);
				TEST_IS_EQ(Ash::Calendar::Month(4), Ash::Calendar::Month::april);
				TEST_IS_EQ(Ash::Calendar::Month(5), Ash::Calendar::Month::may);
				TEST_IS_EQ(Ash::Calendar::Month(6), Ash::Calendar::Month::june);
				TEST_IS_EQ(Ash::Calendar::Month(7), Ash::Calendar::Month::july);
				TEST_IS_EQ(Ash::Calendar::Month(8), Ash::Calendar::Month::august);
				TEST_IS_EQ(Ash::Calendar::Month(9), Ash::Calendar::Month::september);
				TEST_IS_EQ(Ash::Calendar::Month(10), Ash::Calendar::Month::october);
				TEST_IS_EQ(Ash::Calendar::Month(11), Ash::Calendar::Month::november);
				TEST_IS_EQ(Ash::Calendar::Month(12), Ash::Calendar::Month::december);

				for (Year year = 0; year < Year::yearsPerCycle; year = year + Year::cycleStep)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterateBetween(Ash::Calendar::Month::january, Ash::Calendar::Month::november))
					{
						TEST_IS_EQ(month.getFirstDayOrdinal(year) + month.getDays(year), Ash::Calendar::Month(month + 1).getFirstDayOrdinal(year));
					}

					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterateBetween(Ash::Calendar::Month::february, Ash::Calendar::Month::december))
					{
						TEST_IS_EQ(month.getLastDayOrdinal(year) - month.getDays(year), Ash::Calendar::Month(month - 1).getLastDayOrdinal(year));
					}

					Ash::Calendar::Ordinal ordinal = 1;
					for (Ash::Calendar::Month expectedMonth : Ash::Calendar::Month::iterate())
					{
						for (Ash::Calendar::Day expectedDay = 1; expectedDay <= expectedMonth.getDays(year); expectedDay++)
						{
							Ash::Calendar::Day day = 1;
							Ash::Calendar::Month month = Ash::Calendar::Month::getMonthDay(year, ordinal, day);
							TEST_IS_EQ(month, expectedMonth);
							TEST_IS_EQ(day, expectedDay);
							ordinal++;
						}
					}
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion date()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Ash::Calendar::DateDuration expected = Year::epoch;
				Date date(expected);

				for (Year year = 1; year < Year::maximum; year++)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						for (Ash::Calendar::Day day = 1; day <= month.getDays(year); day++)
						{
							expected++;
							date++;

							TEST_IS_EQ(date, expected);
							TEST_IS_EQ(date.getDay(), day);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getYear(), year);
						}
					}
				}

				expected = Year::epoch;
				date = expected;

				for (Year year = 0; year > Year::minimum; year--)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate().reverse())
					{
						for (Ash::Calendar::Day day = Ash::Calendar::Month(month).getDays(year); day >= 1; day--)
						{
							TEST_IS_EQ(date, expected);
							TEST_IS_EQ(date.getDay(), day);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getYear(), year);

							expected--;
							date--;
						}
					}
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion isValid()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;

				Ash::Calendar::DateDuration days = Date::minimum;
				TEST_IS_TRUE(Date::isValid(days));
				days--;
				TEST_IS_FALSE(Date::isValid(days));

				days = Date::maximum;
				TEST_IS_TRUE(Date::isValid(days));
				days++;
				TEST_IS_FALSE(Date::isValid(days));

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion isYearValid()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				TEST_IS_TRUE(Date::isYearValid(Year::minimum));
				TEST_IS_TRUE(Date::isYearValid(Year::maximum));

				TEST_IS_FALSE(Date::isYearValid(int32_t(Year::minimum) - 1));
				TEST_IS_FALSE(Date::isYearValid(int32_t(Year::maximum) + 1));

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion isMonthValid()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;

				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::january));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::february));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::march));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::april));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::may));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::june));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::july));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::august));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::september));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::october));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::november));
				TEST_IS_TRUE(Date::isMonthValid(Ash::Calendar::Month::december));

				TEST_IS_FALSE(Date::isMonthValid(0));
				TEST_IS_FALSE(Date::isMonthValid(-1));
				TEST_IS_FALSE(Date::isMonthValid(13));

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion isWeekdayValid()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;

				TEST_IS_TRUE(Date::isWeekdayValid(Ash::Calendar::Weekday::monday));
				TEST_IS_TRUE(Date::isWeekdayValid(Ash::Calendar::Weekday::tuesday));
				TEST_IS_TRUE(Date::isWeekdayValid(Ash::Calendar::Weekday::wednesday));
				TEST_IS_TRUE(Date::isWeekdayValid(Ash::Calendar::Weekday::thursday));
				TEST_IS_TRUE(Date::isWeekdayValid(Ash::Calendar::Weekday::friday));
				TEST_IS_TRUE(Date::isWeekdayValid(Ash::Calendar::Weekday::saturday));
				TEST_IS_TRUE(Date::isWeekdayValid(Ash::Calendar::Weekday::sunday));

				TEST_IS_FALSE(Date::isWeekdayValid(0));
				TEST_IS_FALSE(Date::isWeekdayValid(-1));
				TEST_IS_FALSE(Date::isWeekdayValid(8));

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion isYearOrdinalValid()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_FALSE(Date::isYearOrdinalValid(year, 0));
					for (Ash::Calendar::Ordinal ordinal = 1; ordinal <= year.getDays(); ordinal++)
					{
						TEST_IS_TRUE(Date::isYearOrdinalValid(year, ordinal));
					}
					TEST_IS_FALSE(Date::isYearOrdinalValid(year, year.getDays() + 1));
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion isYearMonthDayValid()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						TEST_IS_FALSE(Date::isYearMonthDayValid(year, month, 0));
						for (Ash::Calendar::Day day = 1; day <= month.getDays(year); day++)
						{
							TEST_IS_TRUE(Date::isYearMonthDayValid(year, month, day));
						}
						TEST_IS_FALSE(Date::isYearMonthDayValid(year, month, month.getDays(year) + 1));
					}
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion isYearWeekValid()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_FALSE(Date::isYearWeekValid(year, 0));
					for (Ash::Calendar::Week week = 1; week <= year.getWeeks(); week++)
					{
						TEST_IS_TRUE(Date::isYearWeekValid(year, week));
					}
					TEST_IS_FALSE(Date::isYearWeekValid(year, year.getWeeks() + 1));
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion set()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;

				Date date;
				TEST_IS_EQ(date.set(Date::minimum), Ash::Error::none);
				TEST_IS_EQ(date.set(Date::maximum), Ash::Error::none);

				TEST_IS_EQ(date.set(Date::minimum - 1), Ash::Calendar::Error::invalidDuration);
				TEST_IS_EQ(date.set(Date::maximum + 1), Ash::Calendar::Error::invalidDuration);

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion setYearOrdinal()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				TEST_IS_EQ(date.setYearOrdinal(Year::minimum - 1, 1), Ash::Calendar::Error::invalidYear);
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_EQ(date.setYearOrdinal(year, 0), Ash::Calendar::Error::invalidYearOrdinal);
					for (Ash::Calendar::Ordinal ordinal = 1; ordinal <= year.getDays(); ordinal++)
					{
						TEST_IS_EQ(date.setYearOrdinal(year, ordinal), Ash::Error::none);
						TEST_IS_EQ(date, expected++);
					}
					TEST_IS_EQ(date.setYearOrdinal(year, year.getDays() + 1), Ash::Calendar::Error::invalidYearOrdinal);
				}
				TEST_IS_EQ(date.setYearOrdinal(Year::maximum + 1, 1), Ash::Calendar::Error::invalidYear);

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion setYearMonthDay()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				TEST_IS_EQ(date.setYearMonthDay(Year::minimum - 1, Ash::Calendar::Month::january, 1), Ash::Calendar::Error::invalidYear);
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_EQ(date.setYearMonthDay(year, 0, 1), Ash::Calendar::Error::invalidMonth);
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						TEST_IS_EQ(date.setYearMonthDay(year, month, 0), Ash::Calendar::Error::invalidYearMonthDay);
						for (Ash::Calendar::Day day = 1; day <= month.getDays(year); day++)
						{
							TEST_IS_EQ(date.setYearMonthDay(year, month, day), Ash::Error::none);
							TEST_IS_EQ(date, expected++);
						}
						TEST_IS_EQ(date.setYearMonthDay(year, month, month.getDays(year) + 1), Ash::Calendar::Error::invalidYearMonthDay);
					}
					TEST_IS_EQ(date.setYearMonthDay(year, 13, 1), Ash::Calendar::Error::invalidMonth);
				}
				TEST_IS_EQ(date.setYearMonthDay(Year::maximum + 1, Ash::Calendar::Month::january, 1), Ash::Calendar::Error::invalidYear);

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion setYearWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				TEST_IS_EQ(date.setYearWeekday(Year::minimum - 1, 1, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYear);
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_EQ(date.setYearWeekday(year, 0, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYearWeek);
					for (Ash::Calendar::Week week = 1; week <= year.getWeeks(); week++)
					{
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							TEST_IS_EQ(date.setYearWeekday(year, week, weekday), Ash::Error::none);
							TEST_IS_EQ(date, expected++);
						}
					}
					TEST_IS_EQ(date.setYearWeekday(year, year.getWeeks() + 1, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYearWeek);
				}
				TEST_IS_EQ(date.setYearWeekday(Year::maximum + 1, 1, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYear);

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion setYearMonthFirstWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;

				TEST_IS_EQ(date.setYearMonthFirstWeekday(Year::minimum - 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYear);
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_EQ(date.setYearMonthFirstWeekday(year, 0, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidMonth);
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						TEST_IS_EQ(date.setYearMonthFirstWeekday(year, month, 0), Ash::Calendar::Error::invalidWeekday);
						Date start;
						TEST_IS_EQ(start.setYearMonthDay(year, month, 1), Ash::Error::none);
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							TEST_IS_EQ(date.setYearMonthFirstWeekday(year, month, weekday), Ash::Error::none);
							TEST_IS_EQ(date.getYear(), year);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getWeekday(), weekday);
							TEST_IS_EQ(date - start + 1, Ash::Calendar::Weekday(1 + weekday - start.getWeekday()));
						}
						TEST_IS_EQ(date.setYearMonthFirstWeekday(year, month, 8), Ash::Calendar::Error::invalidWeekday);
					}
					TEST_IS_EQ(date.setYearMonthFirstWeekday(year, 13, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidMonth);
				}
				TEST_IS_EQ(date.setYearMonthFirstWeekday(Year::maximum + 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYear);

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion setYearMonthLastWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;

				TEST_IS_EQ(date.setYearMonthLastWeekday(Year::minimum - 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYear);
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_EQ(date.setYearMonthLastWeekday(year, 0, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidMonth);
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						TEST_IS_EQ(date.setYearMonthLastWeekday(year, month, 0), Ash::Calendar::Error::invalidWeekday);
						Date end;
						TEST_IS_EQ(end.setYearMonthDay(year, month, month.getDays(year)), Ash::Error::none);
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							TEST_IS_EQ(date.setYearMonthLastWeekday(year, month, weekday), Ash::Error::none);
							TEST_IS_EQ(date.getYear(), year);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getWeekday(), weekday);
							TEST_IS_EQ(end - date + 1, Ash::Calendar::Weekday(1 + end.getWeekday() - weekday));
						}
						TEST_IS_EQ(date.setYearMonthLastWeekday(year, month, 8), Ash::Calendar::Error::invalidWeekday);
					}
					TEST_IS_EQ(date.setYearMonthLastWeekday(year, 13, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidMonth);
				}
				TEST_IS_EQ(date.setYearMonthLastWeekday(Year::maximum + 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday), Ash::Calendar::Error::invalidYear);

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion withYearOrdinal()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				try
				{
					date = Date::withYearOrdinal(Year::minimum - 1, 1);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					try
					{
						date = Date::withYearOrdinal(year, 0);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYearOrdinal);
					}
					for (Ash::Calendar::Ordinal ordinal = 1; ordinal <= year.getDays(); ordinal++)
					{
						date = Date::withYearOrdinal(year, ordinal);
						TEST_IS_EQ(date, expected++);
					}
					try
					{
						date = Date::withYearOrdinal(year, year.getDays() + 1);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYearOrdinal);
					}
				}
				try
				{
					date = Date::withYearOrdinal(Year::maximum + 1, 1);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion withYearMonthDay()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				try
				{
					date = Date::withYearMonthDay(Year::minimum - 1, Ash::Calendar::Month::january, 1);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					try
					{
						date = Date::withYearMonthDay(year, 0, 1);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMonth);
					}
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						try
						{
							date = Date::withYearMonthDay(year, month, 0);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYearMonthDay);
						}
						for (Ash::Calendar::Day day = 1; day <= month.getDays(year); day++)
						{
							date = Date::withYearMonthDay(year, month, day);
							TEST_IS_EQ(date, expected++);
						}
						try
						{
							date = Date::withYearMonthDay(year, month, month.getDays(year) + 1);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYearMonthDay);
						}
					}
					try
					{
						date = Date::withYearMonthDay(year, 13, 1);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMonth);
					}
				}
				try
				{
					date = Date::withYearMonthDay(Year::maximum + 1, Ash::Calendar::Month::january, 1);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion withYearWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				try
				{
					date = Date::withYearWeekday(Year::minimum - 1, 1, Ash::Calendar::Weekday::monday);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					try
					{
						date = Date::withYearWeekday(year, 0, Ash::Calendar::Weekday::monday);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYearWeek);
					}
					for (Ash::Calendar::Week week = 1; week <= year.getWeeks(); week++)
					{
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							date = Date::withYearWeekday(year, week, weekday);
							TEST_IS_EQ(date, expected++);
						}
					}
					try
					{
						date = Date::withYearWeekday(year, year.getWeeks() + 1, Ash::Calendar::Weekday::monday);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYearWeek);
					}
				}
				try
				{
					date = Date::withYearWeekday(Year::maximum + 1, 1, Ash::Calendar::Weekday::monday);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion withYearMonthFirstWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;

				try
				{
					date = Date::withYearMonthFirstWeekday(Year::minimum - 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					try
					{
						date = Date::withYearMonthFirstWeekday(year, 0, Ash::Calendar::Weekday::monday);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMonth);
					}
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						try
						{
							date = Date::withYearMonthFirstWeekday(year, month, 0);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidWeekday);
						}
						Date start = Date::withYearMonthDay(year, month, 1);
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							date = Date::withYearMonthFirstWeekday(year, month, weekday);
							TEST_IS_EQ(date.getYear(), year);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getWeekday(), weekday);
							TEST_IS_EQ(date - start + 1, Ash::Calendar::Weekday(1 + weekday - start.getWeekday()));
						}
						try
						{
							date = Date::withYearMonthFirstWeekday(year, month, 8);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidWeekday);
						}
					}
					try
					{
						date = Date::withYearMonthFirstWeekday(year, 13, Ash::Calendar::Weekday::monday);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMonth);
					}
				}
				try
				{
					date = Date::withYearMonthFirstWeekday(Year::maximum + 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion withYearMonthLastWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;

				try
				{
					date = Date::withYearMonthLastWeekday(Year::minimum - 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}
				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					try
					{
						date = Date::withYearMonthLastWeekday(year, 0, Ash::Calendar::Weekday::monday);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMonth);
					}
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						try
						{
							date = Date::withYearMonthLastWeekday(year, month, 0);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidWeekday);
						}
						Date end = Date::withYearMonthDay(year, month, month.getDays(year));
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							date = Date::withYearMonthLastWeekday(year, month, weekday);
							TEST_IS_EQ(date.getYear(), year);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getWeekday(), weekday);
							TEST_IS_EQ(end - date + 1, Ash::Calendar::Weekday(1 + end.getWeekday() - weekday));
						}
						try
						{
							date = Date::withYearMonthLastWeekday(year, month, 8);
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidWeekday);
						}
					}
					try
					{
						date = Date::withYearMonthLastWeekday(year, 13, Ash::Calendar::Weekday::monday);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidMonth);
					}
				}
				try
				{
					date = Date::withYearMonthLastWeekday(Year::maximum + 1, Ash::Calendar::Month::january, Ash::Calendar::Weekday::monday);
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Calendar::Error::invalidYear);
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion getYear()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date = Year::epoch - Year::daysPerCycle + 1;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_EQ(date.getYear(), year);
					date = date + year.getDays();
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion getMonth()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date = Year::epoch - Year::daysPerCycle + 1;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						TEST_IS_EQ(date.getMonth(), month);
						date = date + month.getDays(year);
					}
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion getDay()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date = Year::epoch - Year::daysPerCycle + 1;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						for (Ash::Calendar::Day day = 1; day <= month.getDays(year); day++)
						{
							TEST_IS_EQ(date++.getDay(), day);
						}
					}
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion getOrdinal()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date = Year::epoch - Year::daysPerCycle;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					for (Ash::Calendar::Ordinal ordinal = 1; ordinal <= year.getDays(); ordinal++)
					{
						++date;
						TEST_IS_EQ(date.getOrdinal(), ordinal);
					}
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion getWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date = Year::epoch - Year::daysPerCycle;
				Ash::Calendar::Weekday weekday = date.getWeekday();
				
				while (date < Year::epoch + Year::daysPerCycle)
				{
					date++;
					weekday++;
					TEST_IS_EQ(date.getWeekday(), weekday);
				}

				return {};
			}

			template
			<
				typename CALENDAR,
				typename = Ash::Type::IsClass<CALENDAR, Ash::Calendar::Generic::Proleptic>
			>
			static Ash::Test::Assertion getYearWeek()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date = 1;
				Year year = 1;
				Ash::Calendar::Week week = 1;

				while (year < std::numeric_limits<typename Year::Type>::max() - 1)
				{
					Year weekYear;
					TEST_IS_EQ(date.getYearWeek(weekYear), week);
					TEST_IS_EQ(weekYear, year);
					date = date + 6;
					TEST_IS_EQ(date.getYearWeek(weekYear), week);
					TEST_IS_EQ(weekYear, year);
					date++;
					week++;
					if (week > year.getWeeks())
					{
						week = 1;
						year++;
					}
				}

				date = 1;
				week = 1;
				year = 1;

				while (year > std::numeric_limits<typename Year::Type>::min() + 1)
				{
					Year weekYear;
					date--;
					week--;
					if (week == 0)
					{
						year--;
						week = year.getWeeks();
					}
					TEST_IS_EQ(date.getYearWeek(weekYear), week);
					TEST_IS_EQ(weekYear, year);
					date = date - 6;
					TEST_IS_EQ(date.getYearWeek(weekYear), week)
					TEST_IS_EQ(weekYear, year);
				}

				return {};
			}
		}

		TEST_UNIT
		(
			testDate,
			TEST_CASE(Ash::Test::Calendar::weekday),

			TEST_CASE_GENERIC(Ash::Test::Calendar::year, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::month, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::date, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isMonthValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isWeekdayValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearOrdinalValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearMonthDayValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearWeekValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::set, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearOrdinal, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthDay, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthFirstWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthLastWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearOrdinal, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearMonthDay, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearMonthFirstWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearMonthLastWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getMonth, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getDay, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getOrdinal, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getYearWeek, Ash::Calendar::Gregorian),

			TEST_CASE_GENERIC(Ash::Test::Calendar::year, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::month, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::date, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isMonthValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isWeekdayValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearOrdinalValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearMonthDayValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearWeekValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::set, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearOrdinal, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthDay, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthFirstWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthLastWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearOrdinal, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearMonthDay, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearMonthFirstWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::withYearMonthLastWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getMonth, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getDay, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getOrdinal, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getYearWeek, Ash::Calendar::Julian)
		);
	}
}
