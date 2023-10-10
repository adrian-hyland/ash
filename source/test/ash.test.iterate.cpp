#include <cstdint>
#include "ash.iterate.h"
#include "ash.test.iterate.h"


namespace Ash
{
	namespace Test
	{
		namespace Iterate
		{
			template
			<
				typename TYPE,
				typename = Ash::Type::IsIntegerOrPointer<TYPE>
			>
			static Ash::Test::Assertion iterationNone()
			{
				using Type = TYPE;
				using ForwardIteration = Ash::Iteration<Type>;
				using ReverseIteration = typename ForwardIteration::Reverse;

				ForwardIteration forwardIteration;
				TEST_IS_TRUE(forwardIteration.isForward);
				TEST_IS_FALSE(forwardIteration.isReverse);
				TEST_IS_TRUE(forwardIteration.isComplete());

				ReverseIteration reverseIteration;
				TEST_IS_FALSE(reverseIteration.isForward);
				TEST_IS_TRUE(reverseIteration.isReverse);
				TEST_IS_TRUE(reverseIteration.isComplete());

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterationAdvanceInteger()
			{
				using Type = TYPE;
				using ForwardIteration = Ash::Iteration<Type>;
				using ReverseIteration = typename ForwardIteration::Reverse;

				constexpr Type min = std::numeric_limits<Type>::min();
				constexpr Type max = std::numeric_limits<Type>::max();
				constexpr Type middle = min / 2 + max / 2 + 1;

				ForwardIteration forwardIteration(min, min + 1);
				TEST_IS_FALSE(forwardIteration.isComplete());
				TEST_IS_EQ(forwardIteration, min);
				TEST_IS_TRUE(forwardIteration.advance());
				TEST_IS_EQ(forwardIteration, min + 1);
				TEST_IS_FALSE(forwardIteration.advance());

				forwardIteration = ForwardIteration(middle, middle + 1);
				TEST_IS_FALSE(forwardIteration.isComplete());
				TEST_IS_EQ(forwardIteration, middle);
				TEST_IS_TRUE(forwardIteration.advance());
				TEST_IS_EQ(forwardIteration, middle + 1);
				TEST_IS_FALSE(forwardIteration.advance());

				forwardIteration = ForwardIteration(max - 1, max);
				TEST_IS_FALSE(forwardIteration.isComplete());
				TEST_IS_EQ(forwardIteration, max - 1);
				TEST_IS_TRUE(forwardIteration.advance());
				TEST_IS_EQ(forwardIteration, max);
				TEST_IS_FALSE(forwardIteration.advance());

				ReverseIteration reverseIteration(min + 1, min);
				TEST_IS_FALSE(reverseIteration.isComplete());
				TEST_IS_EQ(reverseIteration, min + 1);
				TEST_IS_TRUE(reverseIteration.advance());
				TEST_IS_EQ(reverseIteration, min);
				TEST_IS_FALSE(reverseIteration.advance());

				reverseIteration = ReverseIteration(middle, middle - 1);
				TEST_IS_FALSE(reverseIteration.isComplete());
				TEST_IS_EQ(reverseIteration, middle);
				TEST_IS_TRUE(reverseIteration.advance());
				TEST_IS_EQ(reverseIteration, middle - 1);
				TEST_IS_FALSE(reverseIteration.advance());

				reverseIteration = ReverseIteration(max, max - 1);
				TEST_IS_FALSE(reverseIteration.isComplete());
				TEST_IS_EQ(reverseIteration, max);
				TEST_IS_TRUE(reverseIteration.advance());
				TEST_IS_EQ(reverseIteration, max - 1);
				TEST_IS_FALSE(reverseIteration.advance());

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterationReverseInteger()
			{
				using Type = TYPE;
				using ForwardIteration = Ash::Iteration<Type>;
				using ReverseIteration = typename ForwardIteration::Reverse;

				constexpr Type min = std::numeric_limits<Type>::min();
				constexpr Type max = std::numeric_limits<Type>::max();

				ForwardIteration forwardIteration(min, max);
				TEST_IS_TRUE(forwardIteration.isForward);
				TEST_IS_FALSE(forwardIteration.isReverse);
				TEST_IS_EQ(forwardIteration, min);

				ReverseIteration reverseIteration = forwardIteration.reverse();
				TEST_IS_FALSE(reverseIteration.isForward);
				TEST_IS_TRUE(reverseIteration.isReverse);
				TEST_IS_EQ(reverseIteration, max);

				forwardIteration = reverseIteration.reverse();
				TEST_IS_TRUE(forwardIteration.isForward);
				TEST_IS_FALSE(forwardIteration.isReverse);
				TEST_IS_EQ(forwardIteration, min);

				reverseIteration = ReverseIteration(max, min);
				TEST_IS_FALSE(reverseIteration.isForward);
				TEST_IS_TRUE(reverseIteration.isReverse);
				TEST_IS_EQ(reverseIteration, max);

				forwardIteration = reverseIteration.reverse();
				TEST_IS_TRUE(forwardIteration.isForward);
				TEST_IS_FALSE(forwardIteration.isReverse);
				TEST_IS_EQ(forwardIteration, min);

				reverseIteration = forwardIteration.reverse();
				TEST_IS_FALSE(reverseIteration.isForward);
				TEST_IS_TRUE(reverseIteration.isReverse);
				TEST_IS_EQ(reverseIteration, max);

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterationFromInteger()
			{
				using Type = TYPE;
				using ForwardIteration = Ash::Iteration<Type>;
				using ReverseIteration = typename ForwardIteration::Reverse;

				constexpr Type min = std::numeric_limits<Type>::min();
				constexpr Type max = std::numeric_limits<Type>::max();

				if constexpr (Ash::Type::isSame<std::make_unsigned_t<Type>, size_t>)
				{
					ForwardIteration forwardIteration = ForwardIteration::from(min, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(forwardIteration.isComplete());
					TEST_IS_EQ(forwardIteration, min);
					TEST_IS_EQ(forwardIteration.reverse(), Type(min + std::numeric_limits<size_t>::max() - 1));

					forwardIteration = ForwardIteration::from(max, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(forwardIteration.isComplete());
					TEST_IS_EQ(forwardIteration, max);
					TEST_IS_EQ(forwardIteration.reverse(), max);

					ReverseIteration reverseIteration = ReverseIteration::from(max, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(reverseIteration.isComplete());
					TEST_IS_EQ(reverseIteration, max);
					TEST_IS_EQ(reverseIteration.reverse(), Type(max - std::numeric_limits<size_t>::max() + 1));

					reverseIteration = ReverseIteration::from(min, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(reverseIteration.isComplete());
					TEST_IS_EQ(reverseIteration, min);
					TEST_IS_EQ(reverseIteration.reverse(), min);
				}
				else
				{
					ForwardIteration forwardIteration = ForwardIteration::from(min, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(forwardIteration.isComplete());
					TEST_IS_EQ(forwardIteration, min);
					TEST_IS_EQ(forwardIteration.reverse(), max);

					forwardIteration = ForwardIteration::from(max, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(forwardIteration.isComplete());
					TEST_IS_EQ(forwardIteration, max);
					TEST_IS_EQ(forwardIteration.reverse(), max);

					ReverseIteration reverseIteration = ReverseIteration::from(max, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(reverseIteration.isComplete());
					TEST_IS_EQ(reverseIteration, max);
					TEST_IS_EQ(reverseIteration.reverse(), min);

					reverseIteration = ReverseIteration::from(min, std::numeric_limits<size_t>::max());
					TEST_IS_FALSE(reverseIteration.isComplete());
					TEST_IS_EQ(reverseIteration, min);
					TEST_IS_EQ(reverseIteration.reverse(), min);
				}

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterationInteger()
			{
				TEST_GENERIC(Ash::Test::Iterate::iterationNone, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterationReverseInteger, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterationAdvanceInteger, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterationFromInteger, TYPE);

				return {};
			}

			static Ash::Test::Assertion iterationAdvancePointer()
			{
				using Type = int *;
				using ForwardIteration = Ash::Iteration<Type>;
				using ReverseIteration = typename ForwardIteration::Reverse;

				int value[2] = { 1, 2 };

				ForwardIteration forwardIteration(&value[0], &value[1]);
				TEST_IS_FALSE(forwardIteration.isComplete());
				TEST_IS_EQ(forwardIteration, &value[0]);
				TEST_IS_TRUE(forwardIteration.advance());
				TEST_IS_FALSE(forwardIteration.isComplete());
				TEST_IS_EQ(forwardIteration, &value[1]);
				TEST_IS_FALSE(forwardIteration.advance());

				ReverseIteration reverseIteration(&value[1], &value[0]);
				TEST_IS_FALSE(reverseIteration.isComplete());
				TEST_IS_EQ(reverseIteration, &value[1]);
				TEST_IS_TRUE(reverseIteration.advance());
				TEST_IS_FALSE(reverseIteration.isComplete());
				TEST_IS_EQ(reverseIteration, &value[0]);
				TEST_IS_FALSE(reverseIteration.advance());

				return {};
			}

			static Ash::Test::Assertion iterationReversePointer()
			{
				using Type = int *;
				using ForwardIteration = Ash::Iteration<Type>;
				using ReverseIteration = typename ForwardIteration::Reverse;

				int value[2] = { 1, 2 };

				ForwardIteration forwardIteration(&value[0], &value[1]);
				TEST_IS_TRUE(forwardIteration.isForward);
				TEST_IS_FALSE(forwardIteration.isReverse);
				TEST_IS_EQ(forwardIteration, &value[0]);

				ReverseIteration reverseIteration = forwardIteration.reverse();
				TEST_IS_FALSE(reverseIteration.isForward);
				TEST_IS_TRUE(reverseIteration.isReverse);
				TEST_IS_EQ(reverseIteration, &value[1]);

				forwardIteration = reverseIteration.reverse();
				TEST_IS_TRUE(forwardIteration.isForward);
				TEST_IS_FALSE(forwardIteration.isReverse);
				TEST_IS_EQ(forwardIteration, &value[0]);

				return {};
			}

			static Ash::Test::Assertion iterationFromPointer()
			{
				using Type = int *;
				using ForwardIteration = Ash::Iteration<Type>;
				using ReverseIteration = typename ForwardIteration::Reverse;

				int value[2] = { 1, 2 };

				ForwardIteration forwardIteration = ForwardIteration::from(&value[0], 2);
				TEST_IS_FALSE(forwardIteration.isComplete());
				TEST_IS_EQ(forwardIteration, &value[0]);
				TEST_IS_EQ(forwardIteration.reverse(), &value[1]);

				ReverseIteration reverseIteration = ReverseIteration::from(&value[1], 2);
				TEST_IS_FALSE(reverseIteration.isComplete());
				TEST_IS_EQ(reverseIteration, &value[1]);
				TEST_IS_EQ(reverseIteration.reverse(), &value[0]);

				return {};
			}

			static Ash::Test::Assertion iterationPointer()
			{
				TEST_GENERIC(Ash::Test::Iterate::iterationNone, int *);

				TEST(Ash::Test::Iterate::iterationReversePointer);

				TEST(Ash::Test::Iterate::iterationAdvancePointer);

				TEST(Ash::Test::Iterate::iterationFromPointer);

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsIntegerOrPointer<TYPE>
			>
			static Ash::Test::Assertion iterateNone()
			{
				using Type = TYPE;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;

				ForwardIterate forwardIterate;

				TEST_IS_EQ(forwardIterate.depth, 1);
				TEST_IS_TRUE(forwardIterate.isForward);
				TEST_IS_FALSE(forwardIterate.isReverse);
				TEST_IS_FALSE(forwardIterate.hasNext);
				TEST_IS_TRUE(forwardIterate.getIteration().isComplete());

				size_t count = 0;
				for (Type n : forwardIterate)
				{
					TEST_IS_ZERO(n);
					count++;
				}
				TEST_IS_EQ(count, 0);

				ReverseIterate reverseIterate;

				TEST_IS_EQ(reverseIterate.depth, 1);
				TEST_IS_TRUE(reverseIterate.isReverse);
				TEST_IS_FALSE(reverseIterate.isForward);
				TEST_IS_FALSE(reverseIterate.hasNext);
				TEST_IS_TRUE(forwardIterate.getIteration().isComplete());

				count = 0;
				for (Type n : reverseIterate)
				{
					TEST_IS_ZERO(n);
					count++;
				}
				TEST_IS_EQ(count, 0);

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterateBetweenInteger()
			{
				using Type = TYPE;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;

				constexpr Type min = std::numeric_limits<Type>::min();
				constexpr Type max = std::numeric_limits<Type>::max();
				constexpr Type middle = min / 2 + max / 2 + 1;

				size_t count = 0;
				for (Type n : ForwardIterate::between(min, min + 1))
				{
					TEST_IS_EQ(Type(n - count), min);
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ForwardIterate::between(middle - 1, middle + 1))
				{
					TEST_IS_EQ(Type(n - count), middle - 1);
					count++;
				}
				TEST_IS_EQ(count, 3);

				count = 0;
				for (Type n : ForwardIterate::between(max - 1, max))
				{
					TEST_IS_EQ(Type(n - count), max - 1);
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ReverseIterate::between(min + 1, min))
				{
					TEST_IS_EQ(Type(n + count), min + 1);
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ReverseIterate::between(middle + 1, middle - 1))
				{
					TEST_IS_EQ(Type(n + count), middle + 1);
					count++;
				}
				TEST_IS_EQ(count, 3);

				count = 0;
				for (Type n : ReverseIterate::between(max, max - 1))
				{
					TEST_IS_EQ(Type(n + count), max);
					count++;
				}
				TEST_IS_EQ(count, 2);

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterateFromInteger()
			{
				using Type = TYPE;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;

				constexpr Type min = std::numeric_limits<Type>::min();
				constexpr Type max = std::numeric_limits<Type>::max();
				constexpr Type middle = min / 2 + max / 2 + 1;
				
				size_t count = 0;
				for (Type n : ForwardIterate::from(min, 2))
				{
					TEST_IS_EQ(n, Type(min + count));
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ForwardIterate::from(middle - 1, 3))
				{
					TEST_IS_EQ(n, Type(middle - 1 + count));
					count++;
				}
				TEST_IS_EQ(count, 3);

				count = 0;
				for (Type n : ForwardIterate::from(max - 1, 2))
				{
					TEST_IS_EQ(n, Type(max - 1 + count));
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ReverseIterate::from(min + 1, 2))
				{
					TEST_IS_EQ(n, Type(min + 1 - count));
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ReverseIterate::from(middle + 1, 3))
				{
					TEST_IS_EQ(n, Type(middle + 1 - count));
					count++;
				}
				TEST_IS_EQ(count, 3);

				count = 0;
				for (Type n : ReverseIterate::from(max, 2))
				{
					TEST_IS_EQ(n, Type(max - count));
					count++;
				}
				TEST_IS_EQ(count, 2);

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterateCombineInteger()
			{
				using Type = TYPE;
				using ForwardIterate = Ash::Iterate<Type>;

				size_t count = 0;
				for (Type n : ForwardIterate::from(1, 1) + ForwardIterate::from(2, 1) + ForwardIterate::from(3, 1) + ForwardIterate::from(4, 1))
				{
					TEST_IS_EQ(n, Type(1 + count));
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (Type n : (ForwardIterate::from(1, 1) + ForwardIterate::from(2, 1) + ForwardIterate::from(3, 1) + ForwardIterate::from(4, 1)).reverse())
				{
					TEST_IS_EQ(n, Type(4 - count));
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (Type n : (ForwardIterate::from(0, 0) + ForwardIterate::from(1, 2)) + (ForwardIterate::from(2, 0) + ForwardIterate::from(3, 2)))
				{
					TEST_IS_EQ(n, Type(1 + count));
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (Type n : ((ForwardIterate::from(0, 0) + ForwardIterate::from(1, 2)) + (ForwardIterate::from(2, 0) + ForwardIterate::from(3, 2))).reverse())
				{
					TEST_IS_EQ(n, Type(4 - count));
					count++;
				}
				TEST_IS_EQ(count, 4);

				return {};
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsInteger<TYPE>
			>
			static Ash::Test::Assertion iterateInteger()
			{
				TEST_GENERIC(Ash::Test::Iterate::iterateNone, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterateBetweenInteger, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterateFromInteger, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterateCombineInteger, TYPE);

				return {};
			}

			static Ash::Test::Assertion iterateBetweenPointer()
			{
				using Type = int *;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;

				int value[2] = { 1, 2 };

				size_t count = 0;
				for (Type n : ForwardIterate::between(&value[0], &value[1]))
				{
					TEST_IS_EQ(n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ReverseIterate::between(&value[1], &value[0]))
				{
					TEST_IS_EQ(n, &value[1 - count]);
					count++;
				}
				TEST_IS_EQ(count, 2);

				return {};
			}

			static Ash::Test::Assertion iterateFromPointer()
			{
				using Type = int *;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;

				int value[2] = { 1, 2 };
				
				size_t count = 0;
				for (Type n : ForwardIterate::from(&value[0], 2))
				{
					TEST_IS_EQ(n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (Type n : ReverseIterate::from(&value[1], 2))
				{
					TEST_IS_EQ(n, &value[1 - count]);
					count++;
				}
				TEST_IS_EQ(count, 2);

				return {};
			}

			static Ash::Test::Assertion iterateCombinePointer()
			{
				using Type = int *;
				using ForwardIterate = Ash::Iterate<Type>;

				int value[4] = { 1, 2, 3, 4};

				size_t count = 0;
				for (Type n : ForwardIterate::from(&value[0], 1) + ForwardIterate::from(&value[1], 1) + ForwardIterate::from(&value[2], 1) + ForwardIterate::from(&value[3], 1))
				{
					TEST_IS_EQ(n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (Type n : (ForwardIterate::from(&value[0], 1) + ForwardIterate::from(&value[1], 1) + ForwardIterate::from(&value[2], 1) + ForwardIterate::from(&value[3], 1)).reverse())
				{
					TEST_IS_EQ(n, &value[3 - count]);
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (Type n : (ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[0], 2)) + (ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[2], 2)))
				{
					TEST_IS_EQ(n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (Type n : ((ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[0], 2)) + (ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[2], 2))).reverse())
				{
					TEST_IS_EQ(n, &value[3 - count]);
					count++;
				}
				TEST_IS_EQ(count, 4);

				return {};
			}

			static Ash::Test::Assertion iteratePointer()
			{
				TEST_GENERIC(Ash::Test::Iterate::iterateNone, int *);

				TEST(Ash::Test::Iterate::iterateBetweenPointer);

				TEST(Ash::Test::Iterate::iterateFromPointer);

				TEST(Ash::Test::Iterate::iterateCombinePointer);

				return {};
			}
		}

		TEST_UNIT
		(
			testIterate,

			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, int8_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, int16_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, int32_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, int64_t),

			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterationInteger, uint64_t),

			TEST_CASE(Ash::Test::Iterate::iterationPointer),

			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int8_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int16_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int32_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int64_t),

			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint64_t),

			TEST_CASE(Ash::Test::Iterate::iteratePointer)
		);
	}
}
