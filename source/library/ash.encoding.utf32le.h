#pragma once

#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf32le : Ash::Generic::Encoding
		{
		public:
			using Code = Ash::Encoding::CodeUnit8;

			static constexpr size_t minSize = 4;

			static constexpr size_t maxSize = 4;

			static constexpr bool isLittleEndian = true;

			static constexpr bool isBigEndian = false;

			class Character : public Ash::Memory::Buffer<Code, maxSize>
			{
			public:
				static constexpr Ash::Unicode::Character replacement = Ash::Unicode::Character::replacement;

				constexpr Character() : Ash::Memory::Buffer<Code, maxSize>() {}

				constexpr Character(Ash::Unicode::Character character) : Ash::Memory::Buffer<Code, maxSize>()
				{
					set(character);
				}

				constexpr Ash::Unicode::Character operator = (Ash::Unicode::Character character)
				{
					set(character);
					return character;
				}

				constexpr operator Ash::Unicode::Character () const
				{
					if (getLength() == 4)
					{
						return (getCode(3) << 24) | (getCode(2) << 16) | (getCode(1) << 8) | getCode(0);
					}
					else
					{
						return Ash::Unicode::Character::replacement;
					}
				}

			protected:
				constexpr Ash::Unicode::Character::Value getCode(size_t offset) const { return Ash::Unicode::Character::Value(uint8_t((*this)[offset])); }

				constexpr void set(Ash::Unicode::Character character)
				{
					Ash::Unicode::Character::Value value = character;

					setLength(4);
					(*this)[0] = value & 0xFF;
					(*this)[1] = value >> 8;
					(*this)[2] = value >> 16;
					(*this)[3] = value >> 24;
				}

				constexpr size_t set(Code code1, Code code2, Code code3, Code code4)
				{
					if ((uint8_t(code4) == 0x00) && (uint8_t(code3) < 0x11) && ((uint8_t(code3) != 0) || (uint8_t(code2) < 0xD8) || (uint8_t(code2) >= 0xE0)))
					{
						setLength(4);
						(*this)[0] = code1;
						(*this)[1] = code2;
						(*this)[2] = code3;
						(*this)[3] = code4;
						return 4;
					}

					clear();
					return 0;
				}

				friend Utf32le;
			};

			static constexpr size_t decodeNext(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				if (value.get(offset++, code1) && value.get(offset++, code2) && value.get(offset++, code3) && value.get(offset++, code4))
				{
					return character.set(code1, code2, code3, code4);
				}

				character.clear();
				return 0;
			}

			static constexpr size_t decodePrevious(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				if ((offset > 3) && value.get(--offset, code1) && value.get(--offset, code2) && value.get(--offset, code3) && value.get(--offset, code4))
				{
					return character.set(code4, code3, code2, code1);
				}

				character.clear();
				return 0;
			}

		private:
			Utf32le();
		};
	}
}
