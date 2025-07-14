#pragma once

#include "ash.integer.value.h"


namespace Ash
{
	namespace Integer
	{
		template
		<
			typename INTEGER,
			typename = Ash::Type::IsInteger<INTEGER>
		>
		constexpr bool isEven(INTEGER value) { return value % 2 == 0; }

		template
		<
			typename INTEGER,
			typename = Ash::Type::IsInteger<INTEGER>
		>
		constexpr bool isOdd(INTEGER value) { return value % 2 != 0; }

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
}
