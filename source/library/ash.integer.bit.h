#pragma once

#include <cstddef>
#include <climits>
#include "ash.type.h"


namespace Ash::Integer
{
	/**
	 * @brief  The size of an integer type in bits
	 * @tparam INTEGER The integer type
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsInteger<INTEGER>
	>
	constexpr size_t bitSize = sizeof(INTEGER) * CHAR_BIT;

	/**
	 * @brief  Gets a bit mask for an unsigned integer value
	 * @tparam INTEGER The unsigned integer type
	 * @param  from    The bit position of the start of the bit mask (0 is the least significant bit)
	 * @param  to      The bit position of the end of the bit mask (0 is the least significant bit)
	 * @return The bit mask. This is the unsigned integer value with all the bits set between bit positions `from` and `to`.
	 * @note   - The `from` and `to` bit positions are inclusive.
	 * @note   - The `from` and `to` parameters are interchangable. I.e. `getBitMask(from, to) == getBitMask(to, from)`
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr INTEGER getBitMask(size_t from, size_t to = bitSize<INTEGER> - 1)
	{
		if (from > to)
		{
			std::swap(from, to);
		}

		if (from >= Ash::Integer::bitSize<INTEGER>)
		{
			return 0;
		}

		size_t size = to - from;
		return ((size < Ash::Integer::bitSize<INTEGER> - 1) ? (INTEGER(1) << (size + 1)) - 1 : ~INTEGER(0)) << from;
	}

	/**
	 * @brief  Sets a bit in an unsigned integer value
	 * @tparam INTEGER The unsigned integer type
	 * @param  value   The unsigned integer value
	 * @param  bit     The bit position to set (0 is the least significant bit)
	 * @return The `value` with the `bit` set.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr INTEGER setBit(INTEGER value, size_t bit)
	{
		return value | Ash::Integer::getBitMask<INTEGER>(bit, bit);
	}

	/**
	 * @brief  Clears a bit in an unsigned integer value
	 * @tparam INTEGER The unsigned integer type
	 * @param  value   The unsigned integer value
	 * @param  bit     The bit position to clear (0 is the least significant bit)
	 * @return The `value` with the `bit` cleared.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr INTEGER clearBit(INTEGER value, size_t bit)
	{
		return value & ~Ash::Integer::getBitMask<INTEGER>(bit, bit);
	}

	/**
	 * @brief  Checks if a bit has been set in an unsigned integer value
	 * @tparam INTEGER The unsigned integer type
	 * @param  value   The unsigned integer value
	 * @param  bit     The bit position to check (0 is the least significant bit)
	 * @return `true` is returned if the `bit` in the `value` is set.
	 * @return `false` is returned if the `bit` in the `value` is clear.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr bool hasBitSet(INTEGER value, size_t bit)
	{
		return (value & Ash::Integer::getBitMask<INTEGER>(bit, bit)) != 0;
	}

	/**
	 * @brief  Returns the number of bits that are set/cleared in an unsigned integer value
	 * @tparam INTEGER The unsigned integer type
	 * @param  value The unsigned integer value
	 * @param  isSet Indicates whether to count the number of bits that are set (`true`) or the number of bits that are cleared (`false`)
	 * @return The number of bits in `value` that are set (if `isSet` is `true`).
	 * @return The number of bits in `value` that are cleared (if `isSet` is `false`).
	 * @note   The default value of `isSet` is `true`.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr size_t getBitCount(INTEGER value, bool isSet = true)
	{
		if (!isSet)
		{
			value = ~value;
		}

		if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned int>)
		{
			return __builtin_popcount(value);
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned long>)
		{
			return __builtin_popcountl(value);
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned long long>)
		{
			return __builtin_popcountll(value);
		}
	}

	/**
	 * @brief  Returns the number of leading (most significant) bits that are set/cleared in an unsigned integer
	 * @tparam INTEGER The unsigned integer type
	 * @param  value The unsigned integer value
	 * @param  isSet Indicates whether to count the number of bits that are set (`true`) or the number of bits that are cleared (`false`)
	 * @return The number of leading bits that are set (if `isSet` is true).
	 * @return The number of leading bits that are cleared (if `isSet` is false).
	 * @note   The default value of `isSet` is `false`.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr size_t getLeadingBitCount(INTEGER value, bool isSet = false)
	{
		if (isSet)
		{
			value = ~value;
		}

		if (value == 0)
		{
			return Ash::Integer::bitSize<INTEGER>;
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned int>)
		{
			return __builtin_clz(value) + Ash::Integer::bitSize<INTEGER> - Ash::Integer::bitSize<unsigned int>;
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned long>)
		{
			return __builtin_clzl(value) + Ash::Integer::bitSize<INTEGER> - Ash::Integer::bitSize<unsigned long>;
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned long long>)
		{
			return __builtin_clzll(value) + Ash::Integer::bitSize<INTEGER> - Ash::Integer::bitSize<unsigned long long>;
		}
	}

	/**
	 * @brief  Returns the number of trailing (least significant) bits that are set/cleared in an unsigned integer
	 * @tparam INTEGER The unsigned integer type
	 * @param  value The unsigned integer value
	 * @param  isSet Indicates whether to count the number of bits that are set (`true`) or the number of bits that are cleared (`false`)
	 * @return The number of trailing bits that are set (if `isSet` is true).
	 * @return The number of trailing bits that are cleared (if `isSet` is false).
	 * @note   The default value of `isSet` is `false`.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr size_t getTrailingBitCount(INTEGER value, bool isSet = false)
	{
		if (isSet)
		{
			value = ~value;
		}

		if (value == 0)
		{
			return Ash::Integer::bitSize<INTEGER>;
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned int>)
		{
			return __builtin_ctz(value);
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned long>)
		{
			return __builtin_ctzl(value);
		}
		else if constexpr (Ash::Integer::bitSize<INTEGER> <= Ash::Integer::bitSize<unsigned long long>)
		{
			return __builtin_ctzll(value);
		}
	}

	/**
	 * @brief  Gets the minimum number of bits that are required to hold an unsigned integer value
	 * @tparam INTEGER The unsigned integer type
	 * @param  value   The unsigned integer value
	 * @return The minimum number of bits that are required to hold the `value`.
	 * @note   - The returned value will always be at least 1. I.e. the minimum number of bits to hold a zero value is 1.
	 */
	template
	<
		typename INTEGER,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	constexpr size_t getBitLength(INTEGER value)
	{
		return (value != 0) ? Ash::Integer::bitSize<INTEGER> - Ash::Integer::getLeadingBitCount(value) : 1;
	}
}
