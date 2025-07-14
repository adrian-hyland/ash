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
			a1 = (a > 0) ? a : INTEGER_RETURN(-a);
		}

		INTEGER_RETURN b1;			
		if constexpr (Ash::Type::isUnsignedInteger<INTEGER_B>)
		{
			b1 = b;
		}
		else
		{
			b1 = (b > 0) ? b : INTEGER_RETURN(-b);
		}

		while (Ash::Integer::isEven(a1) && Ash::Integer::isEven(b1))
		{
			a1 = a1 / 2;
			b1 = b1 / 2;
			d++;
		}

		while (Ash::Integer::isEven(a1))
		{
			a1 = a1 / 2;
		}

		while (Ash::Integer::isEven(b1))
		{
			b1 = b1 / 2;
		}

		while (a1 != b1)
		{
			if (a1 > b1)
			{
				a1 = a1 - b1;
				do
				{
					a1 = a1 / 2;
				}
				while (Ash::Integer::isEven(a1));
			}
			else
			{
				b1 = b1 - a1;
				do
				{
					b1 = b1 / 2;
				}
				while (Ash::Integer::isEven(b1));
			}
		}
		return a1 << d;
	}
}
