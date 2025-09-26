#pragma once

#include <cstdint>
#include "ash.calendar.error.h"
#include "ash.integer.h"


namespace Ash
{
	namespace Calendar
	{
		using DateDuration = int32_t;

		using Day = uint8_t;

		using Ordinal = uint16_t;

		using Week = uint8_t;

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

			static constexpr Type january   = 1;
			static constexpr Type february  = 2;
			static constexpr Type march     = 3;
			static constexpr Type april     = 4;
			static constexpr Type may       = 5;
			static constexpr Type june      = 6;
			static constexpr Type july      = 7;
			static constexpr Type august    = 8;
			static constexpr Type september = 9;
			static constexpr Type october   = 10;
			static constexpr Type november  = 11;
			static constexpr Type december  = 12;

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

				return (yearCycle.isLeapYear() && (*this == february)) ? monthDays[*this - january] + 1 : monthDays[*this - january];
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

				return (yearCycle.isLeapYear() && (*this > february)) ? monthDays[*this - january] + 1 : monthDays[*this - january];
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

				return (yearCycle.isLeapYear() && (*this >= february)) ? monthDays[*this - january] + 1 : monthDays[*this - january];
			}

			template
			<
				typename YEAR_CYCLE,
				typename = Ash::Type::IsClass<YEAR_CYCLE, Ash::Calendar::Generic::YearCycle>
			>
			static constexpr Month getMonthDay(YEAR_CYCLE yearCycle, Ordinal ordinal, Day &day)
			{
				Ordinal startOfMarch = Month(Month::march).getFirstDayOrdinal(yearCycle);

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

			static constexpr Type monday    = 1;
			static constexpr Type tuesday   = 2;
			static constexpr Type wednesday = 3;
			static constexpr Type thursday  = 4;
			static constexpr Type friday    = 5;
			static constexpr Type saturday  = 6;
			static constexpr Type sunday    = 7;

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
				static constexpr Type minimum = std::numeric_limits<Type>::min();

				static constexpr Type maximum = std::numeric_limits<Type>::max();

				constexpr Year(Type year = 0) : YearCycle(year) {}

				constexpr Week getWeeks() const
				{
					Weekday weekday = Weekday(YearCycle::getDaysSinceEpoch());
					return ((weekday == Weekday::thursday) || (YearCycle::isLeapYear() && (weekday == Weekday::wednesday))) ? 53 : 52;
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
				static constexpr DateDuration maximum = Ash::Calendar::Proleptic<YearCycle>::Date::withYearMonthDay(Year::maximum, Month::december, 31);

				static constexpr DateDuration minimum = Ash::Calendar::Proleptic<YearCycle>::Date::withYearMonthDay(Year::minimum, Month::january, 1);

				constexpr Date() : m_Year(1), m_Month(Month::january), m_Day(1) {}

				template
				<
					typename DAYS,
					typename = Ash::Type::IsInteger<DAYS>
				>
				constexpr Date(DAYS days) : Date()
				{
					set(days).throwOnError();
				}

				template
				<
					typename YEAR,
					typename ORDINAL,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::IsInteger<ORDINAL>
				>
				static constexpr Date withYearOrdinal(YEAR year, ORDINAL ordinal)
				{
					Date date;

					date.setYearOrdinal(year, ordinal).throwOnError();

					return date;
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename DAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::IsInteger<DAY>
				>
				static constexpr Date withYearMonthDay(YEAR year, MONTH month, DAY day)
				{
					Date date;

					date.setYearMonthDay(year, month, day).throwOnError();

					return date;
				}

				template
				<
					typename YEAR,
					typename WEEK,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::IsInteger<WEEK>,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				static constexpr Date withYearWeekday(YEAR year, WEEK week, WEEKDAY weekday)
				{
					Date date;

					date.setYearWeekday(year, week, weekday).throwOnError();

					return date;
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				static constexpr Date withYearMonthFirstWeekday(YEAR year, MONTH month, WEEKDAY weekday)
				{
					Date date;

					date.setYearMonthFirstWeekday(year, month, weekday).throwOnError();

					return date;
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				static constexpr Date withYearMonthLastWeekday(YEAR year, MONTH month, WEEKDAY weekday)
				{
					Date date;

					date.setYearMonthLastWeekday(year, month, weekday).throwOnError();

					return date;
				}

				template
				<
					typename DAYS,
					typename = Ash::Type::IsInteger<DAYS>
				>
				[[nodiscard]]
				constexpr Ash::Error::Value set(DAYS days)
				{
					Ash::Error::Value error = validate(days);
					if (error == Ash::Error::none)
					{
						Ordinal ordinal = 1;
						Year year = Year::getYearOrdinal(days, ordinal);
						error = setYearOrdinal(year, ordinal);
					}
					return error;
				}

				template
				<
					typename YEAR,
					typename ORDINAL,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::IsInteger<ORDINAL>
				>
				[[nodiscard]]
				constexpr Ash::Error::Value setYearOrdinal(YEAR year, ORDINAL ordinal)
				{
					Ash::Error::Value error = validateYearOrdinal(year, ordinal);
					if (error == Ash::Error::none)
					{
						m_Year = year;
						m_Month = Month::getMonthDay(m_Year, ordinal, m_Day);
					}
					return error;
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename DAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::IsInteger<DAY>
				>
				[[nodiscard]]
				constexpr Ash::Error::Value setYearMonthDay(YEAR year, MONTH month, DAY day)
				{
					Ash::Error::Value error = validateYearMonthDay(year, month, day);
					if (error == Ash::Error::none)
					{
						m_Year = year;
						m_Month = month;
						m_Day = day;
					}
					return error;
				}

				template
				<
					typename YEAR,
					typename WEEK,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::IsInteger<WEEK>,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value setYearWeekday(YEAR year, WEEK week, WEEKDAY weekday)
				{
					Ash::Error::Value error = validateYearWeekday(year, week, weekday);
					if (error == Ash::Error::none)
					{
						error = set(Year(year).getFirstWeek() + (week - 1) * Weekday::daysPerWeek + weekday - 1);
					}
					return error;
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value setYearMonthFirstWeekday(YEAR year, MONTH month, WEEKDAY weekday)
				{
					Ash::Error::Value error = validateYearMonthWeekday(year, month, weekday);
					if (error == Ash::Error::none)
					{
						m_Year = year;
						m_Month = month;
						m_Day = 1;
						m_Day = Weekday(1 + weekday - *this);
					}
					return error;
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value setYearMonthLastWeekday(YEAR year, MONTH month, WEEKDAY weekday)
				{
					Ash::Error::Value error = validateYearMonthWeekday(year, month, weekday);
					if (error == Ash::Error::none)
					{
						m_Year = year;
						m_Month = month;
						m_Day = m_Month.getDays(m_Year);
						m_Day = 1 + m_Day - Weekday(1 + *this - weekday);
					}
					return error;
				}

				template
				<
					typename DAYS,
					typename = Ash::Type::IsInteger<DAYS>
				>
				[[nodiscard]]
				static constexpr Ash::Error::Value validate(DAYS days)
				{
					return !isValid(days) ? Ash::Calendar::Error::invalidDuration : Ash::Error::none;
				}

				template
				<
					typename YEAR,
					typename ORDINAL,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::IsInteger<ORDINAL>
				>
				[[nodiscard]]
				static constexpr Ash::Error::Value validateYearOrdinal(YEAR year, ORDINAL ordinal)
				{
					if (!isYearValid(year))
					{
						return Ash::Calendar::Error::invalidYear;
					}
					else if (!isYearOrdinalValid(year, ordinal))
					{
						return Ash::Calendar::Error::invalidOrdinal;
					}
					else
					{
						return Ash::Error::none;
					}
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename DAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::IsInteger<DAY>
				>
				[[nodiscard]]
				static constexpr Ash::Error::Value validateYearMonthDay(YEAR year, MONTH month, DAY day)
				{
					if (!isYearValid(year))
					{
						return Ash::Calendar::Error::invalidYear;
					}
					else if (!isMonthValid(month))
					{
						return Ash::Calendar::Error::invalidMonth;
					}
					else if (!isYearMonthDayValid(year, month, day))
					{
						return Ash::Calendar::Error::invalidDay;
					}
					else
					{
						return Ash::Error::none;
					}
				}

				template
				<
					typename YEAR,
					typename WEEK,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::IsInteger<WEEK>,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				[[nodiscard]]
				static constexpr Ash::Error::Value validateYearWeekday(YEAR year, WEEK week, WEEKDAY weekday)
				{
					if (!isYearValid(year))
					{
						return Ash::Calendar::Error::invalidYear;
					}
					else if (!isYearWeekValid(year, week))
					{
						return Ash::Calendar::Error::invalidWeek;
					}
					else if (!isWeekdayValid(weekday))
					{
						return Ash::Calendar::Error::invalidWeekday;
					}
					else
					{
						return Ash::Error::none;
					}
				}

				template
				<
					typename YEAR,
					typename MONTH,
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				[[nodiscard]]
				static constexpr Ash::Error::Value validateYearMonthWeekday(YEAR year, MONTH month, WEEKDAY weekday)
				{
					if (!isYearValid(year))
					{
						return Ash::Calendar::Error::invalidYear;
					}
					else if (!isMonthValid(month))
					{
						return Ash::Calendar::Error::invalidMonth;
					}
					else if (!isWeekdayValid(weekday))
					{
						return Ash::Calendar::Error::invalidWeekday;
					}
					else
					{
						return Ash::Error::none;
					}
				}

				constexpr operator DateDuration () const { return m_Year.getDaysSinceEpoch(getOrdinal()); }

				constexpr Year getYear() const { return m_Year; }

				constexpr Month getMonth() const { return m_Month; }

				constexpr Day getDay() const { return m_Day; }

				constexpr Ordinal getOrdinal() const { return m_Month.getFirstDayOrdinal(m_Year) + m_Day - 1; }

				constexpr Weekday getWeekday() const { return Weekday(DateDuration(*this)); }

				[[nodiscard]]
				constexpr Ash::Error::Value getYearWeek(Year &year, Week &week) const
				{
					DateDuration day = *this;
					DateDuration firstDay = m_Year.getFirstWeek();
					DateDuration nextFirstDay = firstDay + m_Year.getWeeks() * Weekday::daysPerWeek;
					if (day >= nextFirstDay)
					{
						if (m_Year == Year::maximum)
						{
							return Ash::Calendar::Error::invalidYear;
						}
						year = m_Year + 1;
						firstDay = nextFirstDay;
					}
					else if (day < firstDay)
					{
						if (m_Year == Year::minimum)
						{
							return Ash::Calendar::Error::invalidYear;
						}
						year = m_Year - 1;
						firstDay = year.getFirstWeek();
					}
					else
					{
						year = m_Year;
					}
					week = (day - firstDay + Weekday::daysPerWeek) / Weekday::daysPerWeek;
					return Ash::Error::none;
				}

				constexpr Date &operator ++ () { *this = *this + 1; return *this; }

				constexpr Date &operator -- () { *this = *this - 1; return *this; }

				constexpr Date operator ++ (int) { Date date = *this; ++(*this); return date; }

				constexpr Date operator -- (int) { Date date = *this; --(*this); return date; }

			protected:
				template
				<
					typename DAYS,
					typename = Ash::Type::IsInteger<DAYS>
				>
				static constexpr bool isValid(DAYS days)
				{
					if constexpr (Ash::Type::isUnsignedInteger<DAYS>)
					{
						return days <= maximum;
					}
					else
					{
						return (days >= minimum) && (days <= maximum);
					}
				}

				template
				<
					typename YEAR,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<YEAR>, Ash::Type::Requirement::IsClass<YEAR, Year>>::IsValid
				>
				static constexpr bool isYearValid(YEAR year)
				{
					if constexpr (Ash::Type::isUnsignedInteger<YEAR>)
					{
						return year <= Year::maximum;
					}
					else
					{
						return (year >= Year::minimum) && (year <= Year::maximum);
					}
				}

				template
				<
					typename MONTH,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MONTH>, Ash::Type::Requirement::IsClass<MONTH, Month>>::IsValid
				>
				static constexpr bool isMonthValid(MONTH month)
				{
					return (month >= Month::minimum) && (month <= Month::maximum);
				}

				template
				<
					typename WEEKDAY,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<WEEKDAY>, Ash::Type::Requirement::IsClass<WEEKDAY, Weekday>>::IsValid
				>
				static constexpr bool isWeekdayValid(WEEKDAY weekday)
				{
					return (weekday >= Weekday::minimum) && (weekday <= Weekday::maximum);
				}

				template
				<
					typename ORDINAL,
					typename = Ash::Type::IsInteger<ORDINAL>
				>
				static constexpr bool isYearOrdinalValid(Year year, ORDINAL ordinal)
				{
					return (ordinal > 0) && (ordinal <= year.getDays());
				}

				template
				<
					typename DAY,
					typename = Ash::Type::IsInteger<DAY>
				>
				static constexpr bool isYearMonthDayValid(Year year, Month month, DAY day)
				{
					return (day > 0) && (day <= month.getDays(year));
				}

				template
				<
					typename WEEK,
					typename = Ash::Type::IsInteger<WEEK>
				>
				static constexpr bool isYearWeekValid(Year year, WEEK week)
				{
					return (week > 0) && (week <= year.getWeeks());
				}

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
