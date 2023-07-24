#pragma once

#include <cstdint>
#include <type_traits>


namespace Ash
{
	namespace Integer
	{
		template <typename INTEGER_TYPE>
		constexpr size_t getBitSize()
		{
			return sizeof(INTEGER_TYPE) * 8;
		}

		template <typename INTEGER_TYPE>
		using enable_if_unsigned = std::enable_if_t<std::is_unsigned<INTEGER_TYPE>::value, INTEGER_TYPE>;

		template <typename INTEGER_TYPE, typename _=enable_if_unsigned<INTEGER_TYPE>>
		constexpr INTEGER_TYPE getBitMask(size_t from, size_t to = getBitSize<INTEGER_TYPE>())
		{
			if (from > to)
			{
				std::swap(from, to);
			}

			if (from >= getBitSize<INTEGER_TYPE>())
			{
				return 0;
			}
			else if (to >= getBitSize<INTEGER_TYPE>() - 1)
			{
				return (from == 0) ? INTEGER_TYPE(-1) : INTEGER_TYPE(-1) << from;
			}
			else if (from == 0)
			{
				return (to == 0) ? ~INTEGER_TYPE(-2) : ~(INTEGER_TYPE(-2) << to);
			}
			else
			{
				return (INTEGER_TYPE(-1) << from) & ~(INTEGER_TYPE(-2) << to);
			}
		}

		template <typename INTEGER_TYPE, typename _=enable_if_unsigned<INTEGER_TYPE>>
		constexpr size_t getBitSize(INTEGER_TYPE value)
		{
			size_t size = getBitSize<INTEGER_TYPE>() / 2;

			for (size_t n = size; n > 0; n = n / 2)
			{
				if ((value & getBitMask<INTEGER_TYPE>(size)) == 0)
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

		template <typename INTEGER_TYPE, typename _=enable_if_unsigned<INTEGER_TYPE>>
		constexpr INTEGER_TYPE setBit(INTEGER_TYPE value, size_t bit)
		{
			return value | getBitMask<INTEGER_TYPE>(bit, bit);
		}

		template <typename INTEGER_TYPE, typename _=enable_if_unsigned<INTEGER_TYPE>>
		constexpr INTEGER_TYPE clearBit(INTEGER_TYPE value, size_t bit)
		{
			return value & ~getBitMask<INTEGER_TYPE>(bit, bit);
		}

		template <typename INTEGER_TYPE, typename _=enable_if_unsigned<INTEGER_TYPE>>
		constexpr bool hasBitSet(INTEGER_TYPE value, size_t bit)
		{
			return (value & getBitMask<INTEGER_TYPE>(bit, bit)) != 0;
		}
	}
}
