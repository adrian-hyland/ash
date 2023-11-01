#pragma once

#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		class Ascii : Ash::Generic::Encoding
		{
		public:		
			using Code = char;

			static constexpr size_t minSize = 1;

			static constexpr size_t maxSize = 1;

			static constexpr bool isLittleEndian = false;

			static constexpr bool isBigEndian = false;

			class Character : public Memory::Buffer<Code, maxSize>
			{
			public:
				static constexpr Ash::Unicode::Character replacement = '?';

				constexpr Character() : Memory::Buffer<Code, maxSize>() {}

				constexpr Character(Ash::Unicode::Character character) : Memory::Buffer<Code, maxSize>()
				{
					set(character);
				}

				constexpr Ash::Unicode::Character operator = (Ash::Unicode::Character character)
				{
					set(character);
					return character;
				}

				constexpr operator Ash::Unicode::Character () const { return (getLength() > 0) ? (*this)[0] : Ash::Unicode::Character::replacement; }

			protected:
				constexpr void set(Ash::Unicode::Character character)
				{
					Ash::Unicode::Character::Value value = character;
					if (value < 0x80)
					{
						setLength(1);
						(*this)[0] = value;
					}
					else
					{
						setLength(0);
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

			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
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

			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
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

		private:
			Ascii();
		};
	}
}
