#pragma once

#include "ash.ascii.h"
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
			class String : public Ash::Wide::StringBuffer<CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
			{
			public:
				using Content = Ash::Wide::StringBuffer<CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
				using Encoding = typename Content::Encoding;
				using Code = typename Encoding::Code;
				using Character = typename Encoding::Character;

				constexpr String() : Content()
				{
					Content::append('\0');
				}

				template
				<
					typename VALUE,
					typename = Ash::Type::IsStringLiteral<VALUE>
				>
				constexpr String(VALUE value, Ash::Unicode::Character replacementCharacter = Character::replacement) : Content()
				{
					convert(Ash::String::View<typename Ash::String::Literal<VALUE>::Encoding>(value), replacementCharacter);
				}

				template
				<
					typename FROM_ENCODING,
					typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
				>
				constexpr String(Ash::String::View<FROM_ENCODING> value, Ash::Unicode::Character replacementCharacter = Character::replacement) : Content()
				{
					convert(value, replacementCharacter);
				}

				constexpr operator const Code *() const { return Content::at(0); }

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
