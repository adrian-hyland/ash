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
				static Ash::Test::Assertion setBit()
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
				static Ash::Test::Assertion clearBit()
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
				static Ash::Test::Assertion getBitMask()
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
				static Ash::Test::Assertion getBitSize()
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

			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitSize, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitSize, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitSize, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Integer::Bit::getBitSize, uint64_t),
		);
	}
}
