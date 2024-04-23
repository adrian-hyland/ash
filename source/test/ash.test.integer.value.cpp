#include <limits>
#include "ash.integer.value.h"
#include "ash.test.integer.value.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			namespace Signed
			{
				static Ash::Test::Assertion withBitSize()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithBitSize<8>::Type, int8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithBitSize<16>::Type, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithBitSize<32>::Type, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithBitSize<64>::Type, int64_t>;
					TEST_IS_TRUE(isSame);

					return {};
				}

				static Ash::Test::Assertion withMaxBitSize()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithMaxBitSize<8, 64>::Type, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithMaxBitSize<64, 8>::Type, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithMaxBitSize<8, 16, 32, 64>::Type, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithMaxBitSize<64, 8, 16, 32>::Type, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithMaxBitSize<32, 64, 8, 16>::Type, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Signed::WithMaxBitSize<16, 32, 64, 8>::Type, int64_t>;
					TEST_IS_TRUE(isSame);

					return {};
				}
			}

			namespace Unsigned
			{
				static Ash::Test::Assertion withBitSize()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithBitSize<8>::Type, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithBitSize<16>::Type, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithBitSize<32>::Type, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithBitSize<64>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);

					return {};
				}

				static Ash::Test::Assertion withMaxBitSize()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithMaxBitSize<8, 64>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithMaxBitSize<64, 8>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithMaxBitSize<8, 16, 32, 64>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithMaxBitSize<64, 8, 16, 32>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithMaxBitSize<32, 64, 8, 16>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Unsigned::WithMaxBitSize<16, 32, 64, 8>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);

					return {};
				}
			}

			namespace Value
			{
				static Ash::Test::Assertion getRangeBitSize()
				{
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(0, 1), 8);

					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()), 8);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()), 16);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()), 32);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()), 64);

					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()), 8);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()), 16);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()), 32);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()), 64);

					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max()), 16);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max()), 32);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max()), 64);

					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max()), 16);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max()), 32);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max()), 64);

					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max()), 16);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max()), 32);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max()), 64);

					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), true), 16);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), true), 32);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), true), 64);

					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), false), 8);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), false), 16);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), false), 32);
					TEST_IS_EQ(Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), false), 64);

					return {};
				}

				static Ash::Test::Assertion withBitSize()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<8, false>::Type, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<16, false>::Type, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<32, false>::Type, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<64, false>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<8, true>::Type, int8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<16, true>::Type, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<32, true>::Type, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithBitSize<64, true>::Type, int64_t>;
					TEST_IS_TRUE(isSame);

					return {};
				}

				static Ash::Test::Assertion withRange()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<0, 1>::Type, uint8_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()>::Type, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()>::Type, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()>::Type, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()>::Type, int8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()>::Type, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()>::Type, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()>::Type, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max()>::Type, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max()>::Type, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max()>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max()>::Type, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max()>::Type, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max()>::Type, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max()>::Type, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max()>::Type, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max()>::Type, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), true>::Type, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), true>::Type, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), true>::Type, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), false>::Type, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), false>::Type, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), false>::Type, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), false>::Type, uint64_t>;
					TEST_IS_TRUE(isSame);

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testIntegerValue,

			TEST_CASE(Ash::Test::Integer::Signed::withBitSize),
			TEST_CASE(Ash::Test::Integer::Signed::withMaxBitSize),

			TEST_CASE(Ash::Test::Integer::Unsigned::withBitSize),
			TEST_CASE(Ash::Test::Integer::Unsigned::withMaxBitSize),

			TEST_CASE(Ash::Test::Integer::Value::getRangeBitSize),
			TEST_CASE(Ash::Test::Integer::Value::withBitSize),
			TEST_CASE(Ash::Test::Integer::Value::withRange),
		);
	}
}
