#include "ash.integer.cycle.h"
#include "ash.test.integer.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			static Ash::Test::Assertion cycleIdentity()
			{
				using TestCycle = Ash::Integer::Cycle<-113, 113>;
				TEST_IS_EQ(TestCycle::identity(), 0);

				using TestCycle1 = Ash::Integer::Cycle<113, 409>;
				for (TestCycle1 n : TestCycle1::iterate())
				{
					TEST_IS_EQ(TestCycle1(n + TestCycle1::identity()), n);
				}

				using TestCycle2 = Ash::Integer::Cycle<-409, -113>;
				for (TestCycle2 n : TestCycle2::iterate())
				{
					TEST_IS_EQ(TestCycle2(n + TestCycle2::identity()), n);
				}

				return {};
			}

			static Ash::Test::Assertion cycleInverse()
			{
				using TestCycle1 = Ash::Integer::Cycle<113, 409>;
				for (TestCycle1 n : TestCycle1::iterate())
				{
					TEST_IS_EQ(TestCycle1(n + n.inverse()), TestCycle1::identity());
				}

				using TestCycle2 = Ash::Integer::Cycle<-409, -113>;
				for (TestCycle2 n : TestCycle2::iterate())
				{
					TEST_IS_EQ(TestCycle2(n + n.inverse()), TestCycle2::identity());
				}

				return {};
			}

			template
			<
				auto START,
				auto END
			>
			static Ash::Test::Assertion cycleIterate()
			{
				using TestCycle = Ash::Integer::Cycle<START, END>;

				typename TestCycle::Size count = 0;
				TestCycle expected = TestCycle::minimum;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::minimum, TestCycle::minimum))
				{
					TEST_IS_EQ(n, expected);
					count++;
				}
				TEST_IS_EQ(count, 1);

				count = 0;
				expected = TestCycle::maximum;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::maximum, TestCycle::maximum))
				{
					TEST_IS_EQ(n, expected);
					count++;
				}
				TEST_IS_EQ(count, 1);

				count = 0;
				expected = TestCycle::minimum;
				TestCycle previous = expected - 1;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::minimum, TestCycle::minimum + 1))
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n - 1), previous);
					expected++;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				expected = TestCycle::maximum - 1;
				previous = expected - 1;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::maximum - 1, TestCycle::maximum))
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n - 1), previous);
					expected++;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				expected = TestCycle::maximum;
				previous = expected - 1;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::maximum, TestCycle::minimum))
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n - 1), previous);
					expected++;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, 2);


				count = 0;
				expected = TestCycle::minimum;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::minimum, TestCycle::minimum).reverse())
				{
					TEST_IS_EQ(n, expected);
					count++;
				}
				TEST_IS_EQ(count, 1);

				count = 0;
				expected = TestCycle::maximum;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::maximum, TestCycle::maximum).reverse())
				{
					TEST_IS_EQ(n, expected);
					count++;
				}
				TEST_IS_EQ(count, 1);

				count = 0;
				expected = TestCycle::minimum + 1;
				previous = expected + 1;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::minimum, TestCycle::minimum + 1).reverse())
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n + 1), previous);
					expected--;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				expected = TestCycle::maximum;
				previous = expected + 1;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::maximum - 1, TestCycle::maximum).reverse())
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n + 1), previous);
					expected--;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				expected = TestCycle::minimum;
				previous = expected + 1;
				for (TestCycle n : TestCycle::iterateBetween(TestCycle::maximum, TestCycle::minimum).reverse())
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n + 1), previous);
					expected--;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, 2);

				return {};
			}

			static Ash::Test::Assertion cycleConversion()
			{
				{
					Ash::Integer::Cycle<0, 10> cycle;
					uint8_t expected = 1;

					for (unsigned int n = 0; n < 8; n++)
					{
						cycle = uint8_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.size)
						{
							expected = expected - cycle.size;
						}
					}
					for (unsigned int n = 8; n < 16; n++)
					{
						cycle = uint16_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.size)
						{
							expected = expected - cycle.size;
						}
					}
					for (unsigned int n = 16; n < 32; n++)
					{
						cycle = uint32_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.size)
						{
							expected = expected - cycle.size;
						}
					}
					for (unsigned int n = 32; n < 64; n++)
					{
						cycle = uint64_t(1ULL << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.size)
						{
							expected = expected - cycle.size;
						}
					}
				}

				{
					Ash::Integer::Cycle<-5, 5> cycle;
					int8_t expected = 1;

					for (unsigned int n = 0; n < 7; n++)
					{
						cycle = int8_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}
					for (unsigned int n = 7; n < 15; n++)
					{
						cycle = int16_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}
					for (unsigned int n = 15; n < 31; n++)
					{
						cycle = int32_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}
					for (unsigned int n = 31; n < 63; n++)
					{
						cycle = int64_t(1ULL << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}

					expected = -1;

					for (unsigned int n = 0; n < 7; n++)
					{
						cycle = -int8_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}
					for (unsigned int n = 7; n < 15; n++)
					{
						cycle = -int16_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}
					for (unsigned int n = 15; n < 31; n++)
					{
						cycle = -int32_t(1 << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}
					for (unsigned int n = 31; n < 63; n++)
					{
						cycle = -int64_t(1ULL << n);
						TEST_IS_EQ(cycle, expected);
						expected = expected * 2;
						if (expected > cycle.maximum)
						{
							expected = expected - cycle.size;
						}
						else if (expected < cycle.minimum)
						{
							expected = expected + cycle.size;
						}
					}
				}

				return {};
			}
		}

		TEST_UNIT
		(
			testIntegerCycle,

			TEST_CASE_GENERIC(Ash::Test::Integer::cycleIterate, std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycleIterate, std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycleIterate, std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()),

			TEST_CASE_GENERIC(Ash::Test::Integer::cycleIterate, std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycleIterate, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycleIterate, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()),

			TEST_CASE(Ash::Test::Integer::cycleIdentity),

			TEST_CASE(Ash::Test::Integer::cycleInverse),

			TEST_CASE(Ash::Test::Integer::cycleConversion)
		);
	}
}
