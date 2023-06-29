#pragma once

#include "ash.string.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf16le
		{
		public:
			using Code = uint8_t;

			static constexpr size_t minSize = 2;

			static constexpr size_t maxSize = 4;

			class Character : public Memory::Buffer<Code, maxSize>
			{
			public:
				static constexpr Ash::Unicode::Character::Value replacement = Ash::Unicode::Character::replacement;

				constexpr Character() : Memory::Buffer<Code, maxSize>() {}

				constexpr Character(Ash::Unicode::Character value) : Memory::Buffer<Code, maxSize>()
				{
					set(value);
				}

				constexpr Ash::Unicode::Character operator = (Ash::Unicode::Character value)
				{
					set(value);
					return value;
				}

				constexpr operator Ash::Unicode::Character () const
				{
					if (getLength() == 2)
					{
						return ((*this)[1] << 8) + (*this)[0];
					}
					else if (getLength() == 4)
					{
						return 0x10000 + (((*this)[1] & 0x03) << 18) + ((*this)[0] << 10) + (((*this)[3] & 0x03) << 8) + (*this)[2];
					}
					else
					{
						return Ash::Unicode::Character::replacement;
					}
				}

			protected:
				constexpr bool set(Ash::Unicode::Character value)
				{
					if (value < 0x10000)
					{
						if ((value >= 0xD800) && (value <= 0xDFFF))
						{
							clear();
							return false;
						}

						setLength(2);
						(*this)[0] = value & 0xFF;
						(*this)[1] = value >> 8;
					}
					else
					{
						value = value - 0x10000;
						setLength(4);
						(*this)[0] = value >> 10;
						(*this)[1] = ((value >> 18) & 0x03) | 0xD8;
						(*this)[2] = value;
						(*this)[3] = ((value >> 8) & 0x03) | 0xDC;
					}

					return true;
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

				friend Utf16le;
			};

			template <typename ALLOCATION>
			static constexpr size_t decodeNext(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				if (value.get(offset++, code1) && value.get(offset++, code2))
				{
					if ((code2 & 0xFC) == 0xD8)
					{
						if (value.get(offset++, code3) && value.get(offset++, code4))
						{
							if ((code4 & 0xFC) == 0xDC)
							{
								return character.set(code1, code2, code3, code4);
							}
						}
					}
					else if ((code2 & 0xFC) != 0xDC)
					{
						return character.set(code1, code2);
					}
				}

				character.clear();
				return 0;
			}

			template <typename ALLOCATION>
			static constexpr size_t decodePrevious(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				if ((offset > 1) && value.get(--offset, code1) && value.get(--offset, code2))
				{
					if ((code1 & 0xFC) == 0xDC)
					{
						if ((offset > 1) && value.get(--offset, code3) && value.get(--offset, code4))
						{
							if ((code3 & 0xFC) == 0xD8)
							{
								return character.set(code4, code3, code2, code1);
							}
						}
					}
					else if ((code1 & 0xFC) != 0xD8)
					{
						return character.set(code2, code1);
					}
				}

				character.clear();
				return 0;
			}

			template <typename ALLOCATION>
			using Value = String::Value<ALLOCATION, Utf16le>;

			template <size_t CAPACITY>
			using Buffer = String::Buffer<Utf16le, CAPACITY>;

			using View = String::View<Utf16le>;

			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			using String = String::Array<Utf16le, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

		private:
			Utf16le();
		};
	}
}
