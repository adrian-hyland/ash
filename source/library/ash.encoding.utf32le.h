#pragma once

#include "ash.string.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf32le
		{
		public:
			using Code = uint8_t;

			static constexpr size_t minSize = 4;

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
						return ((*this)[3] << 24) | ((*this)[2] << 16) | ((*this)[1] << 8) | (*this)[0];
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
					(*this)[0] = value & 0xFF;
					(*this)[1] = value >> 8;
					(*this)[2] = value >> 16;
					(*this)[3] = value >> 24;
					return true;
				}

				constexpr size_t set(Code code1, Code code2, Code code3, Code code4)
				{
					if ((code4 == 0x00) && (code3 < 0x11))
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
			using Value = String::Value<ALLOCATION, Utf32le>;

			template <size_t CAPACITY>
			using Buffer = String::Buffer<Utf32le, CAPACITY>;

			using View = String::View<Utf32le>;

			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			using String = String::Array<Utf32le, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

		private:
			Utf32le();
		};
	}
}
