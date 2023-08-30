#include "ash.test.h"
#include "ash.test.test.h"


namespace Ash
{
	namespace Test
	{
		namespace Test
		{
			static Ash::Test::Assertion isTrue()
			{
				TEST_IS_TRUE(true);

				return {};
			}

			static Ash::Test::Assertion isFalse()
			{
				TEST_IS_FALSE(false);

				return {};
			}

			static Ash::Test::Assertion isNull()
			{
				TEST_IS_NULL(nullptr);

				return {};
			}

			static Ash::Test::Assertion isNotNull()
			{
				TEST_IS_NOT_NULL((volatile void *)isNotNull);

				return {};
			}

			static Ash::Test::Assertion isZero()
			{
				TEST_IS_ZERO(0);

				return {};
			}

			static Ash::Test::Assertion isNotZero()
			{
				TEST_IS_NOT_ZERO(~0);

				return {};
			}

			static Ash::Test::Assertion isEqual()
			{
				TEST_IS_EQ(0, 0);

				return {};
			}

			static Ash::Test::Assertion isNotEqual()
			{
				TEST_IS_NOT_EQ(0, 1);

				return {};
			}

			static Ash::Test::Assertion isLessThan()
			{
				TEST_IS_LT(0, 1);

				return {};
			}

			static Ash::Test::Assertion isLessThanOrEqual()
			{
				TEST_IS_LTE(0, 1);
				TEST_IS_LTE(1, 1);

				return {};
			}

			static Ash::Test::Assertion isGreaterThan()
			{
				TEST_IS_GT(1, 0);

				return {};
			}

			static Ash::Test::Assertion isGreaterThanOrEqual()
			{
				TEST_IS_GTE(1, 0);
				TEST_IS_GTE(1, 1);

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
			TEST_CASE(Ash::Test::Test::isLessThan),
			TEST_CASE(Ash::Test::Test::isLessThanOrEqual),
			TEST_CASE(Ash::Test::Test::isGreaterThan),
			TEST_CASE(Ash::Test::Test::isGreaterThanOrEqual),
		);
	}
}
