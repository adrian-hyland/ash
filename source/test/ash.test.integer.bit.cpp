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

					for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>; n++)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
					}

					value = ~value;

					for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>; n++)
					{
						TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
					}

					for (INTEGER mask = ~0; mask != 0; mask = mask << 1)
					{
						value = value ^ mask;
					}

					for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>; n = n + 2)
					{
						TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
					}

					for (size_t n = 1; n < Ash::Integer::getBitSize<INTEGER>; n = n + 2)
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

					for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>; n++)
					{
						value = Ash::Integer::setBit(value, n);
						for (size_t m = 0; m <= n; m++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n + 1; m < Ash::Integer::getBitSize<INTEGER>; m++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
						}
					}

					value = 0;

					for (size_t n = Ash::Integer::getBitSize<INTEGER>; n > 0; n--)
					{
						value = Ash::Integer::setBit(value, n - 1);
						for (size_t m = 0; m < n - 1; m++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n; m < Ash::Integer::getBitSize<INTEGER>; m++)
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

					for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>; n++)
					{
						value = Ash::Integer::clearBit(value, n);
						for (size_t m = 0; m <= n; m++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n + 1; m < Ash::Integer::getBitSize<INTEGER>; m++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
						}
					}

					value = ~0;

					for (size_t n = Ash::Integer::getBitSize<INTEGER>; n > 0; n--)
					{
						value = Ash::Integer::clearBit(value, n - 1);
						for (size_t m = 0; m < n - 1; m++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, m));
						}
						for (size_t m = n; m < Ash::Integer::getBitSize<INTEGER>; m++)
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
					for (size_t from = 0; from < Ash::Integer::getBitSize<INTEGER>; from++)
					{
						for (size_t to = from; to < Ash::Integer::getBitSize<INTEGER>; to++)
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
							for (size_t n = to + 1; n < Ash::Integer::getBitSize<INTEGER>; n++)
							{
								TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
							}
						}
					}

					for (size_t from = 0; from < Ash::Integer::getBitSize<INTEGER>; from++)
					{
						for (size_t to = from; to < Ash::Integer::getBitSize<INTEGER>; to++)
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
							for (size_t n = to + 1; n < Ash::Integer::getBitSize<INTEGER>; n++)
							{
								TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
							}
						}
					}

					for (size_t from = 0; from < Ash::Integer::getBitSize<INTEGER>; from++)
					{
						INTEGER value = Ash::Integer::getBitMask<INTEGER>(from);

						for (size_t n = 0; n < from; n++)
						{
							TEST_IS_FALSE(Ash::Integer::hasBitSet(value, n));
						}
						for (size_t n = from; n < Ash::Integer::getBitSize<INTEGER>; n++)
						{
							TEST_IS_TRUE(Ash::Integer::hasBitSet(value, n));
						}
					}

					INTEGER value = Ash::Integer::getBitMask<INTEGER>(Ash::Integer::getBitSize<INTEGER>, Ash::Integer::getBitSize<INTEGER> + 1);
					TEST_IS_ZERO(value);

					value = Ash::Integer::getBitMask<INTEGER>(0, Ash::Integer::getBitSize<INTEGER>);
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
					TEST_IS_EQ(Ash::Integer::getBitLength(INTEGER(-1)), Ash::Integer::getBitSize<INTEGER>);

					for (size_t n = 0; n < Ash::Integer::getBitSize<INTEGER>; n++)
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
				static Ash::Test::Assertion getBitCount()
				{
					TEST_IS_EQ(Ash::Integer::getBitCount(INTEGER(0)), 0);
					TEST_IS_EQ(Ash::Integer::getBitCount(INTEGER(~0)), Ash::Integer::getBitSize<INTEGER>);

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
						for (size_t n = 0; (n < Ash::Integer::getBitSize<INTEGER>) && (offset < sizeof(valueCount) / sizeof(valueCount[0])); n = n + 8)
						{
							value = (value << 8) | valueCount[offset].value;
							count = count + valueCount[offset].count;
							
							TEST_IS_EQ(Ash::Integer::getBitCount(value), count);
							
							offset++;
						}
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

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCount, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCount, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCount, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitCount, uint64_t),
		);
	}
}
