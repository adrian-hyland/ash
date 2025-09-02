#pragma once

#include "ash.integer.value.h"


namespace Ash::Integer
{
	/**
	 * @brief  Checks if an integer value is even
	 * @tparam INTEGER The integer type
	 * @param  value   The integer value
	 * @return `true` is returned if the integer value is even.
	 * @return `false` is returned if the integer value is odd.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsInteger<INTEGER>
	>
	constexpr bool isEven(INTEGER value) { return value % 2 == 0; }

	/**
	 * @brief  Checks if an integer value is odd
	 * @tparam INTEGER The integer type
	 * @param  value   The integer value
	 * @return `true` is returned if the integer value is odd.
	 * @return `false` is returned if the integer value is even.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsInteger<INTEGER>
	>
	constexpr bool isOdd(INTEGER value) { return value % 2 != 0; }

	/**
	 * @brief  Calculates the greatest common divisor between two integer values
	 * @tparam INTEGER_A The integer type of the first value
	 * @tparam INTEGER_B The integer type of the second value
	 * @param  a         The first integer value
	 * @param  b         The second integer value
	 * @return The greatest common divisor.
	 * @note   - A zero value is returned if both parameters, `a` and `b`, are zero
	 */
	template
	<
		typename INTEGER_A,
		typename INTEGER_B,
		typename INTEGER_RETURN = typename Ash::Integer::Unsigned::WithMaxBitSize<Ash::Integer::bitSize<INTEGER_A>, Ash::Integer::bitSize<INTEGER_B>>::Type,
		typename = Ash::Type::IsInteger<INTEGER_A>,
		typename = Ash::Type::IsInteger<INTEGER_B>
	>
	constexpr INTEGER_RETURN greatestCommonDivisor(INTEGER_A a, INTEGER_B b)
	{
		size_t d = 0;

		if (a == 0)
		{
			return b;
		}
		if (b == 0)
		{
			return a;
		}

		INTEGER_RETURN a1;
		if constexpr (Ash::Type::isUnsignedInteger<INTEGER_A>)
		{
			a1 = a;
		}
		else
		{
			a1 = (a > 0) ? a : -INTEGER_RETURN(a);
		}

		INTEGER_RETURN b1;
		if constexpr (Ash::Type::isUnsignedInteger<INTEGER_B>)
		{
			b1 = b;
		}
		else
		{
			b1 = (b > 0) ? b : -INTEGER_RETURN(b);
		}

		size_t na = Ash::Integer::getTrailingBitCount(a1);
		size_t nb = Ash::Integer::getTrailingBitCount(b1);
		a1 = a1 >> na;
		b1 = b1 >> nb;
		d = std::min(na, nb);

		while (a1 != b1)
		{
			if (a1 > b1)
			{
				a1 = a1 - b1;
				a1 = a1 >> Ash::Integer::getTrailingBitCount(a1);
			}
			else
			{
				b1 = b1 - a1;
				b1 = b1 >> Ash::Integer::getTrailingBitCount(b1);
			}
		}

		return a1 << d;
	}

	/**
	 * @brief  Calculates the greatest common divisor between multiple (more than 2) integer values
	 * @tparam INTEGER_A    The integer type of the first value
	 * @tparam INTEGER_B    The integer type of the second value
	 * @tparam ...INTEGER_N The integer types of the rest of the values
	 * @param  a            The first integer value
	 * @param  b            The second integer value
	 * @param ...n          The rest of the integer values
	 * @return The greatest common divisor.
	 * @note   - A zero value is returned if all of the parameters are zero
	 */
	template
	<
		typename INTEGER_A,
		typename INTEGER_B,
		typename ...INTEGER_N
	>
	constexpr auto greatestCommonDivisor(INTEGER_A a, INTEGER_B b, INTEGER_N ...n)
	{
		return greatestCommonDivisor(greatestCommonDivisor(a, b), n...);
	}
}
