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

			constexpr Summary add(Real value) const
			{
				Summary result;

				if (m_Count == 0)
				{
					result.m_Count = 1;
					result.m_Minimum = value;
					result.m_Maximum = value;
					result.m_Total = value;
					result.m_Variance = 0;
				}
				else
				{
					result.m_Count = m_Count + 1;
					result.m_Minimum = (value < m_Minimum) ? value : m_Minimum;
					result.m_Maximum = (value > m_Maximum) ? value : m_Maximum;
					result.m_Total = m_Total + value;
					Real meanDifference = result.getMean() - getMean();
					result.m_Variance = m_Variance + Real(value - result.getMean()).square() + meanDifference * (m_Count * (meanDifference - 2 * getMean()) + 2 * m_Total);
				}

				return result;
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
