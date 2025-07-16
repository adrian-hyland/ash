#pragma once

#include <cstddef>
#include <cstdint>


namespace Ash
{
	/**
	 * @brief A class for manipulating a size value whilst keeping track of its validity
	 */
	class Size
	{
	public:
		/**
		 * @brief Default constructor
		 * @note  - This creates a `Size` object with a zero value. It will also be marked as valid.
		 */
		constexpr Size() : m_Value(0), m_IsValid(true) {}

		/**
		 * @brief Constructs a `Size` object with a specified value
		 * @param value The value to initialize `this` with
		 * @note  The constructed `Size` object will be initially marked as valid
		 */
		constexpr Size(size_t value) : m_Value(value), m_IsValid(true) {}

		/**
		 * @brief  Checks whether the value of `this` is still valid
		 * @return `true` is returned if `this` is valid.
		 * @return `false` is returned if `this` is invalid.
		 */
		constexpr bool isValid() const { return m_IsValid; }

		/**
		 * @brief  Retrieves the value held by `this` so long as it is valid
		 * @param  value Used to return the value held by `this`
		 * @return `true` is returned if the value is valid.
		 * @return `false` is returned if the value is invalid (and not returned).
		 */
		constexpr bool getValue(size_t &value) const
		{
			if (m_IsValid)
			{
				value = m_Value;
			}
			return m_IsValid;
		}

		/**
		 * @brief  Retrieves the value held by `this` (or a default value if it is invalid)
		 * @param  defaultValue The default value to return if `this` is invalid
		 * @return The value of `this` if it is valid or the `defaultValue` if `this` is invalid.
		 */
		constexpr size_t getValueOr(size_t defaultValue) const
		{
			return m_IsValid ? m_Value : defaultValue;
		}

		/**
		 * @brief  Adds a value to `this`
		 * @param  value The value to add
		 * @return A `Size` value that contains the value of `this` added with `value`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if an overflow occurs
		 */
		constexpr Size add(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && !__builtin_add_overflow(m_Value, value, &result.m_Value);
			return result;
		}

		/**
		 * @brief  Adds another `Size` value to `this`
		 * @param  value The size value to add
		 * @return A `Size` value that contains the value of `this` added with `value`
		 * @note   - The returned value will be marked as invalid if either `value` or `this` are invalid or if an overflow occurs
		 */
		constexpr Size add(const Size &value) const
		{
			return value.m_IsValid ? add(value.m_Value) : value;
		}

		/**
		 * @brief  Subtracts a value from `this`
		 * @param  value The value to subtract
		 * @return A `Size` value that contains the `value` subtracted from the value of `this`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if an overflow occurs
		 */
		constexpr Size subtract(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && !__builtin_sub_overflow(m_Value, value, &result.m_Value);
			return result;
		}

		/**
		 * @brief  Subtracts another `Size` value from `this`
		 * @param  value The size value to subtract
		 * @return A `Size` value that contains the `value` subtracted from the value of `this`
		 * @note   - The returned value will be marked as invalid if either `value` or `this` are invalid or if an overflow occurs
		 */
		constexpr Size subtract(const Size &value) const
		{
			return value.m_IsValid ? subtract(value.m_Value) : value;
		}

		/**
		 * @brief  Multiplies `this` by a value
		 * @param  value The value to multiply by
		 * @return A `Size` value that contains the value of `this` multiplied by `value`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if an overflow occurs
		 */
		constexpr Size multiply(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && !__builtin_mul_overflow(m_Value, value, &result.m_Value);
			return result;
		}

		/**
		 * @brief  Multiplies `this` by another `Size` value`
		 * @param  value The size value to multiply by
		 * @return A `Size` value that contains the value of `this` multiplied by `value`
		 * @note   - The returned value will be marked as invalid if either `value` or `this` are invalid or if an overflow occurs
		 */
		constexpr Size multiply(const Size &value) const
		{
			return value.m_IsValid ? multiply(value.m_Value) : value;
		}

		/**
		 * @brief  Divides `this` by a value
		 * @param  value The value to divide by
		 * @return A `Size` value that contains the value of `this` divided by `value`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if `value` is zero
		 */
		constexpr Size divide(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (value != 0);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value / value;
			}
			return result;
		}

		/**
		 * @brief  Divides `this` by another `Size` value`
		 * @param  value The size value to divide by
		 * @return A `Size` value that contains the value of `this` divided by `value`
		 * @note   - The returned value will be marked as invalid if either `value` or `this` are invalid or if `value` is zero
		 */
		constexpr Size divide(const Size &value) const
		{
			return value.m_IsValid ? divide(value.m_Value) : value;
		}

		/**
		 * @brief  Gets the remainder when `this` is divided by a value
		 * @param  value The value to divide by
		 * @return A `Size` value that contains the remainder when `this` is divided by `value`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if `value` is zero
		 */
		constexpr Size remainder(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (value != 0);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value % value;
			}
			return result;
		}

		/**
		 * @brief  Gets the remainder when `this` is divided by another `Size` value
		 * @param  value The size value to divide by
		 * @return A `Size` value that contains the remainder when `this` is divided by `value`
		 * @note   - The returned value will be marked as invalid if either `this` or `value` are invalid or if `value` is zero
		 */
		constexpr Size remainder(const Size &value) const
		{
			return value.m_IsValid ? remainder(value.m_Value) : value;
		}

		/**
		 * @brief  Gets the value of `this` rounded up to a non-zero multiple value
		 * @param  value The multiple value
		 * @return A `Size` value that contains the value of `this` rounded up to be a multiple of `value`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if `value` is zero
		 */
		constexpr Size roundUp(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (value != 0);
			if (result.m_IsValid)
			{
				if (m_Value == 0)
				{
					result = value;
				}
				else
				{
					size_t remainder = m_Value % value;
					result = (remainder == 0) ? m_Value : add(value - remainder);
				}
			}
			return result;
		}

		/**
		 * @brief  Gets the value of `this` rounded up to a non-zero multiple of another `Size` value
		 * @param  value The multiple size value
		 * @return A `Size` value that contains the value of `this` rounded up to be a multiple of `value`
		 * @note   - The returned value will be marked as invalid if either `this` or `value` are invalid or if `value` is zero
		 */
		constexpr Size roundUp(const Size &value) const
		{
			return value.m_IsValid ? roundUp(value.m_Value) : value;
		}

		/**
		 * @brief  Gets the value of `this` rounded down to a non-zero multiple value
		 * @param  value The multiple value
		 * @return A `Size` value that contains the value of `this` rounded down to be a multiple of `value`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if `value` is zero
		 */
		constexpr Size roundDown(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (value != 0);
			if (result.m_IsValid)
			{
				result.m_Value = m_Value - m_Value % value;
			}
			return result;
		}

		/**
		 * @brief  Gets the value of `this` rounded down to a non-zero multiple of another `Size` value
		 * @param  value The multiple size value
		 * @return A `Size` value that contains the value of `this` rounded down to be a multiple of `value`
		 * @note   - The returned value will be marked as invalid if either `this` or `value` are invalid or if `value` is zero
		 */
		constexpr Size roundDown(const Size &value) const
		{
			return value.m_IsValid ? roundDown(value.m_Value) : value;
		}

		/**
		 * @brief  Gets a percentage value of `this`
		 * @param  value The percent value
		 * @return A `Size` value that contains the percentage value of `this`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if `value` is greater than 100
		 */
		constexpr Size percent(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (value <= 100);
			result.m_Value = m_Value;
			return result.multiplyFraction(value, 100);
		}

		/**
		 * @brief  Gets a percentage value of `this`
		 * @param  value The percent size value
		 * @return A `Size` value that contains the percentage value of `this`
		 * @note   - The returned value will be marked as invalid if either `this` or `value` are invalid or if `percent` is greater than 100
		 */
		constexpr Size percent(const Size &value) const
		{
			return value.m_IsValid ? percent(value.m_Value) : value;
		}

		/**
		 * @brief  Gets the reverse percentage value of `this`
		 * @param  value The percent value
		 * @return A `Size` value that contains the reverse percentage value of `this`
		 * @note   - The returned value will be marked as invalid if `this` is invalid or if `value` is greater than 100
		 * @note   - The original value (before the percentage increase) can be determined by subtracting the result from `this` 
		 */
		constexpr Size reversePercent(size_t value) const
		{
			Size result;
			result.m_IsValid = m_IsValid && (value <= 100);
			result.m_Value = m_Value;
			return result.multiplyFraction(value, 100 + value);
		}

		/**
		 * @brief  Gets the reverse percentage value of `this`
		 * @param  value The percent size value
		 * @return A `Size` value that contains the reverse percentage value of `this`
		 * @note   - The returned value will be marked as invalid if either `this` or `percent` are invalid or if `percent` is greater than 100
		 * @note   - The original value (before the percentage increase) can be determined by subtracting the result from `this` 
		 */
		constexpr Size reversePercent(const Size &value) const
		{
			return value.m_IsValid ? reversePercent(value.m_Value) : value;
		}

	protected:
		/**
		 * @brief  Multiplies `this` by a fractional value
		 * @param  numerator   The value of the numerator
		 * @param  denominator The value of the denominator
		 * @return The value of `this` multiplied by the `numerator` and divided by the `denominator`
		 * @note   - The returned value will be marked as invalid if `this` is invalid.
		 * @note   - the value of the `numerator` parameter must be less than the value of the `denominator` parameter.
		 */
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
