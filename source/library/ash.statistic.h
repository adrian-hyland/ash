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

			constexpr Summary() : m_Minimum(0), m_Maximum(0), m_Total(0), m_Variance(0), m_Count(0) {}

			template
			<
				typename VALUE,
				typename ...NEXT_VALUE,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>, Ash::Type::Requirement::IsFloatingPoint<VALUE>>::IsValid
			>
			constexpr Summary(VALUE value, NEXT_VALUE ...nextValue) : Summary()
			{
				add(value, nextValue...);
			}

			template
			<
				typename VALUE,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>, Ash::Type::Requirement::IsFloatingPoint<VALUE>>::IsValid
			>
			constexpr Summary &add(VALUE value)
			{
				if (m_Count == 0)
				{
					m_Count = 1;
					m_Minimum = value;
					m_Maximum = value;
					m_Total = value;
					m_Variance = 0;
				}
				else
				{
					Count count = m_Count;
					Real total = m_Total;
					Real mean = getMean();
					m_Count = m_Count + 1;
					if (value < m_Minimum)
					{
						m_Minimum = value;
					}
					if (value > m_Maximum)
					{
						m_Maximum = value;
					}
					m_Total = m_Total + value;
					Real meanDifference = getMean() - mean;
					m_Variance = m_Variance + Real(value - getMean()).square() + meanDifference * (count * (meanDifference - 2 * mean) + 2 * total);
				}

				return *this;
			}

			template
			<
				typename VALUE,
				typename ...NEXT_VALUE,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsClass<VALUE, Ash::Generic::Real>, Ash::Type::Requirement::IsFloatingPoint<VALUE>>::IsValid
			>
			constexpr Summary &add(VALUE value, NEXT_VALUE ...nextValue)
			{
				add(value);
				if constexpr (sizeof...(nextValue) != 0)
				{
					add(nextValue...);
				}
				return *this;
			}

			constexpr bool isEqual(const Summary &summary, Real::Match match = Real::Match::Absolute, Real tolerance = 1.0) const
			{
				return (m_Count == summary.m_Count) &&
				       m_Minimum.isEqual(summary.m_Minimum, match, tolerance) &&
				       m_Maximum.isEqual(summary.m_Maximum, match, tolerance) &&
						 m_Variance.isEqual(summary.m_Variance, match, tolerance) &&
						 m_Total.isEqual(summary.m_Total, match, tolerance);
			}

			constexpr Count getCount() const { return m_Count; }

			constexpr Real getTotal() const { return m_Total; }

			constexpr Real getMean() const { return m_Total / m_Count; }

			constexpr Real getVariance() const { return m_Variance / m_Count; }

			constexpr Real getStandardDeviation() const { return getVariance().squareRoot(); }

			constexpr Real getMinimum() const { return m_Minimum; }

			constexpr Real getMaximum() const { return m_Maximum; }

			constexpr Real getRange() const { return m_Maximum - m_Minimum; }

		private:
			Real  m_Minimum;
			Real  m_Maximum;
			Real  m_Total;
			Real  m_Variance;
			Count m_Count;
		};
	}
}
