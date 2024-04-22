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

					size = Ash::Integer::Value::getBitSize<0, 1>();
					TEST_IS_EQ(size, 8);

					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max()>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max()>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max()>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), true>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), true>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), true>();
					TEST_IS_EQ(size, 64);

					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), false>();
					TEST_IS_EQ(size, 8);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), false>();
					TEST_IS_EQ(size, 16);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), false>();
					TEST_IS_EQ(size, 32);
					size = Ash::Integer::Value::getBitSize<std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), false>();
					TEST_IS_EQ(size, 64);

					return {};
				}

				static Ash::Test::Assertion type()
				{
					bool isSame;

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<0, 1>, unsigned char>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()>, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()>, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()>, int8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max()>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max()>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max()>, uint64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max()>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max()>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max()>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max()>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max()>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max()>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), true>, int16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), true>, int32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), true>, int64_t>;
					TEST_IS_TRUE(isSame);

					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), false>, uint8_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), false>, uint16_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), false>, uint32_t>;
					TEST_IS_TRUE(isSame);
					isSame = Ash::Type::isSame<Ash::Integer::Value::WithRange<std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), false>, uint64_t>;
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
