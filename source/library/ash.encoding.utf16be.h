#pragma once

#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf16be : Ash::Generic::Encoding
		{
		public:
			using Code = Ash::Encoding::CodeUnit8;

			static constexpr size_t minSize = 2;

			static constexpr size_t maxSize = 4;

			static constexpr bool isLittleEndian = false;

			static constexpr bool isBigEndian = true;

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
					if (getLength() == 2)
					{
						return (getCode(0) << 8) + getCode(1);
					}
					else if (getLength() == 4)
					{
						return 0x10000 + ((getCode(0) & 0x03) << 18) + (getCode(1) << 10) + ((getCode(2) & 0x03) << 8) + getCode(3);
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

					if (value < 0x10000)
					{
						setLength(2);
						(*this)[0] = value >> 8;
						(*this)[1] = value & 0xFF;
					}
					else
					{
						value = value - 0x10000;
						setLength(4);
						(*this)[0] = ((value >> 18) & 0x03) | 0xD8;
						(*this)[1] = value >> 10;
						(*this)[2] = ((value >> 8) & 0x03) | 0xDC;
						(*this)[3] = value;
					}
				}

				constexpr size_t set(Code code1, Code code2)
				{
					setLength(2);
					(*this)[0] = code1;
					(*this)[1] = code2;
					return 2;
				}

				constexpr size_t set(Code code1, Code code2, Code code3, Code code4)
				{
					setLength(4);
					(*this)[0] = code1;
					(*this)[1] = code2;
					(*this)[2] = code3;
					(*this)[3] = code4;
					return 4;
				}

				friend Utf16be;
			};

			static constexpr size_t decodeNext(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				if (value.get(offset++, code1) && value.get(offset++, code2))
				{
					if ((code1 & 0xFC) == 0xD8)
					{
						if (value.get(offset++, code3) && value.get(offset++, code4))
						{
							if ((code3 & 0xFC) == 0xDC)
							{
								return character.set(code1, code2, code3, code4);
							}
						}
					}
					else if ((code1 & 0xFC) != 0xDC)
					{
						return character.set(code1, code2);
					}
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

				if ((offset > 1) && value.get(--offset, code1) && value.get(--offset, code2))
				{
					if ((code2 & 0xFC) == 0xDC)
					{
						if ((offset > 1) && value.get(--offset, code3) && value.get(--offset, code4))
						{
							if ((code4 & 0xFC) == 0xD8)
							{
								return character.set(code4, code3, code2, code1);
							}
						}
					}
					else if ((code2 & 0xFC) != 0xD8)
					{
						return character.set(code2, code1);
					}
				}

				character.clear();
				return 0;
			}

		private:
			Utf16be();
		};
	}
}
