#include "ash.test.h"
#include "ash.integer.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			template <typename INTEGER_TYPE, INTEGER_TYPE START, INTEGER_TYPE END>
			constexpr Ash::Test::Assertion cycleIdentity()
			{
				using TestCycle = Ash::Integer::Cycle<INTEGER_TYPE, START, END>;

				if ((TestCycle::minValue <= 0) && (TestCycle::maxValue >= 0))
				{
					TEST_IS_EQ(TestCycle::identity, 0);
				}

				for (TestCycle n : TestCycle::getRange())
				{
					TEST_IS_EQ(TestCycle(n + TestCycle::identity), n);
				}

				return {};
			}

			template <typename INTEGER_TYPE, INTEGER_TYPE START, INTEGER_TYPE END>
			constexpr Ash::Test::Assertion cycleInverse()
			{
				using TestCycle = Ash::Integer::Cycle<INTEGER_TYPE, START, END>;

				for (TestCycle n : TestCycle::getRange())
				{
					TEST_IS_EQ(TestCycle(n + n.inverse()), TestCycle::identity);
				}

				return {};
			}

			template <typename INTEGER_TYPE, INTEGER_TYPE START, INTEGER_TYPE END>
			constexpr Ash::Test::Assertion cycleRange()
			{
				using TestCycle = Ash::Integer::Cycle<INTEGER_TYPE, START, END>;

				int count = 0;
				for (TestCycle start : TestCycle::getRange())
				{
					TestCycle expected = start;
					TestCycle previous = start - 1;
					for (TestCycle n : TestCycle::getRange(start))
					{
						TEST_IS_EQ(n, expected);
						TEST_IS_EQ(TestCycle(n - 1), previous);
						expected++;
						previous = n;
						count++;
					}
				}
				TEST_IS_EQ(count, TestCycle::size * TestCycle::size);

				count = 0;
				for (TestCycle end : TestCycle::getRangeReversed())
				{
					TestCycle expected = end;
					TestCycle previous = end + 1;
					for (TestCycle n : TestCycle::getRangeReversed(end))
					{
						TEST_IS_EQ(n, expected);
						TEST_IS_EQ(TestCycle(n + 1), previous);
						expected--;
						previous = n;
						count++;
					}
				}
				TEST_IS_EQ(count, TestCycle::size * TestCycle::size);

				return {};
			}

			template <typename INTEGER_TYPE, INTEGER_TYPE START, INTEGER_TYPE END>
			constexpr Ash::Test::Assertion cycle()
			{
				TEST_GENERIC(cycleIdentity, INTEGER_TYPE, START, END);

				TEST_GENERIC(cycleInverse, INTEGER_TYPE, START, END);

				TEST_GENERIC(cycleRange, INTEGER_TYPE, START, END);

				return {};
			}
		}

		TEST_UNIT
		(
			testInteger,
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, int, 0, 128),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, int, 113, 409),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, int, 661, 809),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, int, -128, 128),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, int, -128, 0),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, int, -409, -113),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, int, -809, -661),
		);
	}
}
