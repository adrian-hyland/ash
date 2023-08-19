#pragma once

#include "ash.type.h"
#include "ash.encoding.utf16le.h"
#include "ash.string.h"


namespace Ash
{
	namespace Utf16le
	{
		template
		<
			typename ALLOCATION,
			typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
		>
		using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Utf16le>;


		template
		<
			size_t CAPACITY
		>
		using Buffer = Ash::String::Buffer<Ash::Encoding::Utf16le, CAPACITY>;


		using View = Ash::String::View<Ash::Encoding::Utf16le>;


		template
		<
			size_t MINIMUM_CAPACITY    = 32,
			size_t PERCENTAGE_INCREASE = 50,
			size_t BLOCK_SIZE          = 32
		>
		using String = Ash::String::Array<Ash::Encoding::Utf16le, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
	}
}
