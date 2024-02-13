#include "ash.memory.unique.h"
#include "ash.real.h"
#include "ash.test.memory.trace.h"
#include "ash.test.memory.unique.h"


namespace Ash
{
	namespace Test
	{
		namespace Memory
		{
			namespace Unique
			{
				class Shape
				{
				public:
					virtual Ash::Double getPerimeter() const = 0;

					virtual Ash::Double getArea() const = 0;
				};

				class Circle : public Shape
				{
				public:
					constexpr Circle(Ash::Double radius) : m_Radius(radius) {}

					static Circle withPerimeter(Ash::Double perimeter) { return Circle(perimeter / (2 * Ash::Double::pi)); }

					constexpr Ash::Double getPerimeter() const override { return *m_Radius * 2 * Ash::Double::pi; }

					constexpr Ash::Double getArea() const override { return Ash::Double::pi * m_Radius->square(); }

				private:
					TraceValue<Ash::Double> m_Radius;
				};

				class Rectangle : public Shape
				{
				public:
					constexpr Rectangle(Ash::Double width, Ash::Double height) : m_Width(width), m_Height(height) {}

					constexpr Ash::Double getPerimeter() const override { return 2 * (*m_Width + *m_Height); }

					constexpr Ash::Double getArea() const override { return *m_Width * *m_Height; }

				private:
					TraceValue<Ash::Double> m_Width;
					TraceValue<Ash::Double> m_Height;
				};

				class Square : public Rectangle
				{
				public:
					constexpr Square(Ash::Double width) : Rectangle(width, width) {}
				};

				static Ash::Double getCompactnessRatio(const Shape &shape)
				{
					Circle circle = Circle::withPerimeter(shape.getPerimeter());
					return shape.getArea() / circle.getArea();
				}

				class Point
				{
				public:
					constexpr Point() : m_X(0), m_Y(0) {}

					constexpr Point(Ash::Double x, Ash::Double y) : m_X(x), m_Y(y) {}

					constexpr const Ash::Double getX() const { return *m_X; }

					constexpr const Ash::Double getY() const { return *m_Y; }

					constexpr void getX(Ash::Double x) { *m_X = x; }

					constexpr void getY(Ash::Double y) { *m_Y = y; }

					constexpr Ash::Double distance(const Point &point)
					{
						return Ash::Double(Ash::Double(*m_X - *point.m_X).square() + Ash::Double(*m_Y - *point.m_Y).square()).squareRoot();
					}

				private:
					TraceValue<Ash::Double> m_X;
					TraceValue<Ash::Double> m_Y;
				};

				static Ash::Test::Assertion primitiveReference()
				{
					Ash::Memory::Unique::Pointer<int> n1;
					TEST_IS_TRUE(n1.isNull());
					TEST_IS_FALSE(n1.isAllocated());

					int value = 123;
					n1 = Ash::Memory::Unique::at(value);
					TEST_IS_FALSE(n1.isNull());
					TEST_IS_FALSE(n1.isAllocated());
					TEST_IS_EQ(*n1, 123);
					TEST_IS_EQ(static_cast<int *>(n1), &value);

					*n1 = 456;
					TEST_IS_EQ(value, 456);

					n1.release();
					TEST_IS_TRUE(n1.isNull());
					TEST_IS_FALSE(n1.isAllocated());

					Ash::Memory::Unique::Pointer<int> n2 = Ash::Memory::Unique::at(value);
					TEST_IS_FALSE(n2.isNull());
					TEST_IS_FALSE(n2.isAllocated());
					Ash::Memory::Unique::Pointer<int> n3(std::move(n2));
					TEST_IS_TRUE(n2.isNull());
					TEST_IS_FALSE(n2.isAllocated());
					TEST_IS_FALSE(n3.isNull());
					TEST_IS_FALSE(n3.isAllocated());

					n2 = std::move(n3);
					TEST_IS_FALSE(n2.isNull());
					TEST_IS_FALSE(n2.isAllocated());
					TEST_IS_TRUE(n3.isNull());
					TEST_IS_FALSE(n3.isAllocated());

					return {};
				}

				static Ash::Test::Assertion primitiveAllocation()
				{
					Ash::Memory::Unique::Pointer<TraceValue<int>> n1;
					TEST_IS_TRUE(n1.isNull());
					TEST_IS_FALSE(n1.isAllocated());

					n1 = Ash::Memory::Unique::newValue(TraceValue<int>(123));
					TEST_IS_FALSE(n1.isNull());
					TEST_IS_TRUE(n1.isAllocated());
					TEST_IS_EQ(**n1, 123);

					n1.release();
					TEST_IS_TRUE(n1.isNull());
					TEST_IS_FALSE(n1.isAllocated());

					Ash::Memory::Unique::Pointer<TraceValue<int>> n2 = Ash::Memory::Unique::newValue(TraceValue<int>(321));
					TEST_IS_FALSE(n2.isNull());
					TEST_IS_TRUE(n2.isAllocated());
					Ash::Memory::Unique::Pointer<TraceValue<int>> n3(std::move(n2));
					TEST_IS_TRUE(n2.isNull());
					TEST_IS_FALSE(n2.isAllocated());
					TEST_IS_FALSE(n3.isNull());
					TEST_IS_TRUE(n3.isAllocated());

					n2 = std::move(n3);
					TEST_IS_FALSE(n2.isNull());
					TEST_IS_TRUE(n2.isAllocated());
					TEST_IS_TRUE(n3.isNull());
					TEST_IS_FALSE(n3.isAllocated());

					n2.release();
					TEST_IS_EQ(Trace::getAllocatedCount(), 0);

					return {};
				}

				static Ash::Test::Assertion objectReference()
				{
					Ash::Memory::Unique::Pointer<Point> p1;
					TEST_IS_TRUE(p1.isNull());
					TEST_IS_FALSE(p1.isAllocated());

					Point point1(3, 4);
					p1 = Ash::Memory::Unique::at(point1);
					TEST_IS_FALSE(p1.isNull());
					TEST_IS_FALSE(p1.isAllocated());
					TEST_IS_TRUE(p1->distance(Point(0, 0)).isEqual(5));
					TEST_IS_EQ(static_cast<Point *>(p1), &point1);

					p1.release();
					TEST_IS_TRUE(p1.isNull());
					TEST_IS_FALSE(p1.isAllocated());

					Ash::Memory::Unique::Pointer<Point> p2 = Ash::Memory::Unique::at(point1);
					TEST_IS_FALSE(p2.isNull());
					TEST_IS_FALSE(p2.isAllocated());
					Ash::Memory::Unique::Pointer<Point> p3(std::move(p2));
					TEST_IS_TRUE(p2.isNull());
					TEST_IS_FALSE(p2.isAllocated());
					TEST_IS_FALSE(p3.isNull());
					TEST_IS_FALSE(p3.isAllocated());

					p2 = std::move(p3);
					TEST_IS_FALSE(p2.isNull());
					TEST_IS_FALSE(p2.isAllocated());
					TEST_IS_TRUE(p3.isNull());
					TEST_IS_FALSE(p3.isAllocated());

					Point point2(6, 8);
					p2 = Ash::Memory::Unique::at(point2);
					TEST_IS_FALSE(p2.isNull());
					TEST_IS_FALSE(p2.isAllocated());
					TEST_IS_TRUE(p2->distance(Point(0, 0)).isEqual(10));
					TEST_IS_EQ(static_cast<Point *>(p2), &point2);

					return {};
				}

				static Ash::Test::Assertion objectAllocation()
				{
					Ash::Memory::Unique::Pointer<Point> p1;
					TEST_IS_TRUE(p1.isNull());
					TEST_IS_FALSE(p1.isAllocated());

					p1 = Ash::Memory::Unique::newValue(Point(3, 4));
					TEST_IS_FALSE(p1.isNull());
					TEST_IS_TRUE(p1.isAllocated());
					TEST_IS_TRUE(p1->distance(Point(0, 0)).isEqual(5));

					p1.release();
					TEST_IS_TRUE(p1.isNull());
					TEST_IS_FALSE(p1.isAllocated());

					Ash::Memory::Unique::Pointer<Point> p2 = Ash::Memory::Unique::newValue(Point(6, 8));
					TEST_IS_FALSE(p2.isNull());
					TEST_IS_TRUE(p2.isAllocated());
					Ash::Memory::Unique::Pointer<Point> p3(std::move(p2));
					TEST_IS_TRUE(p2.isNull());
					TEST_IS_FALSE(p2.isAllocated());
					TEST_IS_FALSE(p3.isNull());
					TEST_IS_TRUE(p3.isAllocated());

					p2 = std::move(p3);
					TEST_IS_FALSE(p2.isNull());
					TEST_IS_TRUE(p2.isAllocated());
					TEST_IS_TRUE(p3.isNull());
					TEST_IS_FALSE(p3.isAllocated());

					p2 = Ash::Memory::Unique::newValue(Point(6, 8));
					TEST_IS_FALSE(p2.isNull());
					TEST_IS_TRUE(p2.isAllocated());
					TEST_IS_TRUE(p2->distance(Point(0, 0)).isEqual(10));

					p2.release();
					TEST_IS_EQ(Trace::getAllocatedCount(), 0);

					return {};
				}

				static Ash::Test::Assertion derivedReference()
				{
					Ash::Memory::Unique::Pointer<Shape> s1;
					TEST_IS_TRUE(s1.isNull());
					TEST_IS_FALSE(s1.isAllocated());

					Circle circle(10);
					s1 = Ash::Memory::Unique::at(circle);
					TEST_IS_FALSE(s1.isNull());
					TEST_IS_FALSE(s1.isAllocated());
					TEST_IS_TRUE(getCompactnessRatio(*s1).isEqual(1));
					TEST_IS_EQ(static_cast<Shape *>(s1), &circle);

					s1.release();
					TEST_IS_TRUE(s1.isNull());
					TEST_IS_FALSE(s1.isAllocated());

					Ash::Memory::Unique::Pointer<Shape> s2 = Ash::Memory::Unique::at(circle);
					TEST_IS_FALSE(s2.isNull());
					TEST_IS_FALSE(s2.isAllocated());
					Ash::Memory::Unique::Pointer<Shape> s3(std::move(s2));
					TEST_IS_TRUE(s2.isNull());
					TEST_IS_FALSE(s2.isAllocated());
					TEST_IS_FALSE(s3.isNull());
					TEST_IS_FALSE(s3.isAllocated());

					s2 = std::move(s3);
					TEST_IS_FALSE(s2.isNull());
					TEST_IS_FALSE(s2.isAllocated());
					TEST_IS_TRUE(s3.isNull());
					TEST_IS_FALSE(s3.isAllocated());

					Square square(10);
					s2 = Ash::Memory::Unique::at(square);
					TEST_IS_FALSE(s2.isNull());
					TEST_IS_FALSE(s2.isAllocated());
					TEST_IS_TRUE(getCompactnessRatio(*s2).isEqual(Ash::Double::pi / 4));
					TEST_IS_EQ(static_cast<Shape *>(s2), &square);

					return {};
				}

				static Ash::Test::Assertion derivedAllocation()
				{
					Ash::Memory::Unique::Pointer<Shape> s1;
					TEST_IS_TRUE(s1.isNull());
					TEST_IS_FALSE(s1.isAllocated());

					s1 = Ash::Memory::Unique::newValue(Circle(10));
					TEST_IS_FALSE(s1.isNull());
					TEST_IS_TRUE(s1.isAllocated());
					TEST_IS_TRUE(getCompactnessRatio(*s1).isEqual(1));

					s1.release();
					TEST_IS_TRUE(s1.isNull());
					TEST_IS_FALSE(s1.isAllocated());

					Ash::Memory::Unique::Pointer<Shape> s2 = Ash::Memory::Unique::newValue(Circle(10));
					TEST_IS_FALSE(s2.isNull());
					TEST_IS_TRUE(s2.isAllocated());
					Ash::Memory::Unique::Pointer<Shape> s3(std::move(s2));
					TEST_IS_TRUE(s2.isNull());
					TEST_IS_FALSE(s2.isAllocated());
					TEST_IS_FALSE(s3.isNull());
					TEST_IS_TRUE(s3.isAllocated());

					s2 = std::move(s3);
					TEST_IS_FALSE(s2.isNull());
					TEST_IS_TRUE(s2.isAllocated());
					TEST_IS_TRUE(s3.isNull());
					TEST_IS_FALSE(s3.isAllocated());

					s2 = Ash::Memory::Unique::newValue(Square(10));
					TEST_IS_FALSE(s2.isNull());
					TEST_IS_TRUE(s2.isAllocated());
					TEST_IS_TRUE(getCompactnessRatio(*s2).isEqual(Ash::Double::pi / 4));

					s2.release();
					TEST_IS_EQ(Trace::getAllocatedCount(), 0);

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testMemoryUnique,

			TEST_CASE(Ash::Test::Memory::Unique::primitiveReference),
			TEST_CASE(Ash::Test::Memory::Unique::primitiveAllocation),
			TEST_CASE(Ash::Test::Memory::Unique::objectReference),
			TEST_CASE(Ash::Test::Memory::Unique::objectAllocation),
			TEST_CASE(Ash::Test::Memory::Unique::derivedReference),
			TEST_CASE(Ash::Test::Memory::Unique::derivedAllocation),
		);
	}
}
