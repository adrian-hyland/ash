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

			class Character : public Ash::Memory::Buffer<Code, maxSize>
			{
			public:
				static constexpr Code replacementCode = '?';

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

				constexpr operator Ash::Unicode::Character () const { return (getLength() > 0) ? (*this)[0] : '\0'; }

			protected:
				constexpr void set(Ash::Unicode::Character character)
				{
					setLength(1).assertErrorNotSet();
					(*this)[0] = (Ash::Unicode::Character::Value(character) < 0x80) ? Ash::Unicode::Character::Value(character) : Ash::Unicode::Character::Value(replacementCode);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code)
				{
					setLength(1).assertErrorNotSet();
					(*this)[0] = code;
					return Ash::Error::none;
				}

				friend Ascii;
			};

			[[nodiscard]]
			static constexpr Ash::Error::Value decodeNext(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code = 0;
				Ash::Error::Value error = getNextCode(value, offset, code);
				if (!error.hasErrorSet())
				{
					if (code >= 0)
					{
						return character.set(code);
					}
					else
					{
						error = Ash::Encoding::Error::invalidCodeUnit;
					}
				}

				character.clear();
				return error;
			}

			[[nodiscard]]
			static constexpr Ash::Error::Value decodePrevious(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code = 0;
				Ash::Error::Value error = getPreviousCode(value, offset, code);
				if (!error.hasErrorSet())
				{
					if (code >= 0)
					{
						return character.set(code);
					}
					else
					{
						error = Ash::Encoding::Error::invalidCodeUnit;
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
			Ascii();
		};
	}
}
