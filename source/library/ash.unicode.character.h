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

			static constexpr Value maximum = 0x10FFFF;

			static constexpr Value replacement = 0xFFFD;

			constexpr Character(Value value) : m_Value(value <= maximum ? value : replacement) {}

			constexpr operator Value () const { return m_Value; }

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
