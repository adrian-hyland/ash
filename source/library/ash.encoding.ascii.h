#pragma once

#include "ash.string.h"


namespace Ash
{
	namespace Encoding
	{
		class Ascii
		{
		public:		
			using Code = char;

			static constexpr size_t minSize = 1;

			static constexpr size_t maxSize = 1;

			class Character : public Memory::Buffer<Code, maxSize>
			{
			public:
				static constexpr Ash::Unicode::Character::Value replacement = '?';

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

				constexpr operator Ash::Unicode::Character () const { return (getLength() > 0) ? (*this)[0] : Ash::Unicode::Character::replacement; }

			protected:
				constexpr bool set(Ash::Unicode::Character value)
				{
					if (value < 0x80)
					{
						setLength(1);
						(*this)[0] = value;
						return true;
					}
					else
					{
						setLength(0);
						return false;
					}
				}

				constexpr size_t set(Code code)
				{
					setLength(1);
					(*this)[0] = code;
					return 1;
				}

				friend Ascii;
			};

			template <typename ALLOCATION>
			static constexpr size_t decodeNext(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code = 0;

				if (value.get(offset, code) && (code >= 0))
				{
					return character.set(code);
				}
				else
				{
					character.clear();
					return 0;
				}
			}

			template <typename ALLOCATION>
			static constexpr size_t decodePrevious(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code = 0;

				if ((offset > 0) && value.get(--offset, code) && (code >= 0))
				{
					return character.set(code);
				}
				else
				{
					character.clear();
					return 0;
				}
			}

			template <typename ALLOCATION>
			using Value = Ash::String::Value<ALLOCATION, Ascii>;

			template <size_t CAPACITY>
			using Buffer = Ash::String::Buffer<Ascii, CAPACITY>;

			using View = Ash::String::View<Ascii>;

			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			using String = Ash::String::Array<Ascii, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

		private:
			Ascii();
		};
	}
}
