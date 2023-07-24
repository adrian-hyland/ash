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
					TEST_IS_EQ(TestCycle::identity(), 0);
				}

				for (TestCycle n : TestCycle::getRange())
				{
					TEST_IS_EQ(TestCycle(n + TestCycle::identity()), n);
				}

				return {};
			}

			template <typename INTEGER_TYPE, INTEGER_TYPE START, INTEGER_TYPE END>
			constexpr Ash::Test::Assertion cycleInverse()
			{
				using TestCycle = Ash::Integer::Cycle<INTEGER_TYPE, START, END>;

				for (TestCycle n : TestCycle::getRange())
				{
					TEST_IS_EQ(TestCycle(n + n.inverse()), TestCycle::identity());
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

			template <typename INTEGER_TYPE>
			constexpr Ash::Test::Assertion hasBitSet()
			{
				INTEGER_TYPE value = 0;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n++)
				{
					TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
				}

				value = ~value;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n++)
				{
					TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
				}

				for (INTEGER_TYPE mask = ~0; mask != 0; mask = mask << 1)
				{
					value = value ^ mask;
				}

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n = n + 2)
				{
					TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
				}

				for (size_t n = 1; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n = n + 2)
				{
					TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
				}

				return {};
			}

			template <typename INTEGER_TYPE>
			constexpr Ash::Test::Assertion setBit()
			{
				INTEGER_TYPE value = 0;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n++)
				{
					value = Ash::Integer::setBit(value, n);
					for (size_t m = 0; m <= n; m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n + 1; m < Ash::Integer::getBitSize<INTEGER_TYPE>(); m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
				}

				value = 0;

				for (size_t n = Ash::Integer::getBitSize<INTEGER_TYPE>(); n > 0; n--)
				{
					value = Ash::Integer::setBit(value, n - 1);
					for (size_t m = 0; m < n - 1; m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n; m < Ash::Integer::getBitSize<INTEGER_TYPE>(); m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
				}

				return {};
			}

			template <typename INTEGER_TYPE>
			constexpr Ash::Test::Assertion clearBit()
			{
				INTEGER_TYPE value = ~0;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n++)
				{
					value = Ash::Integer::clearBit(value, n);
					for (size_t m = 0; m <= n; m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n + 1; m < Ash::Integer::getBitSize<INTEGER_TYPE>(); m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
				}

				value = ~0;

				for (size_t n = Ash::Integer::getBitSize<INTEGER_TYPE>(); n > 0; n--)
				{
					value = Ash::Integer::clearBit(value, n - 1);
					for (size_t m = 0; m < n - 1; m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n; m < Ash::Integer::getBitSize<INTEGER_TYPE>(); m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
				}

				return {};
			}

			template <typename INTEGER_TYPE>
			constexpr Ash::Test::Assertion getBitMask()
			{
				for (size_t from = 0; from < Ash::Integer::getBitSize<INTEGER_TYPE>(); from++)
				{
					for (size_t to = from; to < Ash::Integer::getBitSize<INTEGER_TYPE>(); to++)
					{
						INTEGER_TYPE value = Ash::Integer::getBitMask<INTEGER_TYPE>(from, to);

						for (size_t n = 0; n < from; n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = from; n <= to; n++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = to + 1; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
					}
				}

				for (size_t from = 0; from < Ash::Integer::getBitSize<INTEGER_TYPE>(); from++)
				{
					for (size_t to = from; to < Ash::Integer::getBitSize<INTEGER_TYPE>(); to++)
					{
						INTEGER_TYPE value = Ash::Integer::getBitMask<INTEGER_TYPE>(to, from);

						for (size_t n = 0; n < from; n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = from; n <= to; n++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = to + 1; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
					}
				}

				return {};
			}

			template <typename INTEGER_TYPE>
			constexpr Ash::Test::Assertion getBitSize()
			{
				TEST_IS_EQ(Ash::Integer::getBitSize(INTEGER_TYPE(0)), 1);

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER_TYPE>(); n++)
				{
					INTEGER_TYPE value = Ash::Integer::setBit(INTEGER_TYPE(0), n);
					TEST_IS_EQ(Ash::Integer::getBitSize(value), n + 1);
					if (n > 0)
					{
						value = Ash::Integer::setBit(value, 0);
						TEST_IS_EQ(Ash::Integer::getBitSize(value), n + 1);
						if (n > 1)
						{
							value = Ash::Integer::setBit(value, n - 1);
							TEST_IS_EQ(Ash::Integer::getBitSize(value), n + 1);
						}
					}
				}

				return {};
			}

			template <typename INTEGER_TYPE>
			constexpr Ash::Test::Assertion bit()
			{
				TEST_GENERIC(hasBitSet, INTEGER_TYPE);

				TEST_GENERIC(setBit, INTEGER_TYPE);

				TEST_GENERIC(clearBit, INTEGER_TYPE);

				TEST_GENERIC(getBitMask, INTEGER_TYPE);

				TEST_GENERIC(getBitSize, INTEGER_TYPE);

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
			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint64_t),
		);
	}
}
