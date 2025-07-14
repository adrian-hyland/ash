#include <limits>
#include "ash.integer.math.h"
#include "ash.test.integer.math.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			namespace Math
			{
				static Ash::Test::Assertion isEven()
				{
					TEST_IS_FALSE(Ash::Integer::isEven(std::numeric_limits<uint64_t>::max()));
					TEST_IS_TRUE(Ash::Integer::isEven(uint64_t(std::numeric_limits<uint32_t>::max()) + 1));
					TEST_IS_FALSE(Ash::Integer::isEven(std::numeric_limits<uint32_t>::max()));
					TEST_IS_TRUE(Ash::Integer::isEven(uint32_t(std::numeric_limits<uint16_t>::max()) + 1));
					TEST_IS_FALSE(Ash::Integer::isEven(std::numeric_limits<uint16_t>::max()));
					TEST_IS_TRUE(Ash::Integer::isEven(uint16_t(std::numeric_limits<uint8_t>::max()) + 1));
					TEST_IS_FALSE(Ash::Integer::isEven(std::numeric_limits<uint8_t>::max()));
					TEST_IS_TRUE(Ash::Integer::isEven(0));
					TEST_IS_TRUE(Ash::Integer::isEven(std::numeric_limits<int8_t>::min()));
					TEST_IS_FALSE(Ash::Integer::isEven(int16_t(std::numeric_limits<int8_t>::min()) - 1));
					TEST_IS_TRUE(Ash::Integer::isEven(std::numeric_limits<int16_t>::min()));
					TEST_IS_FALSE(Ash::Integer::isEven(int32_t(std::numeric_limits<int16_t>::min()) - 1));
					TEST_IS_TRUE(Ash::Integer::isEven(std::numeric_limits<int32_t>::min()));
					TEST_IS_FALSE(Ash::Integer::isEven(int64_t(std::numeric_limits<int32_t>::min()) - 1));
					TEST_IS_TRUE(Ash::Integer::isEven(std::numeric_limits<int64_t>::min()));

					return {};
				}

				static Ash::Test::Assertion isOdd()
				{
					TEST_IS_TRUE(Ash::Integer::isOdd(std::numeric_limits<uint64_t>::max()));
					TEST_IS_FALSE(Ash::Integer::isOdd(uint64_t(std::numeric_limits<uint32_t>::max()) + 1));
					TEST_IS_TRUE(Ash::Integer::isOdd(std::numeric_limits<uint32_t>::max()));
					TEST_IS_FALSE(Ash::Integer::isOdd(uint32_t(std::numeric_limits<uint16_t>::max()) + 1));
					TEST_IS_TRUE(Ash::Integer::isOdd(std::numeric_limits<uint16_t>::max()));
					TEST_IS_FALSE(Ash::Integer::isOdd(uint16_t(std::numeric_limits<uint8_t>::max()) + 1));
					TEST_IS_TRUE(Ash::Integer::isOdd(std::numeric_limits<uint8_t>::max()));
					TEST_IS_FALSE(Ash::Integer::isOdd(0));
					TEST_IS_FALSE(Ash::Integer::isOdd(std::numeric_limits<int8_t>::min()));
					TEST_IS_TRUE(Ash::Integer::isOdd(int16_t(std::numeric_limits<int8_t>::min()) - 1));
					TEST_IS_FALSE(Ash::Integer::isOdd(std::numeric_limits<int16_t>::min()));
					TEST_IS_TRUE(Ash::Integer::isOdd(int32_t(std::numeric_limits<int16_t>::min()) - 1));
					TEST_IS_FALSE(Ash::Integer::isOdd(std::numeric_limits<int32_t>::min()));
					TEST_IS_TRUE(Ash::Integer::isOdd(int64_t(std::numeric_limits<int32_t>::min()) - 1));
					TEST_IS_FALSE(Ash::Integer::isOdd(std::numeric_limits<int64_t>::min()));

					return {};
				}

				static Ash::Test::Assertion greatestCommonDivisor()
				{
					constexpr uint8_t u8 = uint8_t(1) << 7;
					constexpr uint16_t u16 = uint16_t(1) << 15;
					constexpr uint32_t u32 = uint32_t(1) << 31;
					constexpr uint64_t u64 = uint64_t(1) << 63;
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(0, 0), 0);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(0, 2), 2);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(2, 0), 2);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(2, 2), 2);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(2, -2), 2);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(-2, 2), 2);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(-2, -2), 2);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u8,  u16), u8);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u8,  u32), u8);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u8,  u64), u8);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u16, u8),  u8);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u16, u32), u16);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u16, u64), u16);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u32, u8),  u8);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u32, u16), u16);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u32, u64), u32);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u64, u8),  u8);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u64, u16), u16);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u64, u32), u32);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(u8, u16, u32, u64), u8);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(3 * 5 * 7, 11 * 13 * 17), 1);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(3 * 5 * 11 * 17, 2 * 7 * 13 * 17), 17);
					TEST_IS_EQ(Ash::Integer::greatestCommonDivisor(3 * 11 * 19 * 23, 5 * 13 * 19 * 29, 7 * 17 * 19 * 31), 19);

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testIntegerMath,

			TEST_CASE(Ash::Test::Integer::Math::isEven),
			TEST_CASE(Ash::Test::Integer::Math::isOdd),
			TEST_CASE(Ash::Test::Integer::Math::greatestCommonDivisor)
		);
	}
}
