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
			static Ash::Test::Assertion iterateNone()
			{
				using Type = TYPE;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;
				using ReferenceType = typename ForwardIterate::ReferenceType;

				ForwardIterate forwardIterate;

				TEST_IS_EQ(forwardIterate.depth, 1);
				TEST_IS_TRUE(forwardIterate.isForward);
				TEST_IS_FALSE(forwardIterate.isReverse);
				TEST_IS_FALSE(forwardIterate.hasNext);
				TEST_IS_FALSE(forwardIterate != forwardIterate);
				TEST_IS_TRUE(forwardIterate.isComplete());

				size_t count = 0;
				if constexpr (Ash::Type::isInteger<Type>)
				{
					for (Type n : forwardIterate)
					{
						TEST_IS_ZERO(n);
						count++;
					}
				}
				else
				{
					for (ReferenceType n : forwardIterate)
					{
						TEST_IS_ZERO(n);
						count++;
					}
				}
				TEST_IS_TRUE(forwardIterate.isComplete());
				TEST_IS_EQ(count, 0);

				ReverseIterate reverseIterate;

				TEST_IS_EQ(reverseIterate.depth, 1);
				TEST_IS_TRUE(reverseIterate.isReverse);
				TEST_IS_FALSE(reverseIterate.isForward);
				TEST_IS_FALSE(reverseIterate.hasNext);
				TEST_IS_FALSE(reverseIterate != reverseIterate);
				TEST_IS_TRUE(reverseIterate.isComplete());

				count = 0;
				if constexpr (Ash::Type::isInteger<Type>)
				{
					for (Type n : reverseIterate)
					{
						TEST_IS_ZERO(n);
						count++;
					}
				}
				else
				{
					for (ReferenceType n : forwardIterate)
					{
						TEST_IS_ZERO(n);
						count++;
					}
				}
				TEST_IS_TRUE(reverseIterate.isComplete());
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
			static Ash::Test::Assertion iterateLimits()
			{
				using Type = TYPE;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;

				constexpr Type min = std::numeric_limits<Type>::min();
				constexpr Type max = std::numeric_limits<Type>::max();

				size_t count = 0;
				for (Type n : ForwardIterate::from(max, max))
				{
					TEST_IS_EQ(n, Type(max + count));
					count++;
				}
				TEST_IS_EQ(count, 1);

				count = 0;
				for (Type n : ReverseIterate::from(min, max))
				{
					TEST_IS_EQ(n, Type(min - count));
					count++;
				}
				TEST_IS_EQ(count, 1);

				count = 0;
				for (Type n : ForwardIterate::between(max, min))
				{
					TEST_IS_EQ(n, Type(max + count));
					count++;
				}
				TEST_IS_EQ(count, 0);

				count = 0;
				for (Type n : ReverseIterate::between(min, max))
				{
					TEST_IS_EQ(n, Type(min - count));
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
			static Ash::Test::Assertion iterateInteger()
			{
				TEST_GENERIC(Ash::Test::Iterate::iterateNone, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterateBetweenInteger, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterateFromInteger, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterateCombineInteger, TYPE);

				TEST_GENERIC(Ash::Test::Iterate::iterateLimits, TYPE);

				return {};
			}

			static Ash::Test::Assertion iterateBetweenPointer()
			{
				using Type = int *;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReverseIterate = typename ForwardIterate::Reverse;
				using ReferenceType = typename ForwardIterate::ReferenceType;

				int value[2] = { 1, 2 };

				size_t count = 0;
				for (ReferenceType n : ForwardIterate::between(&value[0], &value[1]))
				{
					TEST_IS_EQ(&n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (ReferenceType n : ReverseIterate::between(&value[1], &value[0]))
				{
					TEST_IS_EQ(&n, &value[1 - count]);
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
				using ReferenceType = typename ForwardIterate::ReferenceType;

				int value[2] = { 1, 2 };
				
				size_t count = 0;
				for (ReferenceType n : ForwardIterate::from(&value[0], 2))
				{
					TEST_IS_EQ(&n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 2);

				count = 0;
				for (ReferenceType n : ReverseIterate::from(&value[1], 2))
				{
					TEST_IS_EQ(&n, &value[1 - count]);
					count++;
				}
				TEST_IS_EQ(count, 2);

				return {};
			}

			static Ash::Test::Assertion iterateCombinePointer()
			{
				using Type = int *;
				using ForwardIterate = Ash::Iterate<Type>;
				using ReferenceType = typename ForwardIterate::ReferenceType;

				int value[4] = { 1, 2, 3, 4};

				size_t count = 0;
				for (ReferenceType n : ForwardIterate::from(&value[0], 1) + ForwardIterate::from(&value[1], 1) + ForwardIterate::from(&value[2], 1) + ForwardIterate::from(&value[3], 1))
				{
					TEST_IS_EQ(&n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (ReferenceType n : (ForwardIterate::from(&value[0], 1) + ForwardIterate::from(&value[1], 1) + ForwardIterate::from(&value[2], 1) + ForwardIterate::from(&value[3], 1)).reverse())
				{
					TEST_IS_EQ(&n, &value[3 - count]);
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (ReferenceType n : (ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[0], 2)) + (ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[2], 2)))
				{
					TEST_IS_EQ(&n, &value[count]);
					count++;
				}
				TEST_IS_EQ(count, 4);

				count = 0;
				for (ReferenceType n : ((ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[0], 2)) + (ForwardIterate::from(nullptr, 0) + ForwardIterate::from(&value[2], 2))).reverse())
				{
					TEST_IS_EQ(&n, &value[3 - count]);
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

			static Ash::Test::Assertion isComplete()
			{
				Ash::Iterate<int> forwardIterate;
				TEST_IS_TRUE(forwardIterate.isComplete());

				Ash::Iterate<int, false> reverseIterate;
				TEST_IS_TRUE(reverseIterate.isComplete());

				forwardIterate = Ash::Iterate<int>::from(0, 0);
				TEST_IS_TRUE(forwardIterate.isComplete());

				reverseIterate = Ash::Iterate<int>::from(0, 0).reverse();
				TEST_IS_TRUE(reverseIterate.isComplete());

				forwardIterate = Ash::Iterate<int>::between(0, 0);
				TEST_IS_FALSE(forwardIterate.isComplete());
				++forwardIterate;
				TEST_IS_TRUE(forwardIterate.isComplete());
				++forwardIterate;
				TEST_IS_TRUE(forwardIterate.isComplete());

				reverseIterate = Ash::Iterate<int>::between(0, 0).reverse();
				TEST_IS_FALSE(reverseIterate.isComplete());
				++reverseIterate;
				TEST_IS_TRUE(reverseIterate.isComplete());
				++reverseIterate;
				TEST_IS_TRUE(reverseIterate.isComplete());

				return {};
			}
		}

		TEST_UNIT
		(
			testIterate,

			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int8_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int16_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int32_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, int64_t),

			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint8_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint16_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint32_t),
			TEST_CASE_GENERIC(Ash::Test::Iterate::iterateInteger, uint64_t),

			TEST_CASE(Ash::Test::Iterate::iteratePointer),

			TEST_CASE(Ash::Test::Iterate::isComplete)
		);
	}
}
