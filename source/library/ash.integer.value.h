#pragma once

#include <algorithm>
#include <cstdint>
#include "ash.integer.bit.h"


namespace Ash
{
	namespace Integer
	{
		namespace Signed
		{
			template
			<
				size_t BIT_SIZE
			>
			struct WithBitSize
			{
			};

			template <> struct WithBitSize<8>
			{
				using Type = int8_t;
			};

			template <> struct WithBitSize<16>
			{
				using Type = int16_t;
			};

			template <> struct WithBitSize<32>
			{
				using Type = int32_t;
			};

			template <> struct WithBitSize<64>
			{
				using Type = int64_t;
			};

			template
			<
				size_t BIT_SIZE_1,
				size_t BIT_SIZE_2,
				size_t ...BIT_SIZE_N
			>
			struct WithMaxBitSize
			{
				using Type = Ash::Integer::Signed::WithMaxBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2), BIT_SIZE_N...>::Type;
			};

			template
			<
				size_t BIT_SIZE_1,
				size_t BIT_SIZE_2
			>
			struct WithMaxBitSize<BIT_SIZE_1, BIT_SIZE_2>
			{
				using Type = Ash::Integer::Signed::WithBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2)>::Type;
			};
		}

		namespace Unsigned
		{
			template
			<
				size_t BIT_SIZE
			>
			struct WithBitSize
			{
			};

			template <> struct WithBitSize<8>
			{
				using Type = uint8_t;
			};

			template <> struct WithBitSize<16>
			{
				using Type = uint16_t;
			};

			template <> struct WithBitSize<32>
			{
				using Type = uint32_t;
			};

			template <> struct WithBitSize<64>
			{
				using Type = uint64_t;
			};

			template
			<
				size_t BIT_SIZE_1,
				size_t BIT_SIZE_2,
				size_t ...BIT_SIZE_N
			>
			struct WithMaxBitSize
			{
				using Type = Ash::Integer::Unsigned::WithMaxBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2), BIT_SIZE_N...>::Type;
			};

			template
			<
				size_t BIT_SIZE_1,
				size_t BIT_SIZE_2
			>
			struct WithMaxBitSize<BIT_SIZE_1, BIT_SIZE_2>
			{
				using Type = Ash::Integer::Unsigned::WithBitSize<std::max(BIT_SIZE_1, BIT_SIZE_2)>::Type;
			};
		}

		namespace Value
		{
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

			template
			<
				size_t BIT_SIZE,
				bool   IS_SIGNED
			>
			struct WithBitSize
			{
			};

			template
			<
				size_t BIT_SIZE
			>
			struct WithBitSize<BIT_SIZE, true>
			{
				using Type = Ash::Integer::Signed::WithBitSize<BIT_SIZE>::Type;
			};

			template
			<
				size_t BIT_SIZE
			>
			struct WithBitSize<BIT_SIZE, false>
			{
				using Type = Ash::Integer::Unsigned::WithBitSize<BIT_SIZE>::Type;
			};

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
				using Type = Ash::Integer::Value::WithBitSize<Ash::Integer::Value::getRangeBitSize(MIN_VALUE, MAX_VALUE, IS_SIGNED), IS_SIGNED>::Type;

				static constexpr Type minValue = MIN_VALUE;
				static constexpr Type maxValue = MAX_VALUE;
			};
		}
	}
}
