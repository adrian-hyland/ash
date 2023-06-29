#pragma once

#include "ash.string.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf32be
		{
		public:
			using Code = uint8_t;

			static constexpr size_t minSze = 4;

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
					if (getLength() == 4)
					{
						return ((*this)[0] << 24) | ((*this)[1] << 16) | ((*this)[2] << 8) | (*this)[3];
					}
					else
					{
						return Ash::Unicode::Character::replacement;
					}
				}

			protected:
				constexpr bool set(Ash::Unicode::Character value)
				{
					if ((value >= 0xD800) && (value <= 0xDFFF))
					{
						clear();
						return false;
					}

					setLength(4);
					(*this)[0] = value >> 24;
					(*this)[1] = value >> 16;
					(*this)[2] = value >> 8;
					(*this)[3] = value & 0xFF;
					return true;
				}

				constexpr size_t set(Code code1, Code code2, Code code3, Code code4)
				{
					if ((code1 == 0x00) && (code2 < 0x11))
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

				friend Utf32be;
			};

			template <typename ALLOCATION>
			static constexpr size_t decodeNext(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
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

			template <typename ALLOCATION>
			static constexpr size_t decodePrevious(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
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

			template <typename ALLOCATION>
			using Value = String::Value<ALLOCATION, Utf32be>;

			template <size_t N>
			using Buffer = String::Buffer<Utf32be, N>;

			using View = String::View<Utf32be>;

			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			using String = String::Array<Utf32be, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

		private:
			Utf32be();
		};
	}
}
