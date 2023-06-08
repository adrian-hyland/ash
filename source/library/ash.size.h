#pragma once

#include <cstdint>


namespace Ash
{
	class Size
	{
	public:
		constexpr Size() : m_Value(0), m_IsValid(true) {}

		constexpr Size(size_t value) : m_Value(value), m_IsValid(true) {}

		constexpr bool isValid() { return m_IsValid; }

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
			size_t partialResult = 0;

			result.m_IsValid = m_IsValid && ((msLeft == 0) || (msRight == 0));
			result.m_Value = lsLeft * lsRight;
					
			partialResult = (msLeft == 0) ? lsLeft * msRight : msLeft * lsRight;
			result.m_IsValid = result.m_IsValid && ((partialResult & ~mask) == 0);
			partialResult = partialResult << shift;

			result.m_IsValid = result.m_IsValid && (result.m_Value <= SIZE_MAX - partialResult);
			result.m_Value = result.m_Value + partialResult;

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
				result = add(size - m_Value % size);
			}
			return result;
		}

		constexpr Size roundUp(const Size &size) const
		{
			return size.m_IsValid ? roundUp(size.m_Value) : size;
		}

	private:
		size_t m_Value;
		bool   m_IsValid;
	};
}
