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
			namespace Character
			{
				static Ash::Test::Assertion isValid()
				{
					TEST_IS_TRUE(Ash::Unicode::Character::isValid(0));

					TEST_IS_TRUE(Ash::Unicode::Character::isValid(Ash::Unicode::Character::surrogateStart - 1));

					TEST_IS_FALSE(Ash::Unicode::Character::isValid(Ash::Unicode::Character::surrogateStart));

					TEST_IS_FALSE(Ash::Unicode::Character::isValid(Ash::Unicode::Character::surrogateEnd));

					TEST_IS_TRUE(Ash::Unicode::Character::isValid(Ash::Unicode::Character::surrogateEnd + 1));

					TEST_IS_TRUE(Ash::Unicode::Character::isValid(Ash::Unicode::Character::maximum));

					TEST_IS_FALSE(Ash::Unicode::Character::isValid(Ash::Unicode::Character::maximum + 1));

					TEST_IS_FALSE(Ash::Unicode::Character::isValid(std::numeric_limits<Ash::Unicode::Character::Value>::max()));

					return {};
				}

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

					try
					{
						Ash::Unicode::Character character(Ash::Unicode::Character::surrogateStart, false);

						TEST_FAIL("Ash::Unicode::Error::invalidValue exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Unicode::Error::invalidValue);
					}

					try
					{
						Ash::Unicode::Character character(Ash::Unicode::Character::surrogateEnd, false);

						TEST_FAIL("Ash::Unicode::Error::invalidValue exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Unicode::Error::invalidValue);
					}

					try
					{
						Ash::Unicode::Character character(Ash::Unicode::Character::maximum + 1, false);

						TEST_FAIL("Ash::Unicode::Error::invalidValue exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Unicode::Error::invalidValue);
					}

					try
					{
						Ash::Unicode::Character character(std::numeric_limits<Ash::Unicode::Character::Value>::max(), false);

						TEST_FAIL("Ash::Unicode::Error::invalidValue exception");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Unicode::Error::invalidValue);
					}

					return {};
				}

				static Ash::Test::Assertion set()
				{
					Ash::Unicode::Character character;

					TEST_IS_EQ(character.set(0), Ash::Error::none);

					TEST_IS_EQ(character.set(Ash::Unicode::Character::surrogateStart - 1), Ash::Error::none);

					TEST_IS_EQ(character.set(Ash::Unicode::Character::surrogateStart), Ash::Error::none);
					TEST_IS_EQ(character, Ash::Unicode::Character::replacement);
					TEST_IS_EQ(character.set(Ash::Unicode::Character::surrogateStart, false), Ash::Unicode::Error::invalidValue);

					TEST_IS_EQ(character.set(Ash::Unicode::Character::surrogateEnd), Ash::Error::none);
					TEST_IS_EQ(character, Ash::Unicode::Character::replacement);
					TEST_IS_EQ(character.set(Ash::Unicode::Character::surrogateEnd, false), Ash::Unicode::Error::invalidValue);

					TEST_IS_EQ(character.set(Ash::Unicode::Character::surrogateEnd + 1), Ash::Error::none);

					TEST_IS_EQ(character.set(Ash::Unicode::Character::maximum), Ash::Error::none);

					TEST_IS_EQ(character.set(Ash::Unicode::Character::maximum + 1), Ash::Error::none);
					TEST_IS_EQ(character, Ash::Unicode::Character::replacement);
					TEST_IS_EQ(character.set(Ash::Unicode::Character::maximum + 1, false), Ash::Unicode::Error::invalidValue);

					TEST_IS_EQ(character.set(std::numeric_limits<Ash::Unicode::Character::Value>::max()), Ash::Error::none);
					TEST_IS_EQ(character, Ash::Unicode::Character::replacement);
					TEST_IS_EQ(character.set(std::numeric_limits<Ash::Unicode::Character::Value>::max(), false), Ash::Unicode::Error::invalidValue);

					return {};
				}

				static Ash::Test::Assertion contains()
				{
					TEST_IS_FALSE(Ash::Unicode::Character::contains({}, ' '));

					TEST_IS_FALSE(Ash::Unicode::Character::contains({ '1', '2', '3' }, '0'));

					TEST_IS_TRUE(Ash::Unicode::Character::contains({ '1', '2', '3' }, '1'));

					TEST_IS_TRUE(Ash::Unicode::Character::contains({ '1', '2', '3' }, '3'));

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testUnicode,

			TEST_CASE(Ash::Test::Unicode::Character::isValid),
			TEST_CASE(Ash::Test::Unicode::Character::character),
			TEST_CASE(Ash::Test::Unicode::Character::set),
			TEST_CASE(Ash::Test::Unicode::Character::contains)
		);
	}
}
