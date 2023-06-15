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

		template <typename SUBCYCLE, DateDuration STEP, DateDuration COUNT, DateDuration EPOCH=0>
		class YearCycle : public SUBCYCLE
		{
		public:
			using SubCycle = SUBCYCLE;

			using Value = typename SubCycle::Value;

			static constexpr DateDuration epoch = EPOCH;

			static constexpr DateDuration cycleStep = STEP;

			static constexpr DateDuration cycleCount = COUNT;

			static constexpr DateDuration yearsPerCycle = cycleStep * cycleCount;

			static constexpr DateDuration daysPerCycle = SubCycle::daysPerCycle * (yearsPerCycle / SubCycle::yearsPerCycle) - cycleCount + 1;

			static constexpr DateDuration daysPerCycleStep = (daysPerCycle - 1) / cycleCount;

			constexpr YearCycle(Value year = 0) : SubCycle(year) {}

			constexpr bool isLeapYear() const
			{
				return (Value(*this) % cycleStep == 0) ? (Value(*this) / cycleStep) % cycleCount == 0 : SubCycle::isLeapYear();
			}

			constexpr Ordinal getDays() const { return isLeapYear() ? SubCycle::leapYearDays : SubCycle::commonYearDays; }

			constexpr DateDuration getDaysSinceEpoch(Ordinal ordinal = 1) const
			{
				Value year = ((Value(*this) > 0) ? Value(*this) - 1 : Value(*this)) / cycleStep;
				return year / cycleCount - year + epoch - SubCycle::epoch + SubCycle::getDaysSinceEpoch(ordinal);
			}

			static constexpr Value getYearOrdinal(DateDuration days, Ordinal &ordinal)
			{
				days = days - epoch;

				DateDuration period = days / daysPerCycle;
				Value year = period * yearsPerCycle;
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


		template <typename TYPE, Ordinal DAYS>
		class OrbitalCycle
		{
		public:
			using Value = TYPE;

			static constexpr Ordinal yearsPerCycle = 1;

			static constexpr Ordinal daysPerCycle = DAYS;

			static constexpr Ordinal commonYearDays = daysPerCycle - 1;

			static constexpr Ordinal leapYearDays = daysPerCycle;

			static constexpr DateDuration epoch = 0;

			constexpr OrbitalCycle(Value year = 0) : m_Value(year) {}

			constexpr operator Value () const { return m_Value; }

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
				return (Value(*this) - 1) * leapYearDays + ordinal;
			}

		private:
			Value m_Value;
		};


		using TerrestrialCycle = OrbitalCycle<int16_t, 366>;

		using JulianCycle = YearCycle<TerrestrialCycle, 1, 4, -2>;

		using GregorianCycle = YearCycle<JulianCycle, 100, 4>;


		class Month : public Ash::Integer::Cycle<uint8_t, 1, 12>
		{
		public:
			using Cycle = Ash::Integer::Cycle<uint8_t, 1, 12>;

			enum Value
			{
				January = Cycle::minValue,
				February,
				March,
				April,
				May,
				June,
				July,
				August,
				September,
				October,
				November,
				December
			};

			static constexpr Type monthsPerYear = Cycle::size;

			constexpr Month() : Cycle() {}

			template <typename VALUE>
			constexpr Month(VALUE value) : Cycle(value) {}

			template <typename YEAR_CYCLE>
			constexpr Day getDays(YEAR_CYCLE yearCycle) const
			{
				//                       Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
				const Day monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

				return (yearCycle.isLeapYear() && (*this == February)) ? monthDays[*this - January] + 1 : monthDays[*this - January];
			}

			template <typename YEAR_CYCLE>
			constexpr Ordinal getFirstDayOrdinal(YEAR_CYCLE yearCycle) const
			{
				//                              Jan   Feb   Mar   Apr   May   Jun   Jul   Aug   Sep   Oct   Nov   Dec
				//                                 +31   +28   +31   +30   +31   +30   +31   +31   +30   +31   +30   +31
				const Ordinal monthDays[] = {     1,   32,   60,   91,  121,  152,  182,  213,  244,  274,  305,  335  };

				return (yearCycle.isLeapYear() && (*this > February)) ? monthDays[*this - January] + 1 : monthDays[*this - January];
			}

			template <typename YEAR_CYCLE>
			constexpr Ordinal getLastDayOrdinal(YEAR_CYCLE yearCycle) const
			{
				//                              Jan   Feb   Mar   Apr   May   Jun   Jul   Aug   Sep   Oct   Nov   Dec
				//                           +31   +28   +31   +30   +31   +30   +31   +31   +30   +31   +30   +31
				const Ordinal monthDays[] = {    31,   59,   90,  120,  151,  181,  212,  243,  273,  304,  334,  365  };

				return (yearCycle.isLeapYear() && (*this >= February)) ? monthDays[*this - January] + 1 : monthDays[*this - January];
			}
		};


		class Weekday : public Ash::Integer::Cycle<uint8_t, 1, 7>
		{
		public:
			using Cycle = Ash::Integer::Cycle<uint8_t, 1, 7>;

			enum Value
			{
				Monday = Cycle::minValue,
				Tuesday,
				Wednesday,
				Thursday,
				Friday,
				Saturday,
				Sunday
			};

			static constexpr Type daysPerWeek = Cycle::size;

			constexpr Weekday() : Cycle() {}

			template <typename VALUE>
			constexpr Weekday(VALUE value) : Cycle(value) {}
		};


		template <typename YEAR_CYCLE>
		class Proleptic
		{
		public:
			using Cycle = YEAR_CYCLE;

			using Value = typename Cycle::Value;


			class Year : public Cycle
			{
			public:
				constexpr Year(Value year = 0) : Cycle(year) {}

				constexpr Week getWeeks() const
				{
					Weekday weekday = Weekday(Cycle::getDaysSinceEpoch());
					return ((weekday == Weekday::Thursday) || (Cycle::isLeapYear() && (weekday == Weekday::Wednesday))) ? 53 : 52;
				}

				constexpr DateDuration getFirstWeek() const
				{
					DateDuration days = Cycle::getDaysSinceEpoch(4);
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
					Ordinal ordinal;
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

					for (m_Month = Month::January; (m_Month < Month::December) && (ordinal > m_Month.getDays(m_Year)); m_Month++)
					{
						ordinal = ordinal - m_Month.getDays(m_Year);
					}

					m_Day = ordinal;
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

				constexpr Weekday getWeekday() const { return Weekday(*this); }

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
	}
}
