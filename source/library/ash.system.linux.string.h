#pragma once

#include "ash.ascii.h"
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
			class String : public Ash::Utf8::StringBuffer<CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
			{
			public:
				using Content = Ash::Utf8::StringBuffer<CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
				using Encoding = typename Content::Encoding;
				using Code = typename Encoding::Code;
				using Character = typename Encoding::Character;

				constexpr String() : Content()
				{
					Content::append('\0').assertErrorNotSet();
				}

				template
				<
					typename VALUE,
					typename = Ash::Type::IsStringLiteral<VALUE>
				>
				constexpr String(VALUE value, bool replaceInvalidCharacter = true) : Content()
				{
					convertFrom(value, replaceInvalidCharacter).throwOnError();
				}

				template
				<
					typename FROM_ENCODING,
					typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
				>
				constexpr String(const Ash::String::View<FROM_ENCODING> value, bool replaceInvalidCharacter = true) : Content()
				{
					convertFrom(value, replaceInvalidCharacter).throwOnError();
				}

				template
				<
					typename VALUE,
					typename = Ash::Type::IsStringLiteral<VALUE>
				>
				[[nodiscard]]
				constexpr Ash::Error::Value convertFrom(VALUE value, bool replaceInvalidCharacter = true)
				{
					return convertFrom(Ash::String::view(value), replaceInvalidCharacter);
				}

				template
				<
					typename FROM_ENCODING,
					typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
				>
				[[nodiscard]]
				constexpr Ash::Error::Value convertFrom(const Ash::String::View<FROM_ENCODING> value, bool replaceInvalidCharacter = true)
				{
					Content content;

					Ash::Error::Value error = content.convertFrom(value, replaceInvalidCharacter);
					if (!error)
					{
						error = content.append('\0');
						if (!error)
						{
							Content::moveFrom(content);
						}
					}

					return error;
				}

				constexpr operator const Code *() const { return Content::at(0); }

				constexpr operator const char *() const { return reinterpret_cast<const char *>(Content::at(0)); }
			};
		}
	}
}
