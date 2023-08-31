#include "ash.integer.h"
#include "ash.test.integer.h"


namespace Ash
{
	namespace Test
	{
		namespace Integer
		{
			static Ash::Test::Assertion getValueSize()
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

			static Ash::Test::Assertion value()
			{
				bool isSame;

				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 1>, unsigned char>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()>, uint8_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()>, uint16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()>, uint32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()>, uint64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max()>, int8_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()>, int64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint16_t>::max()>, uint16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint32_t>::max()>, uint32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint64_t>::max()>, uint64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<int16_t>::max()>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<int32_t>::max()>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<int64_t>::max()>, int64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max()>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max()>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max()>, int64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 1>, uint8_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 2>, uint8_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 3>, uint16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 4>, uint16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 5>, uint32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 6>, uint32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 7>, uint32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 8>, uint32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 9>, uint64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 10>, uint64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 11>, uint64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 12>, uint64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 13>, uint64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 14>, uint64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 15>, uint64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 16>, uint64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 1>, int8_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 2>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 3>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 4>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 5>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 6>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 7>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 8>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 9>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 10>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 11>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 12>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 13>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 14>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, -15, 15>, int64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max(), 1, true>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max(), 1, true>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), 1, true>, int64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int8_t>::min(), std::numeric_limits<uint8_t>::max(), 1, false>, uint8_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int16_t>::min(), std::numeric_limits<uint16_t>::max(), 1, false>, uint16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int32_t>::min(), std::numeric_limits<uint32_t>::max(), 1, false>, uint32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<std::numeric_limits<int64_t>::min(), std::numeric_limits<uint64_t>::max(), 1, false>, uint64_t>();
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 1, true>, int8_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 2, true>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 3, true>, int16_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 4, true>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 5, true>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 6, true>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 7, true>, int32_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 8, true>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 9, true>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 10, true>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 11, true>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 12, true>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 13, true>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 14, true>, int64_t>();
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Integer::Value<0, 15, 15, true>, int64_t>();
				TEST_IS_TRUE(isSame);

				return {};
			}

			template
			<
				auto START,
				auto END
			>
			static Ash::Test::Assertion cycleIdentity()
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
			static Ash::Test::Assertion cycleInverse()
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
			static Ash::Test::Assertion cycleRange()
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
			static Ash::Test::Assertion cycle()
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

			template
			<
				typename INTEGER,
				typename = Ash::Type::IsUnsignedInteger<INTEGER>
			>
			static Ash::Test::Assertion bit()
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

			TEST_CASE(Ash::Test::Integer::getValueSize),
			TEST_CASE(Ash::Test::Integer::value),

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
