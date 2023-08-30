#pragma once

#include "ash.test.h"
#include "ash.integer.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			template
			<
				auto START,
				auto END
			>
			constexpr Ash::Test::Assertion cycleIdentity()
			{
				using TestCycle = Ash::Integer::Cycle<START, END>;

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

			template
			<
				auto START,
				auto END
			>
			constexpr Ash::Test::Assertion cycleInverse()
			{
				using TestCycle = Ash::Integer::Cycle<START, END>;

				for (TestCycle n : TestCycle::getRange())
				{
					TEST_IS_EQ(TestCycle(n + n.inverse()), TestCycle::identity());
				}

				return {};
			}

			template
			<
				auto START,
				auto END
			>
			constexpr Ash::Test::Assertion cycleRange()
			{
				using TestCycle = Ash::Integer::Cycle<START, END>;

				typename TestCycle::Size count = 0;
				TestCycle expected = TestCycle::minValue;
				TestCycle previous = expected - 1;
				for (TestCycle n : TestCycle::getRange())
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n - 1), previous);
					expected++;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, TestCycle::size);

				count = 0;
				expected = TestCycle::minValue - 1;
				previous = expected - 1;
				for (TestCycle n : TestCycle::getRange(TestCycle::minValue - 1))
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n - 1), previous);
					expected++;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, TestCycle::size);

				count = 0;
				expected = TestCycle::minValue + 1;
				previous = expected - 1;
				for (TestCycle n : TestCycle::getRange(TestCycle::minValue + 1))
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n - 1), previous);
					expected++;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, TestCycle::size);


				count = 0;
				expected = TestCycle::maxValue;
				previous = expected + 1;
				for (TestCycle n : TestCycle::getRangeReversed())
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n + 1), previous);
					expected--;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, TestCycle::size);

				count = 0;
				expected = TestCycle::maxValue - 1;
				previous = expected + 1;
				for (TestCycle n : TestCycle::getRangeReversed(TestCycle::maxValue - 1))
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n + 1), previous);
					expected--;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, TestCycle::size);

				count = 0;
				expected = TestCycle::maxValue + 1;
				previous = expected + 1;
				for (TestCycle n : TestCycle::getRangeReversed(TestCycle::maxValue + 1))
				{
					TEST_IS_EQ(n, expected);
					TEST_IS_EQ(TestCycle(n + 1), previous);
					expected--;
					previous = n;
					count++;
				}
				TEST_IS_EQ(count, TestCycle::size);

				return {};
			}

			template
			<
				auto START,
				auto END
			>
			constexpr Ash::Test::Assertion cycle()
			{
				TEST_GENERIC(cycleIdentity, START, END);

				TEST_GENERIC(cycleInverse, START, END);

				TEST_GENERIC(cycleRange, START, END);

				return {};
			}

			template
			<
				typename INTEGER,
				typename = Ash::Type::IsUnsignedInteger<INTEGER>
			>
			constexpr Ash::Test::Assertion hasBitSet()
			{
				INTEGER value = 0;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>(); n++)
				{
					TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
				}

				value = ~value;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>(); n++)
				{
					TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
				}

				for (INTEGER mask = ~0; mask != 0; mask = mask << 1)
				{
					value = value ^ mask;
				}

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>(); n = n + 2)
				{
					TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
				}

				for (size_t n = 1; n < Ash::Integer::getBitSize<INTEGER>(); n = n + 2)
				{
					TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
				}

				return {};
			}

			template
			<
				typename INTEGER,
				typename = Ash::Type::IsUnsignedInteger<INTEGER>
			>
			constexpr Ash::Test::Assertion setBit()
			{
				INTEGER value = 0;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>(); n++)
				{
					value = Ash::Integer::setBit(value, n);
					for (size_t m = 0; m <= n; m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n + 1; m < Ash::Integer::getBitSize<INTEGER>(); m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
				}

				value = 0;

				for (size_t n = Ash::Integer::getBitSize<INTEGER>(); n > 0; n--)
				{
					value = Ash::Integer::setBit(value, n - 1);
					for (size_t m = 0; m < n - 1; m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n; m < Ash::Integer::getBitSize<INTEGER>(); m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
				}

				return {};
			}

			template
			<
				typename INTEGER,
				typename = Ash::Type::IsUnsignedInteger<INTEGER>
			>
			constexpr Ash::Test::Assertion clearBit()
			{
				INTEGER value = ~0;

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>(); n++)
				{
					value = Ash::Integer::clearBit(value, n);
					for (size_t m = 0; m <= n; m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n + 1; m < Ash::Integer::getBitSize<INTEGER>(); m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
				}

				value = ~0;

				for (size_t n = Ash::Integer::getBitSize<INTEGER>(); n > 0; n--)
				{
					value = Ash::Integer::clearBit(value, n - 1);
					for (size_t m = 0; m < n - 1; m++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
					}
					for (size_t m = n; m < Ash::Integer::getBitSize<INTEGER>(); m++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
					}
				}

				return {};
			}

			template
			<
				typename INTEGER,
				typename = Ash::Type::IsUnsignedInteger<INTEGER>
			>
			constexpr Ash::Test::Assertion getBitMask()
			{
				for (size_t from = 0; from < Ash::Integer::getBitSize<INTEGER>(); from++)
				{
					for (size_t to = from; to < Ash::Integer::getBitSize<INTEGER>(); to++)
					{
						INTEGER value = Ash::Integer::getBitMask<INTEGER>(from, to);

						for (size_t n = 0; n < from; n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = from; n <= to; n++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = to + 1; n < Ash::Integer::getBitSize<INTEGER>(); n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
					}
				}

				for (size_t from = 0; from < Ash::Integer::getBitSize<INTEGER>(); from++)
				{
					for (size_t to = from; to < Ash::Integer::getBitSize<INTEGER>(); to++)
					{
						INTEGER value = Ash::Integer::getBitMask<INTEGER>(to, from);

						for (size_t n = 0; n < from; n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = from; n <= to; n++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = to + 1; n < Ash::Integer::getBitSize<INTEGER>(); n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
					}
				}

				return {};
			}

			template
			<
				typename INTEGER,
				typename = Ash::Type::IsUnsignedInteger<INTEGER>
			>
			constexpr Ash::Test::Assertion getBitSize()
			{
				TEST_IS_EQ(Ash::Integer::getBitSize(INTEGER(0)), 1);

				for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>(); n++)
				{
					INTEGER value = Ash::Integer::setBit(INTEGER(0), n);
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

			template
			<
				typename INTEGER,
				typename = Ash::Type::IsUnsignedInteger<INTEGER>
			>
			constexpr Ash::Test::Assertion bit()
			{
				TEST_GENERIC(hasBitSet, INTEGER);

				TEST_GENERIC(setBit, INTEGER);

				TEST_GENERIC(clearBit, INTEGER);

				TEST_GENERIC(getBitMask, INTEGER);

				TEST_GENERIC(getBitSize, INTEGER);

				return {};
			}
		}

		TEST_UNIT
		(
			testInteger,

			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max() - 1),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint8_t>::min() + 1, std::numeric_limits<uint8_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max() - 1),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint16_t>::min() + 1, std::numeric_limits<uint16_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max() - 1),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<uint32_t>::min() + 1, std::numeric_limits<uint32_t>::max()),

			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max() - 1),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int8_t>::min() + 1, std::numeric_limits<int8_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max() - 1),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int16_t>::min() + 1, std::numeric_limits<int16_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max() - 1),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, std::numeric_limits<int32_t>::min() + 1, std::numeric_limits<int32_t>::max()),

			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, 0, 128),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, 113, 409),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, 661, 809),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, -128, 128),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, -128, 0),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, -409, -113),
			TEST_CASE_GENERIC(Ash::Test::Integer::cycle, -809, -661),

			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::bit, uint64_t),
		);
	}
}
