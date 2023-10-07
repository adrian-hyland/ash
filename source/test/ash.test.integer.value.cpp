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
				static Ash::Test::Assertion getSize()
				{
					size_t size;

					size = Ash::Integer::getValueSize<0, 1>();
					TEST_IS_EQ(size, 8);

					size = Ash::Integer::getValueSize<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::getValueSize<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::getValueSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<0, 15, 1>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::getValueSize<0, 15, 2>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::getValueSize<0, 15, 3>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<0, 15, 4>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<0, 15, 5>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 6>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 7>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 8>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 9>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 10>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 11>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 12>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 13>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 14>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 15>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 16>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<0, -15, 1>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::getValueSize<0, -15, 2>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<0, -15, 3>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<0, -15, 4>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, -15, 5>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, -15, 6>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, -15, 7>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, -15, 8>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, -15, 9>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, -15, 10>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, -15, 11>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, -15, 12>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, -15, 13>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, -15, 14>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, -15, 15>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), 1, true>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), 1, true>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), 1, true>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), 1, false>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::getValueSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), 1, false>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), 1, false>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), 1, false>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::getValueSize<0, 15, 1, true>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::getValueSize<0, 15, 2, true>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<0, 15, 3, true>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::getValueSize<0, 15, 4, true>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 5, true>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 6, true>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 7, true>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::getValueSize<0, 15, 8, true>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 9, true>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 10, true>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 11, true>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 12, true>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 13, true>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 14, true>();
					TEST_IS_EQ(size, 64);
					size = Ash::Integer::getValueSize<0, 15, 15, true>();
					TEST_IS_EQ(size, 64);

					return {};
				}

				static Ash::Test::Assertion type()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 1>, unsigned char>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()>, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()>, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()>, int8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max()>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max()>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max()>, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max()>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max()>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max()>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max()>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max()>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max()>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 1>, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 2>, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 3>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 4>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 5>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 6>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 7>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 8>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 9>, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 10>, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 11>, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 12>, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 13>, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 14>, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 15>, uint64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 16>, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 1>, int8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 2>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 3>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 4>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 5>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 6>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 7>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 8>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 9>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 10>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 11>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 12>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 13>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 14>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 15>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), 1, true>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), 1, true>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), 1, true>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), 1, false>, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), 1, false>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), 1, false>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), 1, false>, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 1, true>, int8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 2, true>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 3, true>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 4, true>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 5, true>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 6, true>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 7, true>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 8, true>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 9, true>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 10, true>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 11, true>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 12, true>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 13, true>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 14, true>, int64_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 15, true>, int64_t>;
					TEST_IS_TRUE(isSame);

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testIntegerValue,

			TEST_CASE(Ash::Test::Integer::Value::getSize),
			TEST_CASE(Ash::Test::Integer::Value::type),
		);
	}
}
