#pragma once

#include <cstdint>
#include <cstddef>
#include <initializer_list>
#include "ash.unicode.error.h"


namespace Ash
{
	namespace Unicode
	{
		class Character
		{
		public:
			using Value = uint32_t;

			static constexpr Value maximum = 0x10FFFF;

			static constexpr Value replacement = 0xFFFD;

			static constexpr Value surrogateStart = 0xD800;

			static constexpr Value surrogateEnd = 0xDFFF;

			constexpr Character(Value value = 0, bool replaceInvalidValue = true) : m_Value()
			{
				set(value, replaceInvalidValue).throwOnError();
			}

			constexpr operator Value () const { return m_Value; }

			[[nodiscard]]
			constexpr Ash::Error::Value set(Value value, bool replaceInvalidValue = true)
			{
				if (isValid(value))
				{
					m_Value = value;
					return Ash::Error::none;
				}

				if (replaceInvalidValue)
				{
					m_Value = replacement;
					return Ash::Error::none;
				}

				return Ash::Unicode::Error::invalidValue;
			}

			static constexpr bool isValid(Value value) { return (value < surrogateStart) || ((value > surrogateEnd) && (value <= maximum)); }

			static constexpr bool contains(std::initializer_list<Character> characterList, Character character)
			{
				for (Character c : characterList)
				{
					if (c == character)
					{
						return true;
					}
				}

				return false;
			}

		private:
			Value m_Value;
		};
	}
}
