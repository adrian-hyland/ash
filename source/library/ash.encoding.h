#pragma once

#include "ash.memory.h"
#include "ash.unicode.character.h"


namespace Ash
{
	namespace Encoding
	{
		template <typename FROM_ENCODING, typename TO_ENCODING, typename FROM_ALLOCATION, typename TO_ALLOCATION>
		constexpr size_t convert(const Ash::Memory::Value<FROM_ALLOCATION, typename FROM_ENCODING::Code> &from, Ash::Memory::Value<TO_ALLOCATION> &to, Ash::Unicode::Character replacementCharacter = TO_ENCODING::Character::replacement)
		{
			size_t offset = 0;

			to.clear();

			while (offset < from.getLength())
			{
				typename FROM_ENCODING::Character fromCharacter;
				size_t decodeLength = FROM_ENCODING::decodeNext(from, offset, fromCharacter);

				typename TO_ENCODING::Character toCharacter(fromCharacter);
				if (toCharacter.getLength() == 0)
				{
					toCharacter = replacementCharacter;
					if (toCharacter.getLength() == 0)
					{
						break;
					}
				}

				if (!to.append(toCharacter))
				{
					break;
				}

				offset = offset + ((decodeLength != 0) ? decodeLength : FROM_ENCODING::minSize);
			}

			return offset;
		}

		template <typename ENCODING, typename ALLOCATION>
		constexpr size_t find(const Ash::Memory::Value<ALLOCATION, typename ENCODING::Code> &value, size_t offset, Ash::Unicode::Character character)
		{
			typename ENCODING::Character valueCharacter;
			size_t decodeLength = 0;

			for (; offset < value.getLength(); offset = offset + decodeLength)
			{
				decodeLength = ENCODING::decodeNext(value, offset, valueCharacter);
				if (decodeLength != 0)
				{
					if (Ash::Unicode::Character(valueCharacter) == character)
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

		template <typename ENCODING, typename ALLOCATION>
		constexpr bool contains(const Ash::Memory::Value<ALLOCATION, typename ENCODING::Code> &value, Ash::Unicode::Character character)
		{
			return find<ENCODING>(value, 0, character) < value.getLength();
		}

		template <typename ENCODING, typename ALLOCATION>
		constexpr size_t skipAnyOf(const Ash::Memory::Value<ALLOCATION, typename ENCODING::Code> &value, size_t offset, std::initializer_list<Ash::Unicode::Character> characterList)
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

		template <typename ENCODING, typename ALLOCATION>
		constexpr size_t skipNoneOf(const Ash::Memory::Value<ALLOCATION, typename ENCODING::Code> &value, size_t offset, std::initializer_list<Ash::Unicode::Character> characterList)
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

		template <typename ENCODING, typename VALUE_ALLOCATION, typename TOKEN_ALLOCATION>
		constexpr size_t token(const Ash::Memory::Value<VALUE_ALLOCATION, typename ENCODING::Code> &value, size_t offset, std::initializer_list<Ash::Unicode::Character> delimiters, Ash::Memory::Value<TOKEN_ALLOCATION, typename ENCODING::Code> &tokenValue)
		{
			size_t start = skipAnyOf<ENCODING>(value, offset, delimiters);
			size_t end = skipNoneOf<ENCODING>(value, start, delimiters);

			tokenValue = Ash::Memory::Value<TOKEN_ALLOCATION, typename ENCODING::Code>(value.at(start), end - start);

			return end;
		}
	}
}
