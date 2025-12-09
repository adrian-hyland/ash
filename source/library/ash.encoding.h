#pragma once

#include "ash.type.h"
#include "ash.memory.h"
#include "ash.unicode.character.h"
#include "ash.encoding.error.h"


namespace Ash
{
	namespace Generic
	{
		class Encoding {};
	}

	namespace Encoding
	{
		#if STD < 20
		using CodeUnit8 = char;
		#else
		using CodeUnit8 = char8_t;
		#endif

		using CodeUnit16 = char16_t;

		using CodeUnit32 = char32_t;

		using CodeUnitWide = wchar_t;

		template
		<
			typename FROM_ENCODING,
			typename TO_ENCODING,
			typename TO_ALLOCATION,
			typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>,
			typename = Ash::Type::IsClass<TO_ENCODING, Ash::Generic::Encoding>,
			typename = Ash::Type::IsClass<TO_ALLOCATION, Ash::Memory::Generic::Allocation>
		>
		[[nodiscard]]
		constexpr Ash::Error::Value convert(Ash::Memory::View<typename FROM_ENCODING::Code> from, size_t &fromOffset, Ash::Memory::Value<TO_ALLOCATION, typename TO_ENCODING::Code> &to, size_t &toOffset, bool replaceInvalidCharacter = true)
		{
			if (fromOffset > from.getLength())
			{
				return Ash::Memory::Error::readAccessOutOfBound;
			}

			while (fromOffset < from.getLength())
			{
				typename FROM_ENCODING::Character fromCharacter;
				typename TO_ENCODING::Character toCharacter;

				Ash::Error::Value error = FROM_ENCODING::decodeNext(from, fromOffset, fromCharacter);
				if (error.hasErrorSet())
				{
					if ((error.getCategory() != &Ash::Encoding::Error::category) || !replaceInvalidCharacter)
					{
						return error;
					}

					fromCharacter = Ash::Unicode::Character::replacement;
				}

				toCharacter = fromCharacter;

				error = to.set(toOffset, toCharacter);
				if (error.hasErrorSet())
				{
					return error;
				}

				fromOffset = fromOffset + ((fromCharacter.getLength() != 0) ? fromCharacter.getLength() : FROM_ENCODING::minSize);
				toOffset = toOffset + toCharacter.getLength();
			}

			return Ash::Error::none;
		}

		template
		<
			typename FROM_ENCODING,
			typename TO_ENCODING,
			typename TO_ALLOCATION,
			typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>,
			typename = Ash::Type::IsClass<TO_ENCODING, Ash::Generic::Encoding>,
			typename = Ash::Type::IsClass<TO_ALLOCATION, Ash::Memory::Generic::Allocation>
		>
		[[nodiscard]]
		constexpr Ash::Error::Value convert(Ash::Memory::View<typename FROM_ENCODING::Code> from, Ash::Memory::Value<TO_ALLOCATION, typename TO_ENCODING::Code> &to, bool replaceInvalidCharacter = true)
		{
			size_t fromOffset = 0;
			size_t toOffset = 0;

			to.clear();

			return convert<FROM_ENCODING, TO_ENCODING, TO_ALLOCATION>(from, fromOffset, to, toOffset, replaceInvalidCharacter);
		}

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		constexpr size_t find(Ash::Memory::View<typename ENCODING::Code> value, size_t offset, Ash::Unicode::Character character)
		{
			typename ENCODING::Character characterEncoding(character);

			if (characterEncoding.getLength() <= value.getLength())
			{
				if (characterEncoding.getLength() == 1)
				{
					return value.find(offset, *characterEncoding.at(0));
				}
				else if (characterEncoding.getLength() > 1)
				{
					if constexpr (ENCODING::isBigEndian)
					{
						typename ENCODING::Code characterCode = *characterEncoding.at(ENCODING::minSize - 1);
						for (; offset <= value.getLength() - characterEncoding.getLength(); offset = offset + ENCODING::minSize)
						{
							offset = value.template find<ENCODING::minSize, ENCODING::minSize - 1>(offset, characterCode);
							if (offset == value.getLength())
							{
								return offset;
							}
							if (value.match(offset - (ENCODING::minSize - 1), characterEncoding) == characterEncoding.getLength())
							{
								return offset - (ENCODING::minSize - 1);
							}
						}
					}
					else
					{
						typename ENCODING::Code characterCode = *characterEncoding.at(0);
						for (; offset <= value.getLength() - characterEncoding.getLength(); offset = offset + ENCODING::minSize)
						{
							offset = value.template find<ENCODING::minSize>(offset, characterCode);
							if (offset == value.getLength())
							{
								return offset;
							}
							if (value.match(offset, characterEncoding) == characterEncoding.getLength())
							{
								return offset;
							}
						}
					}
				}
			}

			return value.getLength();
		}

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		constexpr size_t reverseFind(Ash::Memory::View<typename ENCODING::Code> value, size_t offset, Ash::Unicode::Character character)
		{
			typename ENCODING::Character characterEncoding(character);

			if (characterEncoding.getLength() <= value.getLength())
			{
				if (characterEncoding.getLength() == 1)
				{
					return value.reverseFind(offset, *characterEncoding.at(0));
				}
				else if (characterEncoding.getLength() > 1)
				{
					if constexpr (ENCODING::isBigEndian)
					{
						if (offset < value.getLength())
						{
							typename ENCODING::Code characterCode = *characterEncoding.at(ENCODING::minSize - 1);
							offset = offset + ENCODING::minSize - 1 - (offset % ENCODING::minSize);
							for (; offset >= ENCODING::minSize; offset = offset - ENCODING::minSize)
							{
								offset = value.template reverseFind<ENCODING::minSize, ENCODING::minSize - 1>(offset, characterCode);
								if (offset == value.getLength())
								{
									return offset;
								}
								if (value.match(offset - (ENCODING::minSize - 1), characterEncoding) == characterEncoding.getLength())
								{
									return offset - (ENCODING::minSize - 1);
								}
							}
							if ((offset == ENCODING::minSize - 1) && (value.match(0, characterEncoding) == characterEncoding.getLength()))
							{
								return 0;
							}
						}
					}
					else
					{
						if (offset < value.getLength())
						{
							typename ENCODING::Code characterCode = *characterEncoding.at(0);
							offset = offset - (offset % ENCODING::minSize);
							for (; offset >= ENCODING::minSize; offset = offset - ENCODING::minSize)
							{
								offset = value.template reverseFind<ENCODING::minSize>(offset, characterCode);
								if (offset == value.getLength())
								{
									return offset;
								}
								if (value.match(offset, characterEncoding) == characterEncoding.getLength())
								{
									return offset;
								}
							}
							if ((offset == 0) && (value.match(0, characterEncoding) == characterEncoding.getLength()))
							{
								return 0;
							}
						}
					}
				}
			}

			return value.getLength();
		}

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		constexpr bool contains(Ash::Memory::View<typename ENCODING::Code> value, Ash::Unicode::Character character)
		{
			return find<ENCODING>(value, 0, character) < value.getLength();
		}

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		constexpr size_t skipAnyOf(Ash::Memory::View<typename ENCODING::Code> value, size_t offset, std::initializer_list<Ash::Unicode::Character> characterList)
		{
			typename ENCODING::Character character;
			size_t start = offset;

			for (; offset < value.getLength(); offset = offset + ((character.getLength() != 0) ? character.getLength() : ENCODING::minSize))
			{
				if (!ENCODING::decodeNext(value, offset, character).hasErrorSet() && !Ash::Unicode::Character::contains(characterList, character))
				{
					break;
				}
			}

			return offset - start;
		}

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		constexpr size_t skipNoneOf(Ash::Memory::View<typename ENCODING::Code> value, size_t offset, std::initializer_list<Ash::Unicode::Character> characterList)
		{
			typename ENCODING::Character character;
			size_t start = offset;

			for (; offset < value.getLength(); offset = offset + ((character.getLength() != 0) ? character.getLength() : ENCODING::minSize))
			{
				if (!ENCODING::decodeNext(value, offset, character).hasErrorSet() && Ash::Unicode::Character::contains(characterList, character))
				{
					break;
				}
			}

			return offset - start;
		}

		template
		<
			typename ENCODING,
			typename TOKEN_ALLOCATION,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>,
			typename = Ash::Type::IsClass<TOKEN_ALLOCATION, Ash::Memory::Generic::Allocation>
		>
		constexpr size_t token(Ash::Memory::View<typename ENCODING::Code> value, size_t offset, std::initializer_list<Ash::Unicode::Character> delimiters, Ash::Memory::Value<TOKEN_ALLOCATION, typename ENCODING::Code> &tokenValue)
		{
			size_t delimiterLength = skipAnyOf<ENCODING>(value, offset, delimiters);
			size_t tokenLength = skipNoneOf<ENCODING>(value, offset + delimiterLength, delimiters);

			tokenValue = Ash::Memory::Value<TOKEN_ALLOCATION, typename ENCODING::Code>(value.at(offset + delimiterLength), tokenLength);

			return delimiterLength + tokenLength;
		}
	}
}
