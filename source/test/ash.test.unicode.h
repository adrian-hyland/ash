#pragma once

#include <limits>
#include "ash.test.h"
#include "ash.unicode.character.h"


namespace Ash
{
	namespace Test
	{
		namespace Unicode
		{
			constexpr Ash::Test::Assertion testCharacter()
			{
				TEST_IS_EQ(Ash::Unicode::Character(0), 0);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::maximum), Ash::Unicode::Character::maximum);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::maximum + 1), Ash::Unicode::Character::replacement);

				TEST_IS_EQ(Ash::Unicode::Character(std::numeric_limits<Ash::Unicode::Character::Value>::max()), Ash::Unicode::Character::replacement);

				return {};
			}

			constexpr Ash::Test::Assertion testCharacterContains()
			{
				TEST_IS_FALSE(Ash::Unicode::Character::contains({}, ' '));

				TEST_IS_FALSE(Ash::Unicode::Character::contains({ '1', '2', '3' }, '0'));

				TEST_IS_TRUE(Ash::Unicode::Character::contains({ '1', '2', '3' }, '1'));

				TEST_IS_TRUE(Ash::Unicode::Character::contains({ '1', '2', '3' }, '3'));

				return {};
			}
		}

		TEST_UNIT
		(
			testUnicode,
			TEST_CASE(Ash::Test::Unicode::testCharacter),
			TEST_CASE(Ash::Test::Unicode::testCharacterContains)
		);
	}
}
