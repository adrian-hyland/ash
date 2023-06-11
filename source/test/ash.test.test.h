#include "ash.test.h"


namespace Ash
{
	namespace Test
	{
		namespace Test
		{
			constexpr Ash::Test::Assertion isTrue()
			{
				TEST_IS_TRUE(true);

				return {};
			}

			constexpr Ash::Test::Assertion isFalse()
			{
				TEST_IS_FALSE(false);

				return {};
			}

			constexpr Ash::Test::Assertion isNull()
			{
				TEST_IS_NULL(nullptr);

				return {};
			}

			constexpr Ash::Test::Assertion isNotNull()
			{
				TEST_IS_NOT_NULL(isNotNull);

				return {};
			}

			constexpr Ash::Test::Assertion isZero()
			{
				TEST_IS_ZERO(0);

				return {};
			}

			constexpr Ash::Test::Assertion isNotZero()
			{
				TEST_IS_NOT_ZERO(~0);

				return {};
			}

			constexpr Ash::Test::Assertion isEqual()
			{
				TEST_IS_EQ(0, 0);

				return {};
			}

			constexpr Ash::Test::Assertion isNotEqual()
			{
				TEST_IS_NOT_EQ(0, 1);

				return {};
			}
		}

		TEST_UNIT
		(
			testTest,
			TEST_CASE(Ash::Test::Test::isTrue),
			TEST_CASE(Ash::Test::Test::isFalse),
			TEST_CASE(Ash::Test::Test::isNull),
			TEST_CASE(Ash::Test::Test::isNotNull),
			TEST_CASE(Ash::Test::Test::isZero),
			TEST_CASE(Ash::Test::Test::isNotZero),
			TEST_CASE(Ash::Test::Test::isEqual),
			TEST_CASE(Ash::Test::Test::isNotEqual),
		);
	}
}
