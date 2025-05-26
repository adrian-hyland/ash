#include "ash.pointer.h"
#include "ash.real.h"
#include "ash.concurrency.h"
#include "ash.test.memory.trace.h"
#include "ash.test.pointer.h"


namespace Ash
{
	namespace Test
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
			Ash::Test::Memory::TraceValue<Ash::Double> m_Radius;
		};

		class Rectangle : public Shape
		{
		public:
			constexpr Rectangle(Ash::Double width, Ash::Double height) : m_Width(width), m_Height(height) {}

			constexpr Ash::Double getPerimeter() const override { return 2 * (*m_Width + *m_Height); }

			constexpr Ash::Double getArea() const override { return *m_Width * *m_Height; }

		private:
			Ash::Test::Memory::TraceValue<Ash::Double> m_Width;
			Ash::Test::Memory::TraceValue<Ash::Double> m_Height;
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
			Ash::Test::Memory::TraceValue<Ash::Double> m_X;
			Ash::Test::Memory::TraceValue<Ash::Double> m_Y;
		};

		namespace Unique
		{
			namespace Pointer
			{
				static Ash::Test::Assertion primitive()
				{
					Ash::Unique::Pointer<int> n1;
					TEST_IS_EQ(n1, nullptr);
					TEST_IS_EQ(n1.getReferenceCount(), 0);

					n1 = Ash::Unique::value(123);
					TEST_IS_NOT_NULL(n1);
					TEST_IS_EQ(n1.getReferenceCount(), 1);
					TEST_IS_EQ(*n1, 123);

					n1.release();
					TEST_IS_NULL(n1);
					TEST_IS_EQ(n1.getReferenceCount(), 0);

					Ash::Unique::Pointer<int> n2 = Ash::Unique::value(321);
					TEST_IS_NOT_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 1);
					Ash::Unique::Pointer<int> n3(std::move(n2));
					TEST_IS_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(n3);
					TEST_IS_EQ(n3.getReferenceCount(), 1);

					n2 = std::move(n3);
					TEST_IS_NOT_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 1);
					TEST_IS_NULL(n3);
					TEST_IS_EQ(n3.getReferenceCount(), 0);
					TEST_IS_EQ(*n2, 321);

					n2.release();
					TEST_IS_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 0);

					return {};
				}

				static Ash::Test::Assertion array()
				{
					Ash::Unique::Pointer<int[4]> a1;
					TEST_IS_EQ(a1, nullptr);
					TEST_IS_EQ(a1.getReferenceCount(), 0);

					a1 = Ash::Unique::value<int[4]>(1, 2, 3, 4);
					TEST_IS_NOT_NULL(a1);
					TEST_IS_EQ(a1.getReferenceCount(), 1);
					TEST_IS_EQ((*a1)[0], 1);
					TEST_IS_EQ((*a1)[1], 2);
					TEST_IS_EQ((*a1)[2], 3);
					TEST_IS_EQ((*a1)[3], 4);

					a1.release();
					TEST_IS_NULL(a1);
					TEST_IS_EQ(a1.getReferenceCount(), 0);

					Ash::Unique::Pointer<int[4]> a2 = Ash::Unique::value<int[4]>(4, 3, 2, 1);
					TEST_IS_NOT_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 1);
					Ash::Unique::Pointer<int[4]> a3(std::move(a2));
					TEST_IS_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(a3);
					TEST_IS_EQ(a3.getReferenceCount(), 1);

					a2 = std::move(a3);
					TEST_IS_NOT_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 1);
					TEST_IS_NULL(a3);
					TEST_IS_EQ(a3.getReferenceCount(), 0);
					TEST_IS_EQ((*a2)[0], 4);
					TEST_IS_EQ((*a2)[1], 3);
					TEST_IS_EQ((*a2)[2], 2);
					TEST_IS_EQ((*a2)[3], 1);

					a2.release();
					TEST_IS_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 0);

					return {};
				}

				static Ash::Test::Assertion object()
				{
					Ash::Unique::Pointer<Point> p1;
					TEST_IS_NULL(p1);
					TEST_IS_EQ(p1.getReferenceCount(), 0);

					p1 = Ash::Unique::value(Point(3, 4));
					TEST_IS_NOT_NULL(p1);
					TEST_IS_EQ(p1.getReferenceCount(), 1);
					TEST_IS_TRUE(p1->distance(Point(0, 0)).isEqual(5));

					p1.release();
					TEST_IS_NULL(p1);
					TEST_IS_EQ(p1.getReferenceCount(), 0);

					Ash::Unique::Pointer<Point> p2 = Ash::Unique::value(Point(6, 8));
					TEST_IS_NOT_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 1);
					Ash::Unique::Pointer<Point> p3(std::move(p2));
					TEST_IS_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(p3);
					TEST_IS_EQ(p3.getReferenceCount(), 1);

					p2 = std::move(p3);
					TEST_IS_NOT_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 1);
					TEST_IS_NULL(p3);
					TEST_IS_EQ(p3.getReferenceCount(), 0);

					p2 = Ash::Unique::value(Point(6, 8));
					TEST_IS_NOT_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 1);
					TEST_IS_TRUE(p2->distance(Point(0, 0)).isEqual(10));

					p2.release();
					TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 0);

					return {};
				}

				static Ash::Test::Assertion derived()
				{
					Ash::Unique::Pointer<Shape> s1;
					TEST_IS_NULL(s1);
					TEST_IS_EQ(s1.getReferenceCount(), 0);

					s1 = Ash::Unique::value(Circle(10));
					TEST_IS_NOT_NULL(s1);
					TEST_IS_EQ(s1.getReferenceCount(), 1);
					TEST_IS_TRUE(getCompactnessRatio(*s1).isEqual(1));

					s1.release();
					TEST_IS_NULL(s1);
					TEST_IS_EQ(s1.getReferenceCount(), 0);

					Ash::Unique::Pointer<Shape> s2 = Ash::Unique::value(Circle(20));
					TEST_IS_NOT_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 1);
					Ash::Unique::Pointer<Shape> s3(std::move(s2));
					TEST_IS_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(s3);
					TEST_IS_EQ(s3.getReferenceCount(), 1);

					s2 = std::move(s3);
					TEST_IS_NOT_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 1);
					TEST_IS_NULL(s3);
					TEST_IS_EQ(s3.getReferenceCount(), 0);

					s2 = Ash::Unique::value(Square(10));
					TEST_IS_NOT_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 1);
					TEST_IS_TRUE(getCompactnessRatio(*s2).isEqual(Ash::Double::pi / 4));

					s2.release();
					TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 0);

					return {};
				}
			}
		}

		namespace Shared
		{
			namespace Pointer
			{
				static Ash::Test::Assertion primitive()
				{
					Ash::Shared::Pointer<int> n1;
					TEST_IS_EQ(n1, nullptr);
					TEST_IS_EQ(n1.getReferenceCount(), 0);

					n1 = Ash::Shared::value(123);
					TEST_IS_NOT_NULL(n1);
					TEST_IS_EQ(n1.getReferenceCount(), 1);
					TEST_IS_EQ(*n1, 123);

					n1.release();
					TEST_IS_NULL(n1);
					TEST_IS_EQ(n1.getReferenceCount(), 0);

					Ash::Shared::Pointer<int> n2 = Ash::Shared::value(321);
					TEST_IS_NOT_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 1);
					Ash::Shared::Pointer<int> n3(std::move(n2));
					TEST_IS_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(n3);
					TEST_IS_EQ(n3.getReferenceCount(), 1);

					n2 = std::move(n3);
					TEST_IS_NOT_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 1);
					TEST_IS_NULL(n3);
					TEST_IS_EQ(n3.getReferenceCount(), 0);
					TEST_IS_EQ(*n2, 321);

					n2.release();
					TEST_IS_NULL(n2);
					TEST_IS_EQ(n2.getReferenceCount(), 0);

					Ash::Shared::Pointer<int> n4 = Ash::Shared::value(789);
					TEST_IS_NOT_NULL(n4);
					TEST_IS_EQ(n4.getReferenceCount(), 1);
					Ash::Shared::Pointer<int> n5(n4);
					TEST_IS_NOT_NULL(n4);
					TEST_IS_EQ(n4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(n5);
					TEST_IS_EQ(n5.getReferenceCount(), 2);
					TEST_IS_EQ(n4, n5);

					n4 = n5;
					TEST_IS_NOT_NULL(n4);
					TEST_IS_EQ(n4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(n5);
					TEST_IS_EQ(n5.getReferenceCount(), 2);
					TEST_IS_EQ(n4, n5);

					Ash::Shared::Pointer<int> n6 = std::move(n4);
					TEST_IS_NULL(n4);
					TEST_IS_EQ(n4.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(n5);
					TEST_IS_EQ(n5.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(n6);
					TEST_IS_EQ(n6.getReferenceCount(), 2);
					TEST_IS_EQ(n5, n6);

					n5 = Ash::Shared::value(987);
					TEST_IS_NOT_NULL(n5);
					TEST_IS_EQ(n5.getReferenceCount(), 1);
					TEST_IS_NOT_NULL(n6);
					TEST_IS_EQ(n6.getReferenceCount(), 1);
					TEST_IS_NOT_EQ(n5, n6);
					TEST_IS_EQ(*n5, 987);
					TEST_IS_EQ(*n6, 789);

					return {};
				}

				static Ash::Test::Assertion array()
				{
					Ash::Shared::Pointer<int[4]> a1;
					TEST_IS_EQ(a1, nullptr);
					TEST_IS_EQ(a1.getReferenceCount(), 0);

					a1 = Ash::Shared::value<int[4]>(1, 2, 3, 4);
					TEST_IS_NOT_NULL(a1);
					TEST_IS_EQ(a1.getReferenceCount(), 1);
					TEST_IS_EQ((*a1)[0], 1);
					TEST_IS_EQ((*a1)[1], 2);
					TEST_IS_EQ((*a1)[2], 3);
					TEST_IS_EQ((*a1)[3], 4);

					a1.release();
					TEST_IS_NULL(a1);
					TEST_IS_EQ(a1.getReferenceCount(), 0);

					Ash::Shared::Pointer<int[4]> a2 = Ash::Shared::value<int[4]>(4, 3, 2, 1);
					TEST_IS_NOT_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 1);
					Ash::Shared::Pointer<int[4]> a3(std::move(a2));
					TEST_IS_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(a3);
					TEST_IS_EQ(a3.getReferenceCount(), 1);

					a2 = std::move(a3);
					TEST_IS_NOT_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 1);
					TEST_IS_NULL(a3);
					TEST_IS_EQ(a3.getReferenceCount(), 0);
					TEST_IS_EQ((*a2)[0], 4);
					TEST_IS_EQ((*a2)[1], 3);
					TEST_IS_EQ((*a2)[2], 2);
					TEST_IS_EQ((*a2)[3], 1);

					a2.release();
					TEST_IS_NULL(a2);
					TEST_IS_EQ(a2.getReferenceCount(), 0);

					Ash::Shared::Pointer<int[4]> a4 = Ash::Shared::value<int[4]>(6, 7, 8, 9);
					TEST_IS_NOT_NULL(a4);
					TEST_IS_EQ(a4.getReferenceCount(), 1);
					Ash::Shared::Pointer<int[4]> a5(a4);
					TEST_IS_NOT_NULL(a4);
					TEST_IS_EQ(a4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(a5);
					TEST_IS_EQ(a5.getReferenceCount(), 2);
					TEST_IS_EQ(a4, a5);

					a4 = a5;
					TEST_IS_NOT_NULL(a4);
					TEST_IS_EQ(a4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(a5);
					TEST_IS_EQ(a5.getReferenceCount(), 2);
					TEST_IS_EQ(a4, a5);

					Ash::Shared::Pointer<int[4]> a6 = std::move(a4);
					TEST_IS_NULL(a4);
					TEST_IS_EQ(a4.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(a5);
					TEST_IS_EQ(a5.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(a6);
					TEST_IS_EQ(a6.getReferenceCount(), 2);
					TEST_IS_EQ(a5, a6);

					a5 = Ash::Shared::value<int[4]>(9, 8, 7, 6);
					TEST_IS_NOT_NULL(a5);
					TEST_IS_EQ(a5.getReferenceCount(), 1);
					TEST_IS_NOT_NULL(a6);
					TEST_IS_EQ(a6.getReferenceCount(), 1);
					TEST_IS_NOT_EQ(a5, a6);
					TEST_IS_EQ((*a5)[0], 9);
					TEST_IS_EQ((*a5)[1], 8);
					TEST_IS_EQ((*a5)[2], 7);
					TEST_IS_EQ((*a5)[3], 6);
					TEST_IS_EQ((*a6)[0], 6);
					TEST_IS_EQ((*a6)[1], 7);
					TEST_IS_EQ((*a6)[2], 8);
					TEST_IS_EQ((*a6)[3], 9);

					return {};
				}

				static Ash::Test::Assertion object()
				{
					Ash::Shared::Pointer<Point> p1;
					TEST_IS_EQ(p1, nullptr);
					TEST_IS_EQ(p1.getReferenceCount(), 0);

					p1 = Ash::Shared::value(Point(3, 4));
					TEST_IS_NOT_NULL(p1);
					TEST_IS_EQ(p1.getReferenceCount(), 1);
					TEST_IS_TRUE(p1->distance(Point(0, 0)).isEqual(5));

					p1.release();
					TEST_IS_NULL(p1);
					TEST_IS_EQ(p1.getReferenceCount(), 0);

					Ash::Shared::Pointer<Point> p2 = Ash::Shared::value(Point(6, 8));
					TEST_IS_NOT_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 1);
					Ash::Shared::Pointer<Point> p3(std::move(p2));
					TEST_IS_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(p3);
					TEST_IS_EQ(p3.getReferenceCount(), 1);

					p2 = std::move(p3);
					TEST_IS_NOT_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 1);
					TEST_IS_NULL(p3);
					TEST_IS_EQ(p3.getReferenceCount(), 0);
					TEST_IS_TRUE(p2->distance(Point(0, 0)).isEqual(10));

					p2.release();
					TEST_IS_NULL(p2);
					TEST_IS_EQ(p2.getReferenceCount(), 0);

					Ash::Shared::Pointer<Point> p4 = Ash::Shared::value(Point(9, 12));
					TEST_IS_NOT_NULL(p4);
					TEST_IS_EQ(p4.getReferenceCount(), 1);
					Ash::Shared::Pointer<Point> p5(p4);
					TEST_IS_NOT_NULL(p4);
					TEST_IS_EQ(p4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(p5);
					TEST_IS_EQ(p5.getReferenceCount(), 2);
					TEST_IS_EQ(p4, p5);

					p4 = p5;
					TEST_IS_NOT_NULL(p4);
					TEST_IS_EQ(p4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(p5);
					TEST_IS_EQ(p5.getReferenceCount(), 2);
					TEST_IS_EQ(p4, p5);

					Ash::Shared::Pointer<Point> p6 = std::move(p4);
					TEST_IS_NULL(p4);
					TEST_IS_EQ(p4.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(p5);
					TEST_IS_EQ(p5.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(p6);
					TEST_IS_EQ(p6.getReferenceCount(), 2);
					TEST_IS_EQ(p5, p6);

					p5 = Ash::Shared::value(Point(12, 16));
					TEST_IS_NOT_NULL(p5);
					TEST_IS_EQ(p5.getReferenceCount(), 1);
					TEST_IS_NOT_NULL(p6);
					TEST_IS_EQ(p6.getReferenceCount(), 1);
					TEST_IS_NOT_EQ(p5, p6);
					TEST_IS_TRUE(p5->distance(Point(0, 0)).isEqual(20));
					TEST_IS_TRUE(p6->distance(Point(0, 0)).isEqual(15));

					return {};
				}

				static Ash::Test::Assertion derived()
				{
					Ash::Shared::Pointer<Shape> s1;
					TEST_IS_EQ(s1, nullptr);
					TEST_IS_EQ(s1.getReferenceCount(), 0);

					s1 = Ash::Shared::value(Circle(10));
					TEST_IS_NOT_NULL(s1);
					TEST_IS_EQ(s1.getReferenceCount(), 1);
					TEST_IS_TRUE(getCompactnessRatio(*s1).isEqual(1));

					s1.release();
					TEST_IS_NULL(s1);
					TEST_IS_EQ(s1.getReferenceCount(), 0);

					Ash::Shared::Pointer<Shape> s2 = Ash::Shared::value(Circle(20));
					TEST_IS_NOT_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 1);
					Ash::Shared::Pointer<Shape> s3(std::move(s2));
					TEST_IS_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(s3);
					TEST_IS_EQ(s3.getReferenceCount(), 1);

					s2 = std::move(s3);
					TEST_IS_NOT_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 1);
					TEST_IS_NULL(s3);
					TEST_IS_EQ(s3.getReferenceCount(), 0);
					TEST_IS_TRUE(getCompactnessRatio(*s2).isEqual(1));

					s2.release();
					TEST_IS_NULL(s2);
					TEST_IS_EQ(s2.getReferenceCount(), 0);

					Ash::Shared::Pointer<Shape> s4 = Ash::Shared::value(Square(10));
					TEST_IS_NOT_NULL(s4);
					TEST_IS_EQ(s4.getReferenceCount(), 1);
					Ash::Shared::Pointer<Shape> s5(s4);
					TEST_IS_NOT_NULL(s4);
					TEST_IS_EQ(s4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(s5);
					TEST_IS_EQ(s5.getReferenceCount(), 2);
					TEST_IS_EQ(s4, s5);

					s4 = s5;
					TEST_IS_NOT_NULL(s4);
					TEST_IS_EQ(s4.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(s5);
					TEST_IS_EQ(s5.getReferenceCount(), 2);
					TEST_IS_EQ(s4, s5);

					Ash::Shared::Pointer<Shape> s6 = std::move(s4);
					TEST_IS_NULL(s4);
					TEST_IS_EQ(s4.getReferenceCount(), 0);
					TEST_IS_NOT_NULL(s5);
					TEST_IS_EQ(s5.getReferenceCount(), 2);
					TEST_IS_NOT_NULL(s6);
					TEST_IS_EQ(s6.getReferenceCount(), 2);
					TEST_IS_EQ(s5, s6);

					s5 = Ash::Shared::value(Circle(20));
					TEST_IS_NOT_NULL(s5);
					TEST_IS_EQ(s5.getReferenceCount(), 1);
					TEST_IS_NOT_NULL(s6);
					TEST_IS_EQ(s6.getReferenceCount(), 1);
					TEST_IS_NOT_EQ(s5, s6);
					TEST_IS_TRUE(getCompactnessRatio(*s5).isEqual(1));
					TEST_IS_TRUE(getCompactnessRatio(*s6).isEqual(Ash::Double::pi / 4));

					return {};
				}
			}

			namespace Atomic
			{
				class Value
				{
				public:
					inline Value() : m_Mutex(), m_ModifyCount(0), m_Count(0) {}

					constexpr int getCount() const { return m_Count; }

					constexpr size_t getModifyCount() const { return m_ModifyCount; }

					inline void increment()
					{
						m_Mutex.acquire();
						m_ModifyCount++;
						m_Count++;
						m_Mutex.release();
					}

					inline void decrement()
					{
						m_Mutex.acquire();
						m_ModifyCount++;
						m_Count--;
						m_Mutex.release();
					}

				private:
					Ash::Concurrency::Mutex m_Mutex;
					size_t                  m_ModifyCount;
					int                     m_Count;
				};

				static Ash::Test::Assertion pointer()
				{
					Ash::Shared::Atomic::Pointer<Value> value = Ash::Shared::Atomic::value<Value>();

					{
						Ash::Concurrency::Thread incrementThread[200];
						Ash::Concurrency::Thread decrementThread[200];

						for (size_t n = 0; n < 200; n++)
						{
							incrementThread[n].run
							(
								[](Ash::Shared::Atomic::Pointer<Value> value)
								{
									for (size_t n = 0; n < 5000; n++)
									{
										value->increment();
									}
								},
								value
							);
							decrementThread[n].run
							(
								[](Ash::Shared::Atomic::Pointer<Value> value)
								{
									for (size_t n = 0; n < 5000; n++)
									{
										value->decrement();
									}
								},
								value
							);
						}
					}

					TEST_IS_EQ(value.getReferenceCount(), 1);
					TEST_IS_EQ(value->getCount(), 0);
					TEST_IS_EQ(value->getModifyCount(), 2000000);

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testPointer,
			TEST_CASE(Ash::Test::Unique::Pointer::primitive),
			TEST_CASE(Ash::Test::Unique::Pointer::array),
			TEST_CASE(Ash::Test::Unique::Pointer::object),
			TEST_CASE(Ash::Test::Unique::Pointer::derived),

			TEST_CASE(Ash::Test::Shared::Pointer::primitive),
			TEST_CASE(Ash::Test::Shared::Pointer::array),
			TEST_CASE(Ash::Test::Shared::Pointer::object),
			TEST_CASE(Ash::Test::Shared::Pointer::derived),

			TEST_CASE(Ash::Test::Shared::Atomic::pointer)
		);
	}
}
