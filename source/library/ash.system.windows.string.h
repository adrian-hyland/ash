#pragma once

#include "ash.wide.h"
#include "ash.utf8.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			class String : protected Ash::Wide::StringBuffer<CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
			{
			public:
				using Content = Ash::Wide::StringBuffer<CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

				constexpr String() : Content()
				{
					Content::append('\0');
				}

				constexpr String(const Ash::Encoding::CodeUnit8 *value, Ash::Unicode::Character replacementCharacter = Content::Encoding::Character::replacement) : Content()
				{
					convert(Ash::Utf8::View(value), replacementCharacter);
				}

				constexpr String(const Ash::Encoding::CodeUnitWide *value, Ash::Unicode::Character replacementCharacter = Content::Encoding::Character::replacement) : Content()
				{
					convert(Ash::Wide::View(value), replacementCharacter);
				}

				template
				<
					typename FROM_ENCODING,
					typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
				>
				constexpr String(Ash::String::View<FROM_ENCODING> value, Ash::Unicode::Character replacementCharacter = Content::Encoding::Character::replacement) : Content()
				{
					convert(value, replacementCharacter);
				}

				constexpr operator const typename Content::Encoding::Code *() const { return Content::at(0); }

			protected:
				template
				<
					typename FROM_ENCODING,
					typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
				>
				constexpr void convert(Ash::String::View<FROM_ENCODING> value, Ash::Unicode::Character replacementCharacter)
				{
					value.convertTo(*this, replacementCharacter);
					Content::append('\0');
				}
			};
		}
	}
}
