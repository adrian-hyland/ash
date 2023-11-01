#include <limits>
#include "ash.test.h"
#include "ash.unicode.character.h"
#include "ash.test.unicode.h"


namespace Ash
{
	namespace Test
	{
		namespace Unicode
		{
			static Ash::Test::Assertion character()
			{
				TEST_IS_EQ(Ash::Unicode::Character(0), 0);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::surrogateStart - 1), Ash::Unicode::Character::surrogateStart - 1);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::surrogateStart), Ash::Unicode::Character::replacement);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::surrogateEnd), Ash::Unicode::Character::replacement);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::surrogateEnd + 1), Ash::Unicode::Character::surrogateEnd + 1);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::maximum), Ash::Unicode::Character::maximum);

				TEST_IS_EQ(Ash::Unicode::Character(Ash::Unicode::Character::maximum + 1), Ash::Unicode::Character::replacement);

				TEST_IS_EQ(Ash::Unicode::Character(std::numeric_limits<Ash::Unicode::Character::Value>::max()), Ash::Unicode::Character::replacement);

				return {};
			}

			static Ash::Test::Assertion characterContains()
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
			TEST_CASE(Ash::Test::Unicode::character),
			TEST_CASE(Ash::Test::Unicode::characterContains)
		);
	}
}
