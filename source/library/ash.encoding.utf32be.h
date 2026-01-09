#pragma once

#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf32be : Ash::Generic::Encoding
		{
		public:
			using Code = Ash::Encoding::CodeUnit8;

			static constexpr size_t minSize = 4;

			static constexpr size_t maxSize = 4;

			static constexpr bool isLittleEndian = false;

			static constexpr bool isBigEndian = true;

			class Character : public Ash::Memory::Buffer<Code, maxSize>
			{
			public:
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
					if (getLength() == 0)
					{
						return '\0';
					}
					else
					{
						return (getCode(0) << 24) | (getCode(1) << 16) | (getCode(2) << 8) | getCode(3);
					}
				}

			protected:
				constexpr Ash::Unicode::Character::Value getCode(size_t offset) const { return Ash::Unicode::Character::Value(uint8_t((*this)[offset])); }

				constexpr void set(Ash::Unicode::Character character)
				{
					Ash::Unicode::Character::Value value = character;

					setLength(maxSize).assertErrorNotSet();
					(*this)[0] = value >> 24;
					(*this)[1] = value >> 16;
					(*this)[2] = value >> 8;
					(*this)[3] = value & 0xFF;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1, Code code2, Code code3, Code code4)
				{
					if ((uint8_t(code1) != 0x00) || (uint8_t(code2) > 0x10) || ((uint8_t(code2) == 0) && (uint8_t(code3) >= 0xD8) && (uint8_t(code3) < 0xE0)))
					{
						clear();
						return Ash::Encoding::Error::invalidCodePoint;
					}

					setLength(maxSize).assertErrorNotSet();
					(*this)[0] = code1;
					(*this)[1] = code2;
					(*this)[2] = code3;
					(*this)[3] = code4;
					return Ash::Error::none;
				}

				friend Utf32be;
			};

			[[nodiscard]]
			static constexpr Ash::Error::Value decodeNext(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				Ash::Error::Value error = getNextCode(value, offset, code1);
				if (!error)
				{
					error = getNextCode(value, offset, code2);
					if (!error)
					{
						error = getNextCode(value, offset, code3);
						if (!error)
						{
							error = getNextCode(value, offset, code4);
							if (!error)
							{
								return character.set(code1, code2, code3, code4);
							}
						}
					}
				}

				character.clear();
				return error;
			}

			[[nodiscard]]
			static constexpr Ash::Error::Value decodePrevious(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				Ash::Error::Value error = getPreviousCode(value, offset, code1);
				if (!error)
				{
					error = getPreviousCode(value, offset, code2);
					if (!error)
					{
						error = getPreviousCode(value, offset, code3);
						if (!error)
						{
							error = getPreviousCode(value, offset, code4);
							if (!error)
							{
								return character.set(code4, code3, code2, code1);
							}
						}
					}
				}

				character.clear();
				return error;
			}

		protected:
			[[nodiscard]]
			static constexpr Ash::Error::Value getNextCode(Ash::Memory::View<Code> value, size_t &offset, Code &code)
			{
				return value.get(offset++, code);
			}

			[[nodiscard]]
			static constexpr Ash::Error::Value getPreviousCode(Ash::Memory::View<Code> value, size_t &offset, Code &code)
			{
				return (offset != 0) ? value.get(--offset, code) : Ash::Memory::Error::readAccessOutOfBound;
			}

		private:
			Utf32be();
		};
	}
}
