#include "ash.nullable.h"
#include "ash.test.memory.trace.h"
#include "ash.test.nullable.h"


namespace Ash
{
	namespace Test
	{
		namespace Nullable
		{
			static Ash::Test::Assertion construct()
			{
				Ash::Nullable<int> int1;
				Ash::Nullable<int> int2(int1);
				TEST_IS_NULL(int1);
				TEST_IS_NULL(int2);

				int1 = 123;
				Ash::Nullable<int> int3(int1);
				TEST_IS_NOT_NULL(int1);
				TEST_IS_NOT_NULL(int3);
				TEST_IS_EQ(*static_cast<const int *>(int1), 123);
				TEST_IS_EQ(*static_cast<const int *>(int3), 123);

				Ash::Nullable<int> int4(std::move(int2));
				TEST_IS_NULL(int2);
				TEST_IS_NULL(int4);

				Ash::Nullable<int> int5(std::move(int3));
				TEST_IS_NULL(int3);
				TEST_IS_NOT_NULL(int5);
				TEST_IS_EQ(*static_cast<const int *>(int5), 123);

				return {};
			}

			static Ash::Test::Assertion assign()
			{
				Ash::Nullable<int> int1;
				int1 = 123;
				TEST_IS_NOT_NULL(int1);
				TEST_IS_EQ(*static_cast<const int *>(int1), 123);

				Ash::Nullable<int> int2;
				int2 = int1;
				TEST_IS_NOT_NULL(int2);
				TEST_IS_EQ(*static_cast<const int *>(int2), 123);

				int1.clear();
				int2 = int1;
				TEST_IS_NULL(int1);
				TEST_IS_NULL(int2);

				int1 = 456;
				int2 = std::move(int1);
				TEST_IS_NULL(int1);
				TEST_IS_NOT_NULL(int2);
				TEST_IS_EQ(*static_cast<const int *>(int2), 456);

				int1.clear();
				int2 = std::move(int1);
				TEST_IS_NULL(int1);
				TEST_IS_NULL(int2);

				return {};
			}

			static Ash::Test::Assertion isNull()
			{
				Ash::Nullable<int> intValue;
				TEST_IS_TRUE(intValue.isNull());

				intValue = 123;
				TEST_IS_FALSE(intValue.isNull());

				intValue.clear();
				TEST_IS_TRUE(intValue.isNull());

				return {};
			}

			static Ash::Test::Assertion getAt()
			{
				Ash::Nullable<int> intValue;
				TEST_IS_NULL(intValue.getAt());

				intValue = 123;
				TEST_IS_NOT_NULL(intValue.getAt());
				TEST_IS_EQ(*intValue.getAt(), 123);

				intValue.clear();
				TEST_IS_NULL(intValue.getAt());

				return {};
			}

			static Ash::Test::Assertion getOr()
			{
				Ash::Nullable<int> intValue;
				TEST_IS_EQ(intValue.getOr(123), 123);

				intValue = 456;
				TEST_IS_EQ(intValue.getOr(123), 456);

				intValue.clear();
				TEST_IS_EQ(intValue.getOr(123), 123);

				return {};
			}

			static Ash::Test::Assertion setAt()
			{
				Ash::Nullable<int> intValue;
				TEST_IS_TRUE(intValue.isNull());

				*intValue.setAt() = 123;
				TEST_IS_FALSE(intValue.isNull());
				TEST_IS_EQ(*intValue.getAt(), 123);

				return {};
			}

			static Ash::Test::Assertion clear()
			{
				Ash::Nullable<int> intValue;
				TEST_IS_TRUE(intValue.isNull());

				intValue = 123;
				TEST_IS_FALSE(intValue.isNull());

				intValue.clear();
				TEST_IS_TRUE(intValue.isNull());

				intValue.setAt();
				TEST_IS_FALSE(intValue.isNull());
				TEST_IS_EQ(*intValue.getAt(), 0);

				return {};
			}

			Ash::Test::Assertion array()
			{
				Ash::Nullable<int [5]> array1;
				TEST_IS_TRUE(array1.isNull());

				const int arrayValue[5] = { 5, 4, 3, 2, 1 };
				array1 = arrayValue;
				TEST_IS_ZERO(memcmp(array1.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));

				array1.clear();
				TEST_IS_TRUE(array1.isNull());

				array1 = { 5, 4, 3, 2, 1 };
				TEST_IS_ZERO(memcmp(array1.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));

				Ash::Nullable<int [5]> array2(std::move(array1));
				TEST_IS_TRUE(array1.isNull());
				TEST_IS_FALSE(array2.isNull());
				TEST_IS_ZERO(memcmp(array2.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));

				Ash::Nullable<int [5]> array3(array2);
				TEST_IS_FALSE(array2.isNull());
				TEST_IS_ZERO(memcmp(array2.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));
				TEST_IS_FALSE(array3.isNull());
				TEST_IS_ZERO(memcmp(array3.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));

				Ash::Nullable<int [5]> array4;
				TEST_IS_TRUE(array4.isNull());

				array4 = std::move(array3);
				TEST_IS_TRUE(array3.isNull());
				TEST_IS_FALSE(array4.isNull());
				TEST_IS_ZERO(memcmp(array4.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));

				array3 = array4;
				TEST_IS_FALSE(array3.isNull());
				TEST_IS_ZERO(memcmp(array3.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));
				TEST_IS_FALSE(array4.isNull());
				TEST_IS_ZERO(memcmp(array4.getOr({ 1, 2, 3, 4, 5 }), arrayValue, 5));

				const int zeroValue[5] = { 0, 0, 0, 0, 0 };
				array3 = std::move(array1);
				TEST_IS_TRUE(array3.isNull());
				TEST_IS_ZERO(memcmp(*array3.setAt(), zeroValue, 5));
				TEST_IS_ZERO(memcmp(*array3.setAt(), zeroValue, 5));

				*array4.setAt()[0] = 100;
				TEST_IS_EQ(*array4.getAt()[0], 100);

				return {};
			}

			Ash::Test::Assertion object()
			{
				struct Point { int x; int y; };

				Ash::Nullable<Point> point1;
				TEST_IS_TRUE(point1.isNull());

				const Point pointValue = { 1, 2 };
				point1 = pointValue;
				TEST_IS_EQ(point1.getAt()->x, 1);
				TEST_IS_EQ(point1.getAt()->y, 2);

				point1.clear();
				TEST_IS_TRUE(point1.isNull());

				point1 = { 1, 2 };
				TEST_IS_EQ(point1.getAt()->x, 1);
				TEST_IS_EQ(point1.getAt()->y, 2);

				Ash::Nullable<Point> point2(std::move(point1));
				TEST_IS_TRUE(point1.isNull());
				TEST_IS_FALSE(point2.isNull());
				TEST_IS_EQ(point2.getAt()->x, 1);
				TEST_IS_EQ(point2.getAt()->y, 2);

				Ash::Nullable<Point> point3(point2);
				TEST_IS_FALSE(point2.isNull());
				TEST_IS_EQ(point2.getAt()->x, 1);
				TEST_IS_EQ(point2.getAt()->y, 2);
				TEST_IS_FALSE(point3.isNull());
				TEST_IS_EQ(point3.getAt()->x, 1);
				TEST_IS_EQ(point3.getAt()->y, 2);

				Ash::Nullable<Point> point4;
				TEST_IS_TRUE(point4.isNull());

				point4 = std::move(point3);
				TEST_IS_TRUE(point3.isNull());
				TEST_IS_FALSE(point4.isNull());
				TEST_IS_EQ(point4.getAt()->x, 1);
				TEST_IS_EQ(point4.getAt()->y, 2);

				point3 = point4;
				TEST_IS_FALSE(point3.isNull());
				TEST_IS_EQ(point3.getAt()->x, 1);
				TEST_IS_EQ(point3.getAt()->y, 2);
				TEST_IS_FALSE(point4.isNull());
				TEST_IS_EQ(point4.getAt()->x, 1);
				TEST_IS_EQ(point4.getAt()->y, 2);

				point3 = std::move(point1);
				TEST_IS_TRUE(point3.isNull());
				TEST_IS_EQ(point3.setAt()->x, 0);
				TEST_IS_EQ(point3.setAt()->y, 0);

				point4.setAt()->x = 100;
				TEST_IS_EQ(point4.getAt()->x, 100);

				return {};
			}

			Ash::Test::Assertion trace()
			{
				Ash::Nullable<Ash::Test::Memory::TraceValue<int>> trace1;
				TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

				trace1 = 99;
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

				trace1.clear();
				TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

				trace1 = 1;
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

				Ash::Nullable<Ash::Test::Memory::TraceValue<int>> trace2(std::move(trace1));
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

				Ash::Nullable<Ash::Test::Memory::TraceValue<int>> trace3(trace2);
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

				Ash::Nullable<Ash::Test::Memory::TraceValue<int>> trace4;

				trace4 = std::move(trace3);
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

				trace3 = trace4;
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3);

				trace2 = std::move(trace1);
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

				trace3 = trace1;
				TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

				return {};
			}
		}

		TEST_UNIT
		(
			testNullable,
			TEST_CASE(Ash::Test::Nullable::construct),
			TEST_CASE(Ash::Test::Nullable::assign),
			TEST_CASE(Ash::Test::Nullable::isNull),
			TEST_CASE(Ash::Test::Nullable::getAt),
			TEST_CASE(Ash::Test::Nullable::getOr),
			TEST_CASE(Ash::Test::Nullable::setAt),
			TEST_CASE(Ash::Test::Nullable::clear),
			TEST_CASE(Ash::Test::Nullable::array),
			TEST_CASE(Ash::Test::Nullable::object),
			TEST_CASE(Ash::Test::Nullable::trace)
		);
	}
}
