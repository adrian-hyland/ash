#pragma once

#include <cstddef>
#include <cstdint>


namespace Ash
{
	class Size
	{
	public:
		constexpr Size() : m_Value(0), m_IsValid(true) {}

		constexpr Size(size_t value) : m_Value(value), m_IsValid(true) {}

		constexpr bool isValid() const { return m_IsValid; }

		constexpr bool getValue(size_t &value) const
		{
			value = m_Value;
			return m_IsValid;
		}

		constexpr size_t getValueOr(size_t defaultValue) const
		{
			return m_IsValid ? m_Value : defaultValue;
		}

		constexpr Size add(size_t size) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (size <= SIZE_MAX - m_Value);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value + size;
			}
			return result;
		}

		constexpr Size add(const Size &size) const
		{
			return size.m_IsValid ? add(size.m_Value) : size;
		}

		constexpr Size subtract(size_t size) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (size <= m_Value);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value - size;
			}
			return result;
		}

		constexpr Size subtract(const Size &size) const
		{
			return size.m_IsValid ? subtract(size.m_Value) : size;
		}

		constexpr Size multiply(size_t size) const
		{
			Size result;
			size_t shift = (sizeof(size_t) * 4);
			size_t mask = size_t(~0) >> shift;
			size_t msLeft = m_Value >> shift;
			size_t lsLeft = m_Value & mask;
			size_t msRight = size >> shift;
			size_t lsRight = size & mask;
			size_t lsResult = lsLeft * lsRight;
			size_t msResult = (msLeft == 0) ? lsLeft * msRight : msLeft * lsRight;

			result.m_IsValid = m_IsValid && ((msLeft == 0) || (msRight == 0)) && ((msResult & ~mask) == 0);
			msResult = msResult << shift;
			result.m_IsValid = result.m_IsValid && (lsResult <= SIZE_MAX - msResult);
			if (result.m_IsValid)
			{
				result.m_Value = msResult + lsResult;
			}

			return result;
		}

		constexpr Size multiply(const Size &size) const
		{
			return size.m_IsValid ? multiply(size.m_Value) : size;
		}

		constexpr Size divide(size_t size) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (size != 0);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value / size;
			}
			return result;
		}

		constexpr Size divide(const Size &size) const
		{
			return size.m_IsValid ? divide(size.m_Value) : size;
		}

		constexpr Size remainder(size_t size) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (size != 0);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value % size;
			}
			return result;
		}

		constexpr Size remainder(const Size &size) const
		{
			return size.m_IsValid ? remainder(size.m_Value) : size;
		}

		constexpr Size roundUp(size_t size) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (size != 0);
			if (result.m_IsValid)
			{
				if (m_Value == 0)
				{
					result = size;
				}
				else
				{
					size_t remainder = m_Value % size;
					result = (remainder == 0) ? m_Value : add(size - remainder);
				}
			}
			return result;
		}

		constexpr Size roundUp(const Size &size) const
		{
			return size.m_IsValid ? roundUp(size.m_Value) : size;
		}

		constexpr Size roundDown(size_t size) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (size != 0);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value - m_Value % size;
			}
			return result;
		}

		constexpr Size roundDown(const Size &size) const
		{
			return size.m_IsValid ? roundDown(size.m_Value) : size;
		}

		constexpr Size percent(size_t percent) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (percent <= 100);
			result.m_Value = m_Value;
			return result.multiplyFraction(percent, 100);
		}

		constexpr Size percent(const Size &size) const
		{
			return size.m_IsValid ? percent(size.m_Value) : size;
		}

		constexpr Size reversePercent(size_t percent) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (percent <= 100);
			result.m_Value = m_Value;
			return result.multiplyFraction(percent, 100 + percent);
		}

		constexpr Size reversePercent(const Size &size) const
		{
			return size.m_IsValid ? reversePercent(size.m_Value) : size;
		}

	protected:
		constexpr Size multiplyFraction(size_t numerator, size_t denominator) const
		{
			Size result;
			result.m_IsValid = m_IsValid;
			if ((result.m_IsValid) && (numerator != 0))
			{
				if (numerator == denominator)
				{
					result.m_Value = m_Value;
				}
				else
				{
					size_t max = SIZE_MAX / numerator;
					if (m_Value > max)
					{
						size_t d = m_Value / max;
						size_t r = m_Value % max;
						size_t n1 = max * numerator;
						size_t n2 = r * numerator;
						size_t n3 = d * (n1 % denominator) + (n2 % denominator);
						result.m_Value = d * (n1 / denominator) + (n2 / denominator) + (n3 / denominator);
					}
					else
					{
						result.m_Value = (m_Value * numerator) / denominator;
					}
				}
			}
			return result;
		}

	private:
		size_t m_Value;
		bool   m_IsValid;
	};
}
