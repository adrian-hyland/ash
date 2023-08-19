#pragma once

#include "ash.type.h"
#include "ash.encoding.wide.h"
#include "ash.string.h"


namespace Ash
{
	namespace Wide
	{
		template
		<
			typename ALLOCATION,
			typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
		>
		using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Wide>;
	
		template
		<
			size_t CAPACITY
		>
		using Buffer = Ash::String::Buffer<Ash::Encoding::Wide, CAPACITY>;

		using View = Ash::String::View<Ash::Encoding::Wide>;

		template
		<
			size_t MINIMUM_CAPACITY    = 32,
			size_t PERCENTAGE_INCREASE = 50,
			size_t BLOCK_SIZE          = 32
		>
		using String = Ash::String::Array<Ash::Encoding::Wide, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
	}
}
