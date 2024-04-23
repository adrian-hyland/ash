#include <limits>
#include "ash.integer.value.h"
#include "ash.test.integer.value.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			namespace Value
			{
				static Ash::Test::Assertion getRangeBitSize()
				{
					size_t size;

					size = Ash::Integer::Value::getRangeBitSize(0, 1);
					TEST_IS_EQ(size, 8);

					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max());
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max());
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max());
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max());
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max());
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max());
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max());
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max());
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max());
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max());
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max());
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max());
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max());
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), true);
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), true);
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), true);
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), false);
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), false);
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), false);
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getRangeBitSize(std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), false);
					TEST_IS_EQ(size, 64);

					return {};
				}

				static Ash::Test::Assertion withRange()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<0, 1>::Type, unsigned char>;
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

			TEST_CASE(Ash::Test::Integer::Value::getRangeBitSize),
			TEST_CASE(Ash::Test::Integer::Value::withRange),
		);
	}
}
