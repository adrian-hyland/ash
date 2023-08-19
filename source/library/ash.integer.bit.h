#pragma once

#include <cstdint>
#include "ash.type.h"


namespace Ash
{
	namespace Integer
	{
		template
		<
			typename INTEGER,
			typename = Ash::Type::IsInteger<INTEGER>
		>
		constexpr size_t getBitSize()
		{
			return sizeof(INTEGER) * 8;
		}

		template
		<
			typename INTEGER,
			typename = Ash::Type::IsUnsignedInteger<INTEGER>
		>
		constexpr INTEGER getBitMask(size_t from, size_t to = getBitSize<INTEGER>())
		{
			if (from > to)
			{
				std::swap(from, to);
			}

			if (from >= getBitSize<INTEGER>())
			{
				return 0;
			}
			else if (to >= getBitSize<INTEGER>() - 1)
			{
				return (from == 0) ? INTEGER(-1) : INTEGER(-1) << from;
			}
			else if (from == 0)
			{
				return (to == 0) ? ~INTEGER(-2) : ~(INTEGER(-2) << to);
			}
			else
			{
				return (INTEGER(-1) << from) & ~(INTEGER(-2) << to);
			}
		}

		template
		<
			typename INTEGER,
			typename = Ash::Type::IsUnsignedInteger<INTEGER>
		>
		constexpr size_t getBitSize(INTEGER value)
		{
			size_t size = getBitSize<INTEGER>() / 2;

			for (size_t n = size; n > 0; n = n / 2)
			{
				if ((value & getBitMask<INTEGER>(size)) == 0)
				{
					size = size - n / 2;
				}
				else
				{
					size = size + (n + 1) / 2;
				}
			}

			return size;
		}

		template
		<
			typename INTEGER,
			typename = Ash::Type::IsUnsignedInteger<INTEGER>
		>
		constexpr INTEGER setBit(INTEGER value, size_t bit)
		{
			return value | getBitMask<INTEGER>(bit, bit);
		}

		template
		<
			typename INTEGER,
			typename = Ash::Type::IsUnsignedInteger<INTEGER>
		>
		constexpr INTEGER clearBit(INTEGER value, size_t bit)
		{
			return value & ~getBitMask<INTEGER>(bit, bit);
		}

		template
		<
			typename INTEGER,
			typename = Ash::Type::IsUnsignedInteger<INTEGER>
		>
		constexpr bool hasBitSet(INTEGER value, size_t bit)
		{
			return (value & getBitMask<INTEGER>(bit, bit)) != 0;
		}
	}
}
