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
