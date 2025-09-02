#include <cstdint>
#include "ash.integer.bit.h"
#include "ash.test.integer.bit.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			namespace Bit
			{
				template
				<
					typename INTEGER,
					typename = Ash::Type::IsUnsignedInteger<INTEGER>
				>
				static Ash::Test::Assertion hasBitSet()
				{
					INTEGER value = 0;

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
					}

					value = ~value;

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
					}

					for (INTEGER mask = ~0; mask != 0; mask = mask << 1)
					{
						value = value ^ mask;
					}

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n = n + 2)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
					}

					for (size_t n = 1; n < Ash::Integer::bitSize<INTEGER>; n = n + 2)
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
				static Ash::Test::Assertion setBit()
				{
					INTEGER value = 0;

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						value = Ash::Integer::setBit(value, n);
						for (size_t m = 0; m <= n; m++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n + 1; m < Ash::Integer::bitSize<INTEGER>; m++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
						}
					}

					value = 0;

					for (size_t n = Ash::Integer::bitSize<INTEGER>; n > 0; n--)
					{
						value = Ash::Integer::setBit(value, n - 1);
						for (size_t m = 0; m < n - 1; m++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n; m < Ash::Integer::bitSize<INTEGER>; m++)
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
				static Ash::Test::Assertion clearBit()
				{
					INTEGER value = ~0;

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						value = Ash::Integer::clearBit(value, n);
						for (size_t m = 0; m <= n; m++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n + 1; m < Ash::Integer::bitSize<INTEGER>; m++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
						}
					}

					value = ~0;

					for (size_t n = Ash::Integer::bitSize<INTEGER>; n > 0; n--)
					{
						value = Ash::Integer::clearBit(value, n - 1);
						for (size_t m = 0; m < n - 1; m++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n; m < Ash::Integer::bitSize<INTEGER>; m++)
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
				static Ash::Test::Assertion getBitMask()
				{
					for (size_t from = 0; from < Ash::Integer::bitSize<INTEGER>; from++)
					{
						for (size_t to = from; to < Ash::Integer::bitSize<INTEGER>; to++)
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
							for (size_t n = to + 1; n < Ash::Integer::bitSize<INTEGER>; n++)
							{
								TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
							}
						}
					}

					for (size_t from = 0; from < Ash::Integer::bitSize<INTEGER>; from++)
					{
						for (size_t to = from; to < Ash::Integer::bitSize<INTEGER>; to++)
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
							for (size_t n = to + 1; n < Ash::Integer::bitSize<INTEGER>; n++)
							{
								TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
							}
						}
					}

					for (size_t from = 0; from < Ash::Integer::bitSize<INTEGER>; from++)
					{
						INTEGER value = Ash::Integer::getBitMask<INTEGER>(from);

						for (size_t n = 0; n < from; n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = from; n < Ash::Integer::bitSize<INTEGER>; n++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
						}
					}

					INTEGER value = Ash::Integer::getBitMask<INTEGER>(Ash::Integer::bitSize<INTEGER>, Ash::Integer::bitSize<INTEGER> + 1);
					TEST_IS_ZERO(value);

					value = Ash::Integer::getBitMask<INTEGER>(0, Ash::Integer::bitSize<INTEGER>);
					TEST_IS_EQ(value, INTEGER(-1));

					return {};
				}

				template
				<
					typename INTEGER,
					typename = Ash::Type::IsUnsignedInteger<INTEGER>
				>
				static Ash::Test::Assertion getBitLength()
				{
					TEST_IS_EQ(Ash::Integer::getBitLength(INTEGER(0)), 1);
					TEST_IS_EQ(Ash::Integer::getBitLength(INTEGER(-1)), Ash::Integer::bitSize<INTEGER>);

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						INTEGER value = Ash::Integer::setBit(INTEGER(0), n);
						TEST_IS_EQ(Ash::Integer::getBitLength(value), n + 1);
						if (n > 0)
						{
							value = Ash::Integer::setBit(value, 0);
							TEST_IS_EQ(Ash::Integer::getBitLength(value), n + 1);
							if (n > 1)
							{
								value = Ash::Integer::setBit(value, n - 1);
								TEST_IS_EQ(Ash::Integer::getBitLength(value), n + 1);
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
				static Ash::Test::Assertion getBitCountSet()
				{
					TEST_IS_EQ(Ash::Integer::getBitCount(INTEGER(0), true), 0);
					TEST_IS_EQ(Ash::Integer::getBitCount(INTEGER(~0), true), Ash::Integer::bitSize<INTEGER>);

					static struct
					{
						uint8_t value;
						size_t  count;
					} valueCount[] =
					{
						{ 0x01, 1 },
						{ 0x12, 2 },
						{ 0x23, 3 },
						{ 0x34, 3 },
						{ 0x45, 3 },
						{ 0x56, 4 },
						{ 0x67, 5 },
						{ 0x78, 4 },
						{ 0x89, 3 },
						{ 0x9A, 4 },
						{ 0xAB, 5 },
						{ 0xBC, 5 },
						{ 0xCD, 5 },
						{ 0xDE, 6 },
						{ 0xEF, 7 },
						{ 0xF0, 4 }
					};

					size_t offset = 0;
					while (offset < sizeof(valueCount) / sizeof(valueCount[0]))
					{
						INTEGER value = 0;
						size_t count = 0;
						for (size_t n = 0; (n < Ash::Integer::bitSize<INTEGER>) && (offset < sizeof(valueCount) / sizeof(valueCount[0])); n = n + 8)
						{
							value = (value << 8) | valueCount[offset].value;
							count = count + valueCount[offset].count;

							TEST_IS_EQ(Ash::Integer::getBitCount(value, true), count);

							offset++;
						}
					}

					return {};
				}

				template
				<
					typename INTEGER,
					typename = Ash::Type::IsUnsignedInteger<INTEGER>
				>
				static Ash::Test::Assertion getBitCountClear()
				{
					TEST_IS_EQ(Ash::Integer::getBitCount(INTEGER(~0), false), 0);
					TEST_IS_EQ(Ash::Integer::getBitCount(INTEGER(0), false), Ash::Integer::bitSize<INTEGER>);

					static struct
					{
						uint8_t value;
						size_t  count;
					} valueCount[] =
					{
						{ 0x01, 7 },
						{ 0x12, 6 },
						{ 0x23, 5 },
						{ 0x34, 5 },
						{ 0x45, 5 },
						{ 0x56, 4 },
						{ 0x67, 3 },
						{ 0x78, 4 },
						{ 0x89, 5 },
						{ 0x9A, 4 },
						{ 0xAB, 3 },
						{ 0xBC, 3 },
						{ 0xCD, 3 },
						{ 0xDE, 2 },
						{ 0xEF, 1 },
						{ 0xF0, 4 }
					};

					size_t offset = 0;
					while (offset < sizeof(valueCount) / sizeof(valueCount[0]))
					{
						INTEGER value = ~0;
						size_t count = 0;
						for (size_t n = 0; (n < Ash::Integer::bitSize<INTEGER>) && (offset < sizeof(valueCount) / sizeof(valueCount[0])); n = n + 8)
						{
							value = (value << 8) | valueCount[offset].value;
							count = count + valueCount[offset].count;

							TEST_IS_EQ(Ash::Integer::getBitCount(value, false), count);

							offset++;
						}
					}

					return {};
				}

				template
				<
					typename INTEGER,
					typename = Ash::Type::IsUnsignedInteger<INTEGER>
				>
				static Ash::Test::Assertion getLeadingBitCountClear()
				{
					INTEGER value = ~0;
					TEST_IS_EQ(Ash::Integer::getLeadingBitCount(value, false), 0);

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						value = Ash::Integer::clearBit<INTEGER>(value, Ash::Integer::bitSize<INTEGER> - n - 1);
						TEST_IS_EQ(Ash::Integer::getLeadingBitCount(value, false), n + 1);
					}
					return {};
				}

				template
				<
					typename INTEGER,
					typename = Ash::Type::IsUnsignedInteger<INTEGER>
				>
				static Ash::Test::Assertion getLeadingBitCountSet()
				{
					INTEGER value = 0;
					TEST_IS_EQ(Ash::Integer::getLeadingBitCount(value, true), 0);

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						value = Ash::Integer::setBit(value, Ash::Integer::bitSize<INTEGER> - n - 1);
						TEST_IS_EQ(Ash::Integer::getLeadingBitCount(value, true), n + 1);
					}
					return {};
				}

				template
				<
					typename INTEGER,
					typename = Ash::Type::IsUnsignedInteger<INTEGER>
				>
				static Ash::Test::Assertion getTrailingBitCountClear()
				{
					INTEGER value = ~0;
					TEST_IS_EQ(Ash::Integer::getTrailingBitCount(value, false), 0);

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						value = Ash::Integer::clearBit<INTEGER>(value, n);
						TEST_IS_EQ(Ash::Integer::getTrailingBitCount(value, false), n + 1);
					}
					return {};
				}

				template
				<
					typename INTEGER,
					typename = Ash::Type::IsUnsignedInteger<INTEGER>
				>
				static Ash::Test::Assertion getTrailingBitCountSet()
				{
					INTEGER value = 0;
					TEST_IS_EQ(Ash::Integer::getTrailingBitCount(value, true), 0);

					for (size_t n = 0; n < Ash::Integer::bitSize<INTEGER>; n++)
					{
						value = Ash::Integer::setBit(value, n);
						TEST_IS_EQ(Ash::Integer::getTrailingBitCount(value, true), n + 1);
					}
					return {};
				}
			}
		}

		TEST_UNIT
		(
			testIntegerBit,

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::hasBitSet, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::hasBitSet, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::hasBitSet, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::hasBitSet, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::setBit, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::setBit, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::setBit, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::setBit, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::clearBit, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::clearBit, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::clearBit, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::clearBit, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitMask, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitMask, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitMask, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitMask, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitLength, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitLength, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitLength, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitLength, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountSet, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountSet, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountSet, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountSet, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountClear, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountClear, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountClear, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCountClear, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountClear, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountClear, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountClear, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountClear, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountSet, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountSet, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountSet, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getLeadingBitCountSet, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountClear, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountClear, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountClear, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountClear, uint64_t),

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountSet, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountSet, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountSet, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getTrailingBitCountSet, uint64_t),
		);
	}
}
