#pragma once

#include "ash.type.h"
#include "ash.memory.h"
#include "ash.unicode.character.h"


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
		constexpr size_t convert(Ash::Memory::View<typename FROM_ENCODING::Code> from, Ash::Memory::Value<TO_ALLOCATION, typename TO_ENCODING::Code> &to, size_t &toOffset, Ash::Unicode::Character replacementCharacter = TO_ENCODING::Character::replacement)
		{
			size_t fromOffset = 0;

			while (fromOffset < from.getLength())
			{
				typename FROM_ENCODING::Character fromCharacter;
				size_t decodeLength = FROM_ENCODING::decodeNext(from, fromOffset, fromCharacter);

				typename TO_ENCODING::Character toCharacter(fromCharacter);
				if (toCharacter.getLength() == 0)
				{
					toCharacter = replacementCharacter;
					if (toCharacter.getLength() == 0)
					{
						break;
					}
				}

				if (!to.set(toOffset, toCharacter))
				{
					break;
				}

				fromOffset = fromOffset + ((decodeLength != 0) ? decodeLength : FROM_ENCODING::minSize);
				toOffset = toOffset + toCharacter.getLength();
			}

			return fromOffset;
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
		constexpr size_t convert(Ash::Memory::View<typename FROM_ENCODING::Code> from, Ash::Memory::Value<TO_ALLOCATION, typename TO_ENCODING::Code> &to, Ash::Unicode::Character replacementCharacter = TO_ENCODING::Character::replacement)
		{
			size_t toOffset = 0;

			to.clear();

			return convert<FROM_ENCODING, TO_ENCODING, TO_ALLOCATION>(from, to, toOffset, replacementCharacter);
		}

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		constexpr size_t find(Ash::Memory::View<typename ENCODING::Code> value, size_t offset, Ash::Unicode::Character character)
		{
			typename ENCODING::Character characterEncoding(character);
			typename ENCODING::Code characterCode;
			
			if (characterEncoding.getLength() <= value.getLength())
			{
				if (characterEncoding.getLength() == 1)
				{
					if (characterEncoding.get(0, characterCode))
					{
						return value.find(offset, characterCode);
					}
				}
				else if (characterEncoding.getLength() > 1)
				{
					if constexpr (ENCODING::isBigEndian)
					{
						if (characterEncoding.get(ENCODING::minSize - 1, characterCode))
						{
							while (offset <= value.getLength() - characterEncoding.getLength())
							{
								offset = value.template find<ENCODING::minSize, ENCODING::minSize - 1>(offset, characterCode);
								if (value.match(offset - (ENCODING::minSize - 1), characterEncoding) == characterEncoding.getLength())
								{
									return offset - (ENCODING::minSize - 1);
								}
								offset = offset + ENCODING::minSize;
							}
						}
					}
					else
					{
						if (characterEncoding.get(0, characterCode))
						{
							while (offset <= value.getLength() - characterEncoding.getLength())
							{
								offset = value.template find<ENCODING::minSize>(offset, characterCode);
								if (value.match(offset, characterEncoding) == characterEncoding.getLength())
								{
									return offset;
								}
								offset = offset + ENCODING::minSize;
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
			size_t decodeLength = 0;

			for (; offset < value.getLength(); offset = offset + decodeLength)
			{
				decodeLength = ENCODING::decodeNext(value, offset, character);
				if (decodeLength != 0)
				{
					if (!Ash::Unicode::Character::contains(characterList, character))
					{
						break;
					}
				}
				else
				{
					decodeLength = ENCODING::minSize;
				}
			}

			return offset;
		}

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		constexpr size_t skipNoneOf(Ash::Memory::View<typename ENCODING::Code> value, size_t offset, std::initializer_list<Ash::Unicode::Character> characterList)
		{
			typename ENCODING::Character character;
			size_t decodeLength = 0;

			for (; offset < value.getLength(); offset = offset + decodeLength)
			{
				decodeLength = ENCODING::decodeNext(value, offset, character);
				if (decodeLength != 0)
				{
					if (Ash::Unicode::Character::contains(characterList, character))
					{
						break;
					}
				}
				else
				{
					decodeLength = ENCODING::minSize;
				}
			}

			return offset;
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
			size_t start = skipAnyOf<ENCODING>(value, offset, delimiters);
			size_t end = skipNoneOf<ENCODING>(value, start, delimiters);

			tokenValue = Ash::Memory::Value<TOKEN_ALLOCATION, typename ENCODING::Code>(value.at(start), end - start);

			return end;
		}
	}
}
