#pragma once

#include <cstdint>
#include "ash.real.h"


namespace Ash
{
	namespace Statistic
	{
		template
		<
			typename REAL,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		class Summary
		{
		public:
			using Real = REAL;
			
			using Count = uintmax_t;

			constexpr Summary() : m_Minimum(0), m_Maximum(0), m_Total(0), m_TotalSquares(0), m_Count(0) {}

			template
			<
				typename VALUE,
				typename ...NEXT_VALUE,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>, Ash::Type::Requirement::IsFloatingPoint<VALUE>, Ash::Type::Requirement::IsInteger<VALUE>>::IsValid
			>
			constexpr Summary(VALUE value, NEXT_VALUE ...nextValue) : Summary()
			{
				add(value, nextValue...);
			}

			template
			<
				typename VALUE,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>, Ash::Type::Requirement::IsFloatingPoint<VALUE>, Ash::Type::Requirement::IsInteger<VALUE>>::IsValid
			>
			constexpr Summary &add(VALUE value)
			{
				if (m_Count == 0)
				{
					m_Count = 1;
					m_Minimum = value;
					m_Maximum = value;
					m_Total = value;
					m_TotalSquares = Real(value).square();
				}
				else
				{
					m_Count = m_Count + 1;
					if (value < m_Minimum)
					{
						m_Minimum = value;
					}
					else if (value > m_Maximum)
					{
						m_Maximum = value;
					}
					m_Total = m_Total + value;
					m_TotalSquares = m_TotalSquares + Real(value).square();
				}

				return *this;
			}

			template
			<
				typename VALUE,
				typename ...NEXT_VALUE,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>, Ash::Type::Requirement::IsFloatingPoint<VALUE>, Ash::Type::Requirement::IsInteger<VALUE>>::IsValid
			>
			constexpr void add(VALUE value, NEXT_VALUE ...nextValue)
			{
				add(value);
				if constexpr (sizeof...(nextValue) != 0)
				{
					add(nextValue...);
				}
			}

			constexpr void add(const Summary &summary)
			{
				if (this == &summary)
				{
					m_Count = 2 * m_Count;
					m_Total = 2 * m_Total;
					m_TotalSquares = 2 * m_TotalSquares;
				}
				else if (m_Count == 0)
				{
					m_Count = summary.m_Count;
					m_Minimum = summary.m_Minimum;
					m_Maximum = summary.m_Maximum;
					m_Total = summary.m_Total;
					m_TotalSquares = summary.m_TotalSquares;
				}
				else if (summary.m_Count != 0)
				{
					m_Count = m_Count + summary.m_Count;
					if (summary.m_Minimum < m_Minimum)
					{
						m_Minimum = summary.m_Minimum;
					}
					if (summary.m_Maximum > m_Maximum)
					{
						m_Maximum = summary.m_Maximum;
					}
					m_Total = m_Total + summary.m_Total;
					m_TotalSquares = m_TotalSquares + summary.m_TotalSquares;
				}
			}

			constexpr bool isEqual(const Summary &summary, Real::Match match = Real::Match::Absolute, Real tolerance = 1.0) const
			{
				return (m_Count == summary.m_Count) &&
				       m_Minimum.isEqual(summary.m_Minimum, match, tolerance) &&
				       m_Maximum.isEqual(summary.m_Maximum, match, tolerance) &&
						 m_TotalSquares.isEqual(summary.m_TotalSquares, match, tolerance) &&
						 m_Total.isEqual(summary.m_Total, match, tolerance);
			}

			constexpr Count getCount() const { return m_Count; }

			constexpr Real getTotal() const { return m_Total; }

			constexpr Real getMean() const { return m_Total / m_Count; }

			constexpr Real getVariance() const
			{
				Real mean = getMean();
				return ((m_Count * mean - 2 * m_Total) * mean + m_TotalSquares) / m_Count;
			}

			constexpr Real getStandardDeviation() const { return getVariance().squareRoot(); }

			constexpr Real getMinimum() const { return m_Minimum; }

			constexpr Real getMaximum() const { return m_Maximum; }

			constexpr Real getRange() const { return m_Maximum - m_Minimum; }

		private:
			Real  m_Minimum;
			Real  m_Maximum;
			Real  m_Total;
			Real  m_TotalSquares;
			Count m_Count;
		};
	}
}
