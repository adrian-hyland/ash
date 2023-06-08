#include "ash.test.h"
#include "ash.size.h"


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
				size_t sizeValue;

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
				size_t sizeValue;

				for (size_t value = 0; value <= SIZE_MAX / 2; value++)
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
				size_t sizeValue;

				for (size_t value = 0; value < SIZE_MAX; value++)
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
				size_t sizeValue;

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
				size_t sizeValue;

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
				size_t sizeValue;

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
				size_t sizeValue;

				// valid roundup constant -> valid
				size = Ash::Size(33).roundUp(32);
				TEST_IS_TRUE(size.isValid());
				TEST_IS_TRUE(size.getValue(sizeValue));
				TEST_IS_EQ(sizeValue, 64);

				// valid roundup constant -> invalid
				size = MaximumSize.roundUp(0);
				TEST_IS_FALSE(size.isValid());
				TEST_IS_FALSE(size.getValue(sizeValue));
				size = MaximumSize.roundUp(1);
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
				size = MaximumSize.roundUp(Ash::Size(1));
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

			static const Ash::Test::Unit test TEST_UNIT
			(
				TEST_CASE(Ash::Test::Size::getValue),
				TEST_CASE(Ash::Test::Size::add),
				TEST_CASE(Ash::Test::Size::subtract),
				TEST_CASE(Ash::Test::Size::multiply),
				TEST_CASE(Ash::Test::Size::divide),
				TEST_CASE(Ash::Test::Size::remainder),
				TEST_CASE(Ash::Test::Size::roundUp)
			);
		}
	}
}
