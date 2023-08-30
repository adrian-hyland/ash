#pragma once

#include <algorithm>
#include "ash.integer.bit.h"


namespace Ash
{
	namespace Integer
	{
		template
		<
			auto MIN_VALUE,
			auto MAX_VALUE,
			auto SCALAR = 1,
			auto IS_SIGNED = (MIN_VALUE < 0) || (MAX_VALUE < 0)
		>
		constexpr size_t getValueSize()
		{
			std::make_unsigned_t<decltype(MIN_VALUE)> absMinValue = (MIN_VALUE < 0) ? (IS_SIGNED ? -(MIN_VALUE + 1) : 0) : MIN_VALUE;
			std::make_unsigned_t<decltype(MAX_VALUE)> absMaxValue = (MAX_VALUE < 0) ? (IS_SIGNED ? -(MAX_VALUE + 1) : 0) : MAX_VALUE;

			size_t minByteSize = (Ash::Integer::getBitSize(absMinValue) * SCALAR + (IS_SIGNED ? 8 : 7)) / 8;
			size_t maxByteSize = (Ash::Integer::getBitSize(absMaxValue) * SCALAR + (IS_SIGNED ? 8 : 7)) / 8;

			size_t byteSize = std::max(minByteSize, maxByteSize);
			return (byteSize == 1) ? 8 : 8 << Ash::Integer::getBitSize(byteSize - 1);
		}

		template
		<
			size_t BIT_SIZE
		>
		struct ValueProperty
		{
			using Type = int;
		};

		template <> struct ValueProperty<8>
		{
			using Type = int8_t;
		};

		template <> struct ValueProperty<16>
		{
			using Type = int16_t;
		};

		template <> struct ValueProperty<32>
		{
			using Type = int32_t;
		};

		template <> struct ValueProperty<64>
		{
			using Type = uint64_t;
		};

		template
		<
			size_t BIT_SIZE,
			bool   IS_SIGNED
		>
		struct SignedProperty
		{
		};

		template
		<
			size_t BIT_SIZE
		>
		struct SignedProperty<BIT_SIZE, true>
		{
			using Type = std::make_signed_t<typename ValueProperty<BIT_SIZE>::Type>;
		};

		template
		<
			size_t BIT_SIZE
		>
		struct SignedProperty<BIT_SIZE, false>
		{
			using Type = std::make_unsigned_t<typename ValueProperty<BIT_SIZE>::Type>;
		};


		template
		<
			auto MIN_VALUE,
			auto MAX_VALUE,
			auto SCALAR = 1,
			auto IS_SIGNED = (MIN_VALUE < 0) || (MAX_VALUE < 0)
		>
		using Value = typename SignedProperty<getValueSize<MIN_VALUE, MAX_VALUE, SCALAR, IS_SIGNED>(), IS_SIGNED>::Type;
	}
}
