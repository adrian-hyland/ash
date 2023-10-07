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

				TEST_IS_EQ(Ash::Calendar::Weekday(1), Ash::Calendar::Weekday::Monday);
				TEST_IS_EQ(Ash::Calendar::Weekday(2), Ash::Calendar::Weekday::Tuesday);
				TEST_IS_EQ(Ash::Calendar::Weekday(3), Ash::Calendar::Weekday::Wednesday);
				TEST_IS_EQ(Ash::Calendar::Weekday(4), Ash::Calendar::Weekday::Thursday);
				TEST_IS_EQ(Ash::Calendar::Weekday(5), Ash::Calendar::Weekday::Friday);
				TEST_IS_EQ(Ash::Calendar::Weekday(6), Ash::Calendar::Weekday::Saturday);
				TEST_IS_EQ(Ash::Calendar::Weekday(7), Ash::Calendar::Weekday::Sunday);

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

				TEST_IS_EQ(Ash::Calendar::Month(1), Ash::Calendar::Month::January);
				TEST_IS_EQ(Ash::Calendar::Month(2), Ash::Calendar::Month::February);
				TEST_IS_EQ(Ash::Calendar::Month(3), Ash::Calendar::Month::March);
				TEST_IS_EQ(Ash::Calendar::Month(4), Ash::Calendar::Month::April);
				TEST_IS_EQ(Ash::Calendar::Month(5), Ash::Calendar::Month::May);
				TEST_IS_EQ(Ash::Calendar::Month(6), Ash::Calendar::Month::June);
				TEST_IS_EQ(Ash::Calendar::Month(7), Ash::Calendar::Month::July);
				TEST_IS_EQ(Ash::Calendar::Month(8), Ash::Calendar::Month::August);
				TEST_IS_EQ(Ash::Calendar::Month(9), Ash::Calendar::Month::September);
				TEST_IS_EQ(Ash::Calendar::Month(10), Ash::Calendar::Month::October);
				TEST_IS_EQ(Ash::Calendar::Month(11), Ash::Calendar::Month::November);
				TEST_IS_EQ(Ash::Calendar::Month(12), Ash::Calendar::Month::December);

				for (Year year = 0; year < Year::yearsPerCycle; year = year + Year::cycleStep)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterateBetween(Ash::Calendar::Month::January, Ash::Calendar::Month::November))
					{
						TEST_IS_EQ(month.getFirstDayOrdinal(year) + month.getDays(year), Ash::Calendar::Month(month + 1).getFirstDayOrdinal(year));
					}

					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterateBetween(Ash::Calendar::Month::February, Ash::Calendar::Month::December))
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

				for (Year year = 1; year < std::numeric_limits<typename Year::Type>::max(); year++)
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

				for (Year year = 0; year > std::numeric_limits<typename Year::Type>::min(); year--)
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
			static Ash::Test::Assertion setYearOrdinal()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_FALSE(date.setYearOrdinal(year, 0));
					for (Ash::Calendar::Ordinal ordinal = 1; ordinal <= year.getDays(); ordinal++)
					{
						TEST_IS_TRUE(date.setYearOrdinal(year, ordinal));
						TEST_IS_EQ(date, expected++);
					}
					TEST_IS_FALSE(date.setYearOrdinal(year, year.getDays() + 1));
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
			static Ash::Test::Assertion setYearMonthDay()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						TEST_IS_FALSE(date.setYearMonthDay(year, month, 0));
						for (Ash::Calendar::Day day = 1; day <= month.getDays(year); day++)
						{
							TEST_IS_TRUE(date.setYearMonthDay(year, month, day));
							TEST_IS_EQ(date, expected++);
						}
						TEST_IS_FALSE(date.setYearMonthDay(year, month, month.getDays(year) + 1));
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
			static Ash::Test::Assertion setYearWeekDay()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;
				Ash::Calendar::DateDuration expected = Year::epoch - Year::daysPerCycle + 1;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					TEST_IS_FALSE(date.setYearWeekDay(year, 0, Ash::Calendar::Weekday::Monday));
					for (Ash::Calendar::Week week = 1; week <= year.getWeeks(); week++)
					{
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							TEST_IS_TRUE(date.setYearWeekDay(year, week, weekday));
							TEST_IS_EQ(date, expected++);
						}
					}
					TEST_IS_FALSE(date.setYearWeekDay(year, year.getWeeks() + 1, Ash::Calendar::Weekday::Monday));
				}

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

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						Date start;
						TEST_IS_TRUE(start.setYearMonthDay(year, month, 1));
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							TEST_IS_TRUE(date.setYearMonthFirstWeekday(year, month, weekday));
							TEST_IS_EQ(date.getYear(), year);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getWeekday(), weekday);
							TEST_IS_EQ(date - start + 1, Ash::Calendar::Weekday(1 + weekday - start.getWeekday()));
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
			static Ash::Test::Assertion setYearMonthLastWeekday()
			{
				using Calendar = CALENDAR;
				using Date = typename Calendar::Date;
				using Year = typename Calendar::Year;

				Date date;

				for (Year year = -Year::yearsPerCycle + 1; year < Year::yearsPerCycle; year++)
				{
					for (Ash::Calendar::Month month : Ash::Calendar::Month::iterate())
					{
						Date end;
						TEST_IS_TRUE(end.setYearMonthDay(year, month, month.getDays(year)));
						for (Ash::Calendar::Weekday weekday : Ash::Calendar::Weekday::iterate())
						{
							TEST_IS_TRUE(date.setYearMonthLastWeekday(year, month, weekday));
							TEST_IS_EQ(date.getYear(), year);
							TEST_IS_EQ(date.getMonth(), month);
							TEST_IS_EQ(date.getWeekday(), weekday);
							TEST_IS_EQ(end - date + 1, Ash::Calendar::Weekday(1 + end.getWeekday() - weekday));
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

			TEST_CASE_GENERIC(Ash::Test::Calendar::month, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::date, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearOrdinalValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearOrdinal, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearMonthDayValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthDay, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearWeekValid, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearWeekDay, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthFirstWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthLastWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getMonth, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getDay, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getOrdinal, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getWeekday, Ash::Calendar::Gregorian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getYearWeek, Ash::Calendar::Gregorian),

			TEST_CASE_GENERIC(Ash::Test::Calendar::month, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::date, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearOrdinalValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearOrdinal, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearMonthDayValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthDay, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::isYearWeekValid, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearWeekDay, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthFirstWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::setYearMonthLastWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getMonth, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getDay, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getOrdinal, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getWeekday, Ash::Calendar::Julian),
			TEST_CASE_GENERIC(Ash::Test::Calendar::getYearWeek, Ash::Calendar::Julian)
		);
	}
}
