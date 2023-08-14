#pragma once

#include "ash.test.h"
#include "ash.geometry.point2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			namespace Point2D
			{
				template <typename REAL>
				constexpr Ash::Test::Assertion add()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Angle = Ash::Geometry::Degree<Real>;

					Point point(10, 10);
					for (Angle angle = -180; angle < 180; angle = angle + 45)
					{
						point = point + Vector(5, angle);
					}

					TEST_IS_TRUE(point.isEqual(Point(10, 10), Real::MatchRelative, 2));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion subtract()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Point origin = Point(5, 5);

					TEST_IS_TRUE((Point(10, 5) - origin).isEqual(Vector(5, 0)));
					TEST_IS_TRUE((Point(10, 10) - origin).isEqual(Vector(5, 5)));
					TEST_IS_TRUE((Point(5, 10) - origin).isEqual(Vector(0, 5)));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion negate()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;

					Point point = -Point(1, 2);
					TEST_IS_TRUE(point.x.isEqual(-1, Real::MatchRelative));
					TEST_IS_TRUE(point.y.isEqual(-2, Real::MatchRelative));

					point = -point;
					TEST_IS_TRUE(point.x.isEqual(1, Real::MatchRelative));
					TEST_IS_TRUE(point.y.isEqual(2, Real::MatchRelative));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion intersects()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Point point1;
					Point point2;
					Vector direction1;
					Vector direction2;
					Real distance1;
					Real distance2;

					point1 = Point(2, 2);
					point2 = Point(10, 2);
					direction1 = Vector(1, 1);
					direction2 = Vector(-1, 1);
					TEST_IS_TRUE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));
					TEST_IS_TRUE(distance1.isEqual(4));
					TEST_IS_TRUE(distance2.isEqual(4));
					TEST_IS_TRUE((point1 + direction1 * distance1).isEqual(point2 + direction2 * distance2));

					point1 = Point(2, 2);
					point2 = Point(2, 10);
					direction1 = Vector(1, 1);
					direction2 = Vector(-1, 1);
					TEST_IS_TRUE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));
					TEST_IS_TRUE(distance1.isEqual(4));
					TEST_IS_TRUE(distance2.isEqual(-4));
					TEST_IS_TRUE((point1 + direction1 * distance1).isEqual(point2 + direction2 * distance2));

					point1 = Point(2, 2);
					point2 = Point(2, -6);
					direction1 = Vector(1, 1);
					direction2 = Vector(-1, 1);
					TEST_IS_TRUE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));
					TEST_IS_TRUE(distance1.isEqual(-4));
					TEST_IS_TRUE(distance2.isEqual(4));
					TEST_IS_TRUE((point1 + direction1 * distance1).isEqual(point2 + direction2 * distance2));

					point1 = Point(2, 2);
					point2 = Point(-6, 2);
					direction1 = Vector(1, 1);
					direction2 = Vector(-1, 1);
					TEST_IS_TRUE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));
					TEST_IS_TRUE(distance1.isEqual(-4));
					TEST_IS_TRUE(distance2.isEqual(-4));
					TEST_IS_TRUE((point1 + direction1 * distance1).isEqual(point2 + direction2 * distance2));

					point1 = Point(-2, 3);
					point2 = Point(4, -3);
					direction1 = Vector(4, 1);
					direction2 = Vector(-1, 5);
					TEST_IS_TRUE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));
					TEST_IS_TRUE((point1 + direction1 * distance1).isEqual(point2 + direction2 * distance2));

					point1 = Point(2, 2);
					point2 = Point(10, 2);
					direction1 = Vector(1, 1);
					direction2 = Vector(1, 1);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));

					point1 = Point(2, 2);
					point2 = Point(10, 2);
					direction1 = Vector(-1, 1);
					direction2 = Vector(1, -1);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));

					point1 = Point(2, 2);
					point2 = Point(10, 2);
					direction1 = Vector(-1, -1);
					direction2 = Vector(-1, -1);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, &distance1, &distance2));

					point1 = Point(2, 2);
					point2 = Point(10, 2);
					direction1 = Vector(1, 1);
					direction2 = Vector(-1, 1);
					TEST_IS_TRUE(Point::intersects(point1, direction1, point2, direction2, 2.0, 6.0, &distance2));
					TEST_IS_TRUE(distance2.isEqual(4));

					point1 = Point(2, 2);
					point2 = Point(10, 2);
					direction1 = Vector(1, 1);
					direction2 = Vector(-1, 1);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, 5.0, 6.0, &distance2));

					point1 = Point(2, 2);
					point2 = Point(10, 2);
					direction1 = Vector(1, 1);
					direction2 = Vector(-1, 1);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, 2.0, 3.0, &distance2));

					point1 = Point(2, 1);
					point2 = Point(20, -4);
					direction1 = Vector(2, 1);
					direction2 = Vector(-3, 2);
					TEST_IS_TRUE(Point::intersects(point1, direction1, point2, direction2, 2.0, 4.0, 3.0, 5.0, &distance1, &distance2));
					TEST_IS_TRUE(distance1.isEqual(3));
					TEST_IS_TRUE(distance2.isEqual(4));

					point1 = Point(2, 1);
					point2 = Point(20, -4);
					direction1 = Vector(2, 1);
					direction2 = Vector(-3, 2);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, 1.0, 2.0, 3.0, 5.0, &distance1, &distance2));

					point1 = Point(2, 1);
					point2 = Point(20, -4);
					direction1 = Vector(2, 1);
					direction2 = Vector(-3, 2);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, 4.0, 5.0, 3.0, 5.0, &distance1, &distance2));

					point1 = Point(2, 1);
					point2 = Point(20, -4);
					direction1 = Vector(2, 1);
					direction2 = Vector(-3, 2);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, 2.0, 4.0, 1.0, 3.0, &distance1, &distance2));

					point1 = Point(2, 1);
					point2 = Point(20, -4);
					direction1 = Vector(2, 1);
					direction2 = Vector(-3, 2);
					TEST_IS_FALSE(Point::intersects(point1, direction1, point2, direction2, 2.0, 4.0, 5.0, 36.0, &distance1, &distance2));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion invalid()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Point point = Point::invalid;

					TEST_IS_FALSE((point + Vector(1, 1)).isValid());
					TEST_IS_FALSE((point - Point(1, 1)).isValid());
					TEST_IS_FALSE((-point).isValid());
					TEST_IS_FALSE(Point::intersects(point, Vector(1, 0), Point(1, 1), Vector(0, 1)));

					return {};
				}
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion point2D()
			{
				TEST_GENERIC(Ash::Test::Geometry::Point2D::add, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Point2D::subtract, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Point2D::negate, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Point2D::intersects, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Point2D::invalid, REAL);

				return {};
			}
		}

		TEST_UNIT
		(
			testPoint2D,

			TEST_CASE_GENERIC(Ash::Test::Geometry::point2D, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::point2D, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::point2D, Ash::LongDouble),
		);
	}
}
