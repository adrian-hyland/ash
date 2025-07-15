#pragma once

#include <algorithm>
#include <cstdint>
#include "ash.integer.bit.h"


namespace Ash::Integer::Signed
{
	/**
	 * @brief  Provides the signed integer type that is capable of represnting a value with a particular bit size
	 * @tparam BIT_SIZE The bit size
	 */
	template
	<
		size_t BIT_SIZE
	>
	struct WithBitSize
	{
	};

	/**
	 * @brief Provides the signed integer type that is capable of representing an 8 bit value
	 */
	template <> struct WithBitSize<8>
	{
		/**
		 * @brief The signed integer type for an 8 bit value
		 */
		using Type = int8_t;
	};

	/**
	 * @brief Provides the signed integer type that is capable of representing a 16 bit value
	 */
	template <> struct WithBitSize<16>
	{
		/**
		 * @brief The signed integer type for a 16 bit value
		 */
		using Type = int16_t;
	};

	/**
	 * @brief Provides the signed integer type that is capable of representing a 32 bit value
	 */
	template <> struct WithBitSize<32>
	{
		/**
		 * @brief The signed integer type for a 32 bit value
		 */
		using Type = int32_t;
	};

	/**
	 * @brief Provides the signed integer type that is capable of representing a 64 bit value
	 */
	template <> struct WithBitSize<64>
	{
		/**
		 * @brief The signed integer type for a 64 bit value
		 */
		using Type = int64_t;
	};

	/**
	 * @brief Provides the signed integer type that has the maximum of a number of bit sizes
	 * @tparam BIT_SIZE_1    The first bit size
	 * @tparam BIT_SIZE_2    The second bit size
	 * @tparam ...BIT_SIZE_N The rest of the bit sizes
	 */
	template
	<
		size_t BIT_SIZE_1,
		size_t BIT_SIZE_2,
		size_t ...BIT_SIZE_N
	>
	struct WithMaxBitSize
	{
		/**
		 * @brief The signed integer type that is the maximum of `BIT_SIZE_1`, `BIT_SIZE_2` and `...BIT_SIZE_N` in size
		 */
		using Type = typename Ash::Integer::Signed::WithMaxBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2), BIT_SIZE_N...>::Type;
	};

	/**
	 * @brief Provides the signed integer type that has the maximum of two bit sizes
	 * @tparam BIT_SIZE_1 The first bit size
	 * @tparam BIT_SIZE_2 The second bit size
	 */
	template
	<
		size_t BIT_SIZE_1,
		size_t BIT_SIZE_2
	>
	struct WithMaxBitSize<BIT_SIZE_1, BIT_SIZE_2>
	{
		/**
		 * @brief The signed integer type that is the maximum of `BIT_SIZE_1` and `BIT_SIZE_2` in size
		 */
		using Type = typename Ash::Integer::Signed::WithBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2)>::Type;
	};
}

namespace Ash::Integer::Unsigned
{
	/**
	 * @brief  Provides the unsigned integer type that is capable of representing a value with a particular bit size
	 * @tparam BIT_SIZE The bit size
	 */
	template
	<
		size_t BIT_SIZE
	>
	struct WithBitSize
	{
	};

	/**
	 * @brief Provides the unsigned integer type that is capable of representing an 8 bit value
	 */
	template <> struct WithBitSize<8>
	{
		/**
		 * @brief The unsigned integer type for an 8 bit value
		 */
		using Type = uint8_t;
	};

	/**
	 * @brief Provides the unsigned integer type that is capable of representing a 16 bit value
	 */
	template <> struct WithBitSize<16>
	{
		/**
		 * @brief The unsigned integer type for a 16 bit value
		 */
		using Type = uint16_t;
	};

	/**
	 * @brief Provides the unsigned integer type that is capable of representing a 32 bit value
	 */
	template <> struct WithBitSize<32>
	{
		/**
		 * @brief The unsigned integer type for a 32 bit value
		 */
		using Type = uint32_t;
	};

	/**
	 * @brief Provides the unsigned integer type that is capable of representing a 64 bit value
	 */
	template <> struct WithBitSize<64>
	{
		/**
		 * @brief The unsigned integer type for a 64 bit value
		 */
		using Type = uint64_t;
	};

	/**
	 * @brief Provides the unsigned integer type that has the maximum of a number of bit sizes
	 * @tparam BIT_SIZE_1    The first bit size
	 * @tparam BIT_SIZE_2    The second bit size
	 * @tparam ...BIT_SIZE_N The rest of the bit sizes
	 */
	template
	<
		size_t BIT_SIZE_1,
		size_t BIT_SIZE_2,
		size_t ...BIT_SIZE_N
	>
	struct WithMaxBitSize
	{
		/**
		 * @brief The unsigned integer type that is the maximum of `BIT_SIZE_1`, `BIT_SIZE_2` and `...BIT_SIZE_N` in size
		 */
		using Type = typename Ash::Integer::Unsigned::WithMaxBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2), BIT_SIZE_N...>::Type;
	};

	/**
	 * @brief Provides the unsigned integer type that has the maximum of two bit sizes
	 * @tparam BIT_SIZE_1 The first bit size
	 * @tparam BIT_SIZE_2 The second bit size
	 */
	template
	<
		size_t BIT_SIZE_1,
		size_t BIT_SIZE_2
	>
	struct WithMaxBitSize<BIT_SIZE_1, BIT_SIZE_2>
	{
		/**
		 * @brief The unsigned integer type that is the maximum of `BIT_SIZE_1` and `BIT_SIZE_2` in size
		 */
		using Type = typename Ash::Integer::Unsigned::WithBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2)>::Type;
	};
}

namespace Ash::Integer::Value
{
	/**
	 * @brief  Gets the number of bits that are required to represent a range of integer values
	 * @tparam MIN_VALUE The integer type of the `minValue` parameter
	 * @tparam MAX_VALUE The integer type of the `maxValue` parameter
	 * @param  minValue  The minimum value of the range
	 * @param  maxValue  The maximum value of the range
	 * @param  isSigned  Indicates whether signed integer values are also required to be represented
	 * @return The required number of bits that are able to represent the range
	 */
	template
	<
		typename MIN_VALUE,
		typename MAX_VALUE,
		typename = Ash::Type::IsInteger<MIN_VALUE>,
		typename = Ash::Type::IsInteger<MAX_VALUE>
	>
	constexpr size_t getRangeBitSize(MIN_VALUE minValue, MAX_VALUE maxValue, bool isSigned = Ash::Type::isSignedInteger<MIN_VALUE> || Ash::Type::isSignedInteger<MAX_VALUE>)
	{
		std::make_unsigned_t<MIN_VALUE> absMinValue = (minValue < 0) ? (isSigned ? -(minValue + 1) : 0) : minValue;
		std::make_unsigned_t<MAX_VALUE> absMaxValue = (maxValue < 0) ? (isSigned ? -(maxValue + 1) : 0) : maxValue;

		size_t minByteSize = (Ash::Integer::getBitLength(absMinValue) + (isSigned ? 8 : 7)) / 8;
		size_t maxByteSize = (Ash::Integer::getBitLength(absMaxValue) + (isSigned ? 8 : 7)) / 8;

		size_t byteSize = std::max(minByteSize, maxByteSize);
		return (byteSize == 1) ? 8 : 8 << Ash::Integer::getBitLength(byteSize - 1);
	}

	/**
	 * @brief  Provides the integer type that is capable of representing a value with a particular bit size
	 * @tparam BIT_SIZE  The bit size
	 * @tparam IS_SIGNED Indicates if the integer type needs to be signed
	 */
	template
	<
		size_t BIT_SIZE,
		bool   IS_SIGNED
	>
	struct WithBitSize
	{
	};

	/**
	 * @brief  Provides the signed integer type that is capable of representing a value with a particular bit size
	 * @tparam BIT_SIZE The bit size
	 */
	template
	<
		size_t BIT_SIZE
	>
	struct WithBitSize<BIT_SIZE, true>
	{
		/**
		 * @brief The signed integer type that is capable of representing a value that is `BIT_SIZE` bits in size
		 */
		using Type = typename Ash::Integer::Signed::WithBitSize<BIT_SIZE>::Type;
	};

	/**
	 * @brief  Provides the signed integer type that is capable of representing a value with a particular bit size
	 * @tparam BIT_SIZE The bit size
	 */
	template
	<
		size_t BIT_SIZE
	>
	struct WithBitSize<BIT_SIZE, false>
	{
		/**
		 * @brief The unsigned integer type that is capable of representing a value that is `BIT_SIZE` bits in size
		 */
		using Type = typename Ash::Integer::Unsigned::WithBitSize<BIT_SIZE>::Type;
	};

	/**
	 * @brief  Provides the integer type that is able to represent a range of integer values
	 * @tparam MIN_VALUE The minimum value of the range
	 * @tparam MAX_VALUE The maximum value of the range
	 * @note   - The values of the `MIN_VALUE` and `MAX_VALUE` template parameters are inclusive
	 */
	template
	<
		auto     MIN_VALUE,
		auto     MAX_VALUE,
		bool     IS_SIGNED = (MIN_VALUE < 0) || (MAX_VALUE < 0),
		typename = Ash::Type::IsInteger<decltype(MIN_VALUE)>,
		typename = Ash::Type::IsInteger<decltype(MAX_VALUE)>
	>
	struct WithRange
	{
		/**
		 * @brief The integer type that is capable of representing an integer value in the range `MIN_VALUE` to `MAX_VALUE`
		 */
		using Type = typename Ash::Integer::Value::WithBitSize<Ash::Integer::Value::getRangeBitSize(MIN_VALUE, MAX_VALUE, IS_SIGNED), IS_SIGNED>::Type;

		/**
		 * @brief The minimum value of the range
		 */
		static constexpr Type minValue = MIN_VALUE;

		/**
		 * @brief The maximum value of the range
		 */
		static constexpr Type maxValue = MAX_VALUE;
	};
}
