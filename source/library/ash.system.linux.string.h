#pragma once

#include "ash.wide.h"
#include "ash.utf8.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			class String : protected Ash::String::ArrayBuffer<Ash::Encoding::Utf8, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
			{
			public:
				using Content = Ash::String::ArrayBuffer<Ash::Encoding::Utf8, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
				using Encoding = typename Content::Encoding;
				using Code = typename Encoding::Code;
				using Character = typename Encoding::Character;

				constexpr String() : Content()
				{
					Content::append('\0');
				}

				constexpr String(const Ash::Encoding::CodeUnit8 *value, Ash::Unicode::Character replacementCharacter = Character::replacement) : Content()
				{
					convert(Ash::Utf8::View(value), replacementCharacter);
				}

				constexpr String(const Ash::Encoding::CodeUnitWide *value, Ash::Unicode::Character replacementCharacter = Character::replacement) : Content()
				{
					convert(Ash::Wide::View(value), replacementCharacter);
				}

				template
				<
					typename FROM_ALLOCATION,
					typename FROM_ENCODING,
					typename = Ash::Type::IsClass<FROM_ALLOCATION, Ash::Memory::Generic::Allocation>,
					typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
				>
				constexpr String(const Ash::String::Value<FROM_ALLOCATION, FROM_ENCODING> &value, Ash::Unicode::Character replacementCharacter = Character::replacement) : Content()
				{
					convert(value, replacementCharacter);
				}

				constexpr operator const Code * () const { return Content::at(0); }

			protected:
				template
				<
					typename FROM_ALLOCATION,
					typename FROM_ENCODING,
					typename = Ash::Type::IsClass<FROM_ALLOCATION, Ash::Memory::Generic::Allocation>,
					typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
				>
				constexpr void convert(const Ash::String::Value<FROM_ALLOCATION, FROM_ENCODING> &value, Ash::Unicode::Character replacementCharacter)
				{
					value.convertTo(*this, replacementCharacter);
					Content::append('\0');
				}
			};
		}
	}
}
