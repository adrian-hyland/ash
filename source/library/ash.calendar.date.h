#pragma once

#include <cstdint>
#include "ash.integer.h"


namespace Ash
{
	namespace Calendar
	{
		using DateDuration = int32_t;

		using Day = unsigned char;

		using Ordinal = uint16_t;

		using Week = unsigned char;

		namespace Generic
		{
			class YearCycle {};
		}

		template
		<
			typename     SUBCYCLE,
			DateDuration STEP,
			DateDuration COUNT,
			DateDuration EPOCH = 0,
			typename     = Ash::Type::IsClass<SUBCYCLE, Ash::Calendar::Generic::YearCycle>
		>
		class YearCycle : public SUBCYCLE
		{
		public:
			using SubCycle = SUBCYCLE;

			using Type = typename SubCycle::Type;

			static constexpr DateDuration epoch = EPOCH;

			static constexpr DateDuration cycleStep = STEP;

			static constexpr DateDuration cycleCount = COUNT;

			static constexpr DateDuration yearsPerCycle = cycleStep * cycleCount;

			static constexpr DateDuration daysPerCycle = SubCycle::daysPerCycle * (yearsPerCycle / SubCycle::yearsPerCycle) - cycleCount + 1;

			static constexpr DateDuration daysPerCycleStep = (daysPerCycle - 1) / cycleCount;

			constexpr YearCycle(Type year = 0) : SubCycle(year) {}

			constexpr YearCycle &operator ++ () { *this = *this + 1; return *this; }

			constexpr YearCycle &operator -- () { *this = *this - 1; return *this; }

			constexpr YearCycle operator ++ (int) { YearCycle result = *this; ++(*this); return result; }

			constexpr YearCycle operator -- (int) { YearCycle result = *this; --(*this); return result; }

			constexpr bool isLeapYear() const
			{
				return (Type(*this) % cycleStep == 0) ? (Type(*this) / cycleStep) % cycleCount == 0 : SubCycle::isLeapYear();
			}

			constexpr Ordinal getDays() const { return isLeapYear() ? SubCycle::leapYearDays : SubCycle::commonYearDays; }

			constexpr DateDuration getDaysSinceEpoch(Ordinal ordinal = 1) const
			{
				Type year = ((Type(*this) > 0) ? Type(*this) - 1 : Type(*this)) / cycleStep;
				return year / cycleCount - year + epoch - SubCycle::epoch + SubCycle::getDaysSinceEpoch(ordinal);
			}

			static constexpr Type getYearOrdinal(DateDuration days, Ordinal &ordinal)
			{
				days = days - epoch;

				DateDuration period = days / daysPerCycle;
				Type year = period * yearsPerCycle;
				days = days % daysPerCycle;
				if (days == 0)
				{
					ordinal = SubCycle::leapYearDays;
					return year;
				}

				period = days / daysPerCycleStep;
				year = year + period * cycleStep;
				days = days % daysPerCycleStep;
				if (days == 0)
				{
					ordinal = (period < 0) ? 1 : SubCycle::commonYearDays;
					return (period < 0) ? year + 1 : year;
				}

				return year + SubCycle::getYearOrdinal(days + SubCycle::epoch, ordinal);
			}
		};


		template
		<
			typename TYPE,
			Ordinal  DAYS,
			typename = Ash::Type::IsInteger<TYPE>
		>
		class OrbitalCycle : Ash::Calendar::Generic::YearCycle
		{
		public:
			using Type = TYPE;

			static constexpr Ordinal yearsPerCycle = 1;

			static constexpr Ordinal daysPerCycle = DAYS;

			static constexpr Ordinal commonYearDays = daysPerCycle - 1;

			static constexpr Ordinal leapYearDays = daysPerCycle;

			static constexpr DateDuration epoch = 0;

			constexpr OrbitalCycle(Type year = 0) : m_Value(year) {}

			constexpr operator Type () const { return m_Value; }

			constexpr bool isLeapYear() const { return false; }

			constexpr OrbitalCycle &operator ++ () { *this = *this + 1; return *this; }

			constexpr OrbitalCycle &operator -- () { *this = *this - 1; return *this; }

			constexpr OrbitalCycle operator ++ (int) { OrbitalCycle result = *this; ++(*this); return result; }

			constexpr OrbitalCycle operator -- (int) { OrbitalCycle result = *this; --(*this); return result; }

			static constexpr DateDuration getYearOrdinal(DateDuration days, Ordinal &ordinal)
			{
				ordinal = (days < 0) ? days + leapYearDays : days;
				return (days < 0) ? 0 : 1;
			}

			constexpr DateDuration getDaysSinceEpoch(Ordinal ordinal = 1) const
			{
				return (Type(*this) - 1) * leapYearDays + ordinal;
			}

		private:
			Type m_Value;
		};


		using TerrestrialCycle = OrbitalCycle<int16_t, 366>;

		using JulianCycle = YearCycle<TerrestrialCycle, 1, 4, -2>;

		using GregorianCycle = YearCycle<JulianCycle, 100, 4>;


		class Month : public Ash::Integer::Cycle<1, 12>
		{
		public:
			using Cycle = Ash::Integer::Cycle<1, 12>;

			static constexpr Type January   = 1;
			static constexpr Type February  = 2;
			static constexpr Type March     = 3;
			static constexpr Type April     = 4;
			static constexpr Type May       = 5;
			static constexpr Type June      = 6;
			static constexpr Type July      = 7;
			static constexpr Type August    = 8;
			static constexpr Type September = 9;
			static constexpr Type October   = 10;
			static constexpr Type November  = 11;
			static constexpr Type December  = 12;

			static constexpr Type monthsPerYear = Cycle::size;

			constexpr Month() : Cycle() {}

			constexpr Month(const Cycle &value) : Cycle(value) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Month(VALUE value) : Cycle(value) {}

			template
			<
				typename YEAR_CYCLE,
				typename = Ash::Type::IsClass<YEAR_CYCLE, Ash::Calendar::Generic::YearCycle>
			>
			constexpr Day getDays(YEAR_CYCLE yearCycle) const
			{
				//                           Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
				constexpr Day monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

				return (yearCycle.isLeapYear() && (*this == February)) ? monthDays[*this - January] + 1 : monthDays[*this - January];
			}

			template
			<
				typename YEAR_CYCLE,
				typename = Ash::Type::IsClass<YEAR_CYCLE, Ash::Calendar::Generic::YearCycle>
			>
			constexpr Ordinal getFirstDayOrdinal(YEAR_CYCLE yearCycle) const
			{
				//                                  Jan   Feb   Mar   Apr   May   Jun   Jul   Aug   Sep   Oct   Nov   Dec
				//                                     +31   +28   +31   +30   +31   +30   +31   +31   +30   +31   +30   +31
				constexpr Ordinal monthDays[] = {     1,   32,   60,   91,  121,  152,  182,  213,  244,  274,  305,  335  };

				return (yearCycle.isLeapYear() && (*this > February)) ? monthDays[*this - January] + 1 : monthDays[*this - January];
			}

			template
			<
				typename YEAR_CYCLE,
				typename = Ash::Type::IsClass<YEAR_CYCLE, Ash::Calendar::Generic::YearCycle>
			>
			constexpr Ordinal getLastDayOrdinal(YEAR_CYCLE yearCycle) const
			{
				//                                  Jan   Feb   Mar   Apr   May   Jun   Jul   Aug   Sep   Oct   Nov   Dec
				//                               +31   +28   +31   +30   +31   +30   +31   +31   +30   +31   +30   +31
				constexpr Ordinal monthDays[] = {    31,   59,   90,  120,  151,  181,  212,  243,  273,  304,  334,  365  };

				return (yearCycle.isLeapYear() && (*this >= February)) ? monthDays[*this - January] + 1 : monthDays[*this - January];
			}

			template
			<
				typename YEAR_CYCLE,
				typename = Ash::Type::IsClass<YEAR_CYCLE, Ash::Calendar::Generic::YearCycle>
			>
			static constexpr Month getMonthDay(YEAR_CYCLE yearCycle, Ordinal ordinal, Day &day)
			{
				Ordinal startOfMarch = Month(Month::March).getFirstDayOrdinal(yearCycle);

				if (ordinal < startOfMarch)
				{
					ordinal = ordinal + yearCycle.getDays();
				}
				ordinal = (ordinal - startOfMarch) * 5 + 461;

				Month::Cycle::Type month = ordinal / 153;

				day = (ordinal - month * 153) / 5 + 1;

				return month;
			}
		};


		class Weekday : public Ash::Integer::Cycle<1, 7>
		{
		public:
			using Cycle = Ash::Integer::Cycle<1, 7>;

			static constexpr Type Monday    = 1;
			static constexpr Type Tuesday   = 2;
			static constexpr Type Wednesday = 3;
			static constexpr Type Thursday  = 4;
			static constexpr Type Friday    = 5;
			static constexpr Type Saturday  = 6;
			static constexpr Type Sunday    = 7;

			static constexpr Type daysPerWeek = Cycle::size;

			constexpr Weekday() : Cycle() {}

			constexpr Weekday(const Cycle &value) : Cycle(value) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Weekday(VALUE value) : Cycle(value) {}
		};


		namespace Generic
		{
			class Proleptic {};
		};

		template
		<
			typename YEAR_CYCLE,
			typename = Ash::Type::IsClass<YEAR_CYCLE, Ash::Calendar::Generic::YearCycle>
		>
		class Proleptic : Ash::Calendar::Generic::Proleptic
		{
		public:
			using YearCycle = YEAR_CYCLE;

			using Type = typename YearCycle::Type;


			class Year : public YearCycle
			{
			public:
				constexpr Year(Type year = 0) : YearCycle(year) {}

				constexpr Week getWeeks() const
				{
					Weekday weekday = Weekday(YearCycle::getDaysSinceEpoch());
					return ((weekday == Weekday::Thursday) || (YearCycle::isLeapYear() && (weekday == Weekday::Wednesday))) ? 53 : 52;
				}

				constexpr DateDuration getFirstWeek() const
				{
					DateDuration days = YearCycle::getDaysSinceEpoch(4);
					return days - Weekday(days) + 1;
				}
			};


			class Date
			{
			public:
				constexpr Date() : m_Year(1), m_Month(Month::January), m_Day(1) {}

				constexpr Date(DateDuration days) : Date()
				{
					set(days);
				}

				constexpr bool set(DateDuration days)
				{
					Ordinal ordinal = 1;
					Year year = Year::getYearOrdinal(days, ordinal);
					return setYearOrdinal(year, ordinal);
				}

				static constexpr bool isYearOrdinalValid(Year year, Ordinal ordinal)
				{
					return (ordinal > 0) && (ordinal <= year.getDays());
				}

				constexpr bool setYearOrdinal(Year year, Ordinal ordinal)
				{
					if (!isYearOrdinalValid(year, ordinal))
					{
						return false;
					}

					m_Year = year;
					m_Month = Month::getMonthDay(m_Year, ordinal, m_Day);
					return true;
				}

				static constexpr bool isYearMonthDayValid(Year year, Month month, Day day)
				{
					return (day > 0) && (day <= month.getDays(year));
				}

				constexpr bool setYearMonthDay(Year year, Month month, Day day)
				{
					if (!isYearMonthDayValid(year, month, day))
					{
						return false;
					}

					m_Year = year;
					m_Month = month;
					m_Day = day;
					return true;
				}

				static constexpr bool isYearWeekValid(Year year, Week week)
				{
					return (week > 0) && (week <= year.getWeeks());
				}

				constexpr bool setYearWeekDay(Year year, Week week, Weekday weekday)
				{
					if (!isYearWeekValid(year, week))
					{
						return false;
					}

					return set(year.getFirstWeek() + (week - 1) * Weekday::daysPerWeek + weekday - 1);
				}

				constexpr bool setYearMonthFirstWeekday(Year year, Month month, Weekday weekday)
				{
					if (!setYearMonthDay(year, month, 1))
					{
						return false;
					}

					m_Day = Weekday(1 + weekday - *this);
					return true;
				}

				constexpr bool setYearMonthLastWeekday(Year year, Month month, Weekday weekday)
				{
					if (!setYearMonthDay(year, month, month.getDays(year)))
					{
						return false;
					}

					m_Day = 1 + m_Day - Weekday(1 + *this - weekday);
					return true;
				}

				constexpr operator DateDuration () const { return m_Year.getDaysSinceEpoch(getOrdinal()); }

				constexpr Year getYear() const { return m_Year; }

				constexpr Month getMonth() const { return m_Month; }

				constexpr Day getDay() const { return m_Day; }

				constexpr Ordinal getOrdinal() const { return m_Month.getFirstDayOrdinal(m_Year) + m_Day - 1; }

				constexpr Weekday getWeekday() const { return Weekday(DateDuration(*this)); }

				constexpr Week getYearWeek(Year &year) const
				{
					year = m_Year + 1;
					DateDuration day = *this;
					DateDuration firstDay = year.getFirstWeek();
					if (day < firstDay)
					{
						year--;
						firstDay = year.getFirstWeek();
						if (day < firstDay)
						{
							year--;
							firstDay = year.getFirstWeek();
						}
					}
					return (day - firstDay + Weekday::daysPerWeek) / Weekday::daysPerWeek;
				}

				constexpr Date &operator ++ () { *this = *this + 1; return *this; }

				constexpr Date &operator -- () { *this = *this - 1; return *this; }

				constexpr Date operator ++ (int) { Date date = *this; ++(*this); return date; }

				constexpr Date operator -- (int) { Date date = *this; --(*this); return date; }

			private:
				Year  m_Year;
				Month m_Month;
				Day   m_Day;
			};
		};

		using Gregorian = Proleptic<GregorianCycle>;

		using Julian = Proleptic<JulianCycle>;

		using Date = Gregorian::Date;

		using Year = Gregorian::Year;
	}
}
