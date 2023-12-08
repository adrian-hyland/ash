#include "ash.size.h"
#include "ash.test.size.h"


namespace Ash
{
	namespace Test
	{
		namespace Size
		{
			constexpr Ash::Size MaximumSize = Ash::Size(SIZE_MAX);

			constexpr Ash::Size InvalidSize = MaximumSize.add(1);

			static Ash::Test::Assertion getValue()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				TEST_IS_TRUE(size.isValid());

				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_ZERO(sizeValue);

				TEST_IS_ZERO(size.getValueOr(SIZE_MAX));

				size = MaximumSize;

				TEST_IS_TRUE(size.isValid());

				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX);

				TEST_IS_EQ(size.getValueOr(0), SIZE_MAX);

				size = InvalidSize;

				TEST_IS_FALSE(size.isValid());

				TEST_IS_FALSE(size.getValue(sizeValue));

				TEST_IS_EQ(size.getValueOr(123), 123);

				return {};
			}

			static Ash::Test::Assertion add()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				for (size_t value = 1; value < SIZE_MAX / 2 + 1; value = value * 2)
				{
					size = Ash::Size(value).add(SIZE_MAX - value);
					TEST_IS_TRUE(size.isValid());
					TEST_IS_TRUE(size.getValue(sizeValue));
					TEST_IS_EQ(sizeValue, SIZE_MAX);
				}

				// valid + constant -> valid
				size = Ash::Size(1).add(2);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 3);

				// valid + constant -> invalid
				size = Ash::Size(1).add(SIZE_MAX);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid + constant -> invalid
				size = InvalidSize.add(1);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid + valid -> valid
				size = Ash::Size(3).add(Ash::Size(4));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 7);

				// valid + valid -> invalid
				size = Ash::Size(1).add(MaximumSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid + valid -> invalid
				size = InvalidSize.add(Ash::Size(1));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid + invalid -> invalid
				size = InvalidSize.add(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid + invalid -> invalid
				size = Ash::Size(1).add(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion subtract()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				for (size_t value = 1; value < SIZE_MAX / 2 + 1; value = value * 2)
				{
					size = Ash::Size(SIZE_MAX).subtract(SIZE_MAX - value);
					TEST_IS_TRUE(size.isValid());
					TEST_IS_TRUE(size.getValue(sizeValue));
					TEST_IS_EQ(sizeValue, value);
				}

				// valid - constant -> valid
				size = Ash::Size(3).subtract(1);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 2);

				// valid - constant -> invalid
				size = Ash::Size(0).subtract(1);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid - constant -> invalid
				size = InvalidSize.subtract(1);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid - valid -> valid
				size = Ash::Size(7).subtract(Ash::Size(4));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 3);

				// valid - valid -> invalid
				size = Ash::Size(0).subtract(MaximumSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid - valid -> invalid
				size = InvalidSize.subtract(Ash::Size(1));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid - invalid -> invalid
				size = InvalidSize.subtract(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid - invalid -> invalid
				size = MaximumSize.subtract(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion multiply()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				for (size_t left = 1; left < sizeof(size_t) * 8; left++)
				{
					for (size_t right = 1; right < sizeof(size_t) * 8; right++)
					{
						size_t leftMask = (size_t(1) << left) - 1;
						size_t rightMask = (size_t(1) << right) - 1;
						size = Ash::Size(leftMask).multiply(rightMask);
						TEST_IS_EQ(size.isValid(), left + right - 1 < sizeof(size_t) * 8);
						if (size.isValid())
						{
							TEST_IS_TRUE(size.getValue(sizeValue));
							TEST_IS_EQ(sizeValue, leftMask * rightMask);
						}
					}
				}

				// valid * constant -> valid
				size = Ash::Size(3).multiply(2);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 6);

				// valid * constant -> invalid
				size = MaximumSize.multiply(SIZE_MAX);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid * constant -> invalid
				size = InvalidSize.multiply(1);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid * valid -> valid
				size = Ash::Size(5).multiply(Ash::Size(4));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 20);

				// valid * valid -> invalid
				size = MaximumSize.multiply(MaximumSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid * valid -> invalid
				size = InvalidSize.multiply(Ash::Size(1));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid * invalid -> invalid
				size = InvalidSize.multiply(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid * invalid -> invalid
				size = MaximumSize.multiply(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion divide()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				for (size_t value = 0; value < sizeof(SIZE_MAX) * 8; value++)
				{
					size = Ash::Size(SIZE_MAX).divide(size_t(1) << value);
					TEST_IS_TRUE(size.isValid());
					TEST_IS_TRUE(size.getValue(sizeValue));
					TEST_IS_EQ(sizeValue, SIZE_MAX >> value);
				}

				// valid / constant -> valid
				size = Ash::Size(8).divide(2);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 4);

				// valid / constant -> invalid
				size = MaximumSize.divide(0);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid / constant -> invalid
				size = InvalidSize.divide(2);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid / valid -> valid
				size = Ash::Size(16).divide(Ash::Size(8));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 2);

				// valid / valid -> invalid
				size = Ash::Size(1).divide(Ash::Size(0));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid / valid -> invalid
				size = InvalidSize.divide(Ash::Size(1));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid / invalid -> invalid
				size = InvalidSize.divide(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid / invalid -> invalid
				size = MaximumSize.divide(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion remainder()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				for (size_t value = 0; value < sizeof(SIZE_MAX) * 8; value++)
				{
					size = Ash::Size(SIZE_MAX).remainder(size_t(1) << value);
					TEST_IS_TRUE(size.isValid());
					TEST_IS_TRUE(size.getValue(sizeValue));
					TEST_IS_EQ(sizeValue, (size_t(1) << value) - 1);
				}

				// valid % constant -> valid
				size = Ash::Size(8).remainder(3);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 2);

				// valid % constant -> invalid
				size = MaximumSize.remainder(0);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid % constant -> invalid
				size = InvalidSize.remainder(2);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid % valid -> valid
				size = Ash::Size(43).remainder(Ash::Size(8));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 3);

				// valid % valid -> invalid
				size = Ash::Size(1).remainder(Ash::Size(0));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid % valid -> invalid
				size = InvalidSize.remainder(Ash::Size(1));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid % invalid -> invalid
				size = InvalidSize.remainder(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid % invalid -> invalid
				size = MaximumSize.remainder(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion roundUp()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				// valid roundUp constant -> valid
				size = Ash::Size(0).roundUp(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 32);
				size = Ash::Size(31).roundUp(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 32);
				size = Ash::Size(32).roundUp(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 32);
				size = Ash::Size(33).roundUp(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 64);
				size = Ash::Size(63).roundUp(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 64);
				size = Ash::Size(64).roundUp(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 64);

				// valid roundUp constant -> invalid
				size = MaximumSize.roundUp(0);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));
				size = MaximumSize.roundUp(2);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid roundUp constant -> invalid
				size = InvalidSize.roundUp(1);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid roundUp valid -> valid
				size = Ash::Size(43).roundUp(Ash::Size(8));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 48);

				// valid roundUp valid -> invalid
				size = Ash::Size(1).roundUp(Ash::Size(0));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));
				size = MaximumSize.roundUp(Ash::Size(2));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid roundUp valid -> invalid
				size = InvalidSize.roundUp(Ash::Size(1));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid roundUp invalid -> invalid
				size = InvalidSize.roundUp(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid roundUp invalid -> invalid
				size = MaximumSize.roundUp(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion roundDown()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				// valid roundDown constant -> valid
				size = Ash::Size(0).roundDown(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 0);
				size = Ash::Size(31).roundDown(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 0);
				size = Ash::Size(32).roundDown(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 32);
				size = Ash::Size(33).roundDown(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 32);
				size = Ash::Size(63).roundDown(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 32);
				size = Ash::Size(64).roundDown(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 64);

				// valid roundDown constant -> invalid
				size = MaximumSize.roundDown(0);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid roundDown constant -> invalid
				size = InvalidSize.roundDown(1);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid roundDown valid -> valid
				size = Ash::Size(43).roundDown(Ash::Size(8));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 40);

				// valid roundDown valid -> invalid
				size = Ash::Size(1).roundDown(Ash::Size(0));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid roundDown valid -> invalid
				size = InvalidSize.roundDown(Ash::Size(1));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid roundDown invalid -> invalid
				size = InvalidSize.roundDown(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid roundDown invalid -> invalid
				size = MaximumSize.roundDown(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion percent()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				// valid percent constant -> valid
				size = Ash::Size(100).percent(100);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 100);

				size = Ash::Size(100).percent(99);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 99);

				size = Ash::Size(100).percent(50);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 50);

				size = Ash::Size(100).percent(1);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 1);

				size = Ash::Size(0).percent(0);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 0);

				size = MaximumSize.percent(100);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX);

				size = MaximumSize.percent(99);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX - SIZE_MAX / 100 - 1);

				size = MaximumSize.percent(50);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX / 2);

				size = MaximumSize.percent(1);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX / 100);

				size = MaximumSize.percent(0);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 0);

				// valid percent constant -> invalid value
				size = MaximumSize.percent(101);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid percent constant -> invalid value
				size = InvalidSize.percent(100);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid percent valid -> valid
				size = Ash::Size(100).percent(Ash::Size(100));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 100);

				// valid percent valid -> invalid
				size = MaximumSize.percent(Ash::Size(101));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid percent valid -> invalid
				size = InvalidSize.percent(Ash::Size(100));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid percent invalid -> invalid
				size = InvalidSize.percent(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid percent invalid -> invalid
				size = MaximumSize.percent(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}

			static Ash::Test::Assertion reversePercent()
			{
				Ash::Size size;
				size_t sizeValue = 0;

				// valid reversePercent constant -> valid
				size = Ash::Size(200).reversePercent(100);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 100);

				size = Ash::Size(199).reversePercent(99);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 99);

				size = Ash::Size(150).reversePercent(50);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 50);

				size = Ash::Size(101).reversePercent(1);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 1);

				size = Ash::Size(100).reversePercent(0);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 0);

				size = MaximumSize.reversePercent(100);
				size = size.add(size.percent(100));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX - 1);

				size = MaximumSize.subtract(MaximumSize.reversePercent(99));
				size = size.add(size.percent(99));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX);

				size = MaximumSize.subtract(MaximumSize.reversePercent(50));
				size = size.add(size.percent(50));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX);

				size = MaximumSize.subtract(MaximumSize.reversePercent(1));
				size = size.add(size.percent(1));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX);

				size = MaximumSize.subtract(MaximumSize.reversePercent(0));
				size = size.add(size.percent(0));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, SIZE_MAX);

				// invalid reversePercent constant -> invalid
				size = MaximumSize.reversePercent(101);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid reversePercent constant -> invalid
				size = InvalidSize.reversePercent(100);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid reversePercent valid -> valid
				size = Ash::Size(200).reversePercent(Ash::Size(100));
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 100);

				// valid reversePercent valid -> invalid
				size = MaximumSize.reversePercent(Ash::Size(101));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid reversePercent valid -> invalid
				size = InvalidSize.reversePercent(Ash::Size(100));
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// invalid reversePercent invalid -> invalid
				size = InvalidSize.reversePercent(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				// valid reversePercent invalid -> invalid
				size = MaximumSize.reversePercent(InvalidSize);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));

				return {};
			}
		}

		TEST_UNIT
		(
			testSize,
			TEST_CASE(Ash::Test::Size::getValue),
			TEST_CASE(Ash::Test::Size::add),
			TEST_CASE(Ash::Test::Size::subtract),
			TEST_CASE(Ash::Test::Size::multiply),
			TEST_CASE(Ash::Test::Size::divide),
			TEST_CASE(Ash::Test::Size::remainder),
			TEST_CASE(Ash::Test::Size::roundUp),
			TEST_CASE(Ash::Test::Size::roundDown),
			TEST_CASE(Ash::Test::Size::percent),
			TEST_CASE(Ash::Test::Size::reversePercent)
		);
	}
}
