#pragma once

#include <cstdint>
#include <cstddef>


namespace Ash
{
	namespace Unicode
	{
		class Character
		{
		public:
			using Value = uint32_t;

			constexpr Character(Value value) : m_Value(value) {}

			constexpr operator Value () const { return m_Value; }

			static constexpr Value Maximum = 0x10FFFF;

			static constexpr Value Replacement = 0xFFFD;

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
