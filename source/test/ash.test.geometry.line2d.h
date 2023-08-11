#pragma once

#include "ash.test.h"
#include "ash.geometry.line2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			namespace Line2D
			{
				template <typename REAL>
				constexpr Ash::Test::Assertion add()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Angle = Ash::Geometry::Degree<Real>;

					Line line(Point(-10, -10), Point(10, 10));
					for (Angle angle = -180; angle < 180; angle = angle + 45)
					{
						line = line + Vector(5, angle);
					}

					TEST_IS_TRUE(line.startPoint.isEqual(Point(-10, -10), Real::MatchRelative, 2));
					TEST_IS_TRUE(line.endPoint.isEqual(Point(10, 10), Real::MatchRelative, 2));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion subtract()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Angle = Ash::Geometry::Degree<Real>;

					Line line(Point(-10, -10), Point(10, 10));
					for (Angle angle = -180; angle < 180; angle = angle + 45)
					{
						line = line - Vector(5, angle);
					}

					TEST_IS_TRUE(line.startPoint.isEqual(Point(-10, -10), Real::MatchRelative, 2));
					TEST_IS_TRUE(line.endPoint.isEqual(Point(10, 10), Real::MatchRelative, 2));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion intersects()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Line line;
					Point point;
					Vector direction;
					Point intersection;
					Real distance;

					line = Line(Point(2, 2), Point(10, 10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(-1, 1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(6, 6)));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(-1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(2, 2)));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, 1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(10, 10)));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x + Real(1, -Real::fractionSize + 3);
					point.y = point.y - Real(1, -Real::fractionSize);
					direction = Vector(-1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));

					direction = Vector(0, 1);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));


					line = Line(Point(-2, 2), Point(-10, 10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(1, 1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(-6, 6)));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(-2, 2)));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, 1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(-10, 10)));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x - Real(1, -Real::fractionSize + 3);
					point.y = point.y - Real(1, -Real::fractionSize);
					direction = Vector(-1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));

					direction = Vector(0, 1);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));


					line = Line(Point(-2, -2), Point(-10, -10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(1, -1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(-6, -6)));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(-2, -2)));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, -1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(-10, -10)));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x - Real(1, -Real::fractionSize + 3);
					point.y = point.y + Real(1, -Real::fractionSize);
					direction = Vector(1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));

					direction = Vector(0, -1);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));


					line = Line(Point(2, -2), Point(10, -10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(-1, -1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(6, -6)));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(-1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(2, -2)));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, -1);
					TEST_IS_TRUE(line.intersects(point, direction, intersection, distance));
					TEST_IS_TRUE(intersection.isEqual(Point(10, -10)));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x + Real(1, -Real::fractionSize + 3);
					point.y = point.y + Real(1, -Real::fractionSize);
					direction = Vector(-1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));

					direction = Vector(0, -1);
					TEST_IS_FALSE(line.intersects(point, direction, intersection, distance));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion collides()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Line = Ash::Geometry::Line2D<Real>;

					Line line;
					Point point;
					Vector lineDirection;
					Vector pointDirection;
					Point linePoint;
					Point collisionPoint;
					Real distance;

					line = Line(Point(0, 2), Point(2, 0));
					point = Point(5, 5);
					pointDirection = Vector(-1, -3);
					lineDirection = Vector(3, 1);

					TEST_IS_TRUE(line.collides(lineDirection, point, pointDirection, 1, linePoint, collisionPoint, distance));
					TEST_IS_TRUE(linePoint.isEqual(Point(1, 1)));
					TEST_IS_TRUE(collisionPoint.isEqual(Point(4, 2)));
					TEST_IS_TRUE(distance.isEqual(1));

					line = Line(Point(1, 1), Point(3, -1));
					TEST_IS_TRUE(line.collides(lineDirection, point, pointDirection, 1, linePoint, collisionPoint, distance));
					TEST_IS_TRUE(linePoint.isEqual(Point(1, 1)));
					TEST_IS_TRUE(collisionPoint.isEqual(Point(4, 2)));
					TEST_IS_TRUE(distance.isEqual(1));

					line = Line(Point(-1, 3), Point(1, 1));
					TEST_IS_TRUE(line.collides(lineDirection, point, pointDirection, 1, linePoint, collisionPoint, distance));
					TEST_IS_TRUE(linePoint.isEqual(Point(1, 1)));
					TEST_IS_TRUE(collisionPoint.isEqual(Point(4, 2)));
					TEST_IS_TRUE(distance.isEqual(1));

					line = Line(Point(1, 1), Point(3, -1));
					TEST_IS_FALSE(line.collides(lineDirection, point, pointDirection, 1 - Real(1, -Real::fractionSize), linePoint, collisionPoint, distance));

					line = Line(Point(-1, 3), Point(1, 1)) + Vector(-Real(1, -Real::fractionSize), Real(1, -Real::fractionSize)) * 3;
					TEST_IS_FALSE(line.collides(lineDirection, point, pointDirection, 2, linePoint, collisionPoint, distance));

					line = Line(Point(1, 1), Point(3, -1)) + Vector(Real(1, -Real::fractionSize), -Real(1, -Real::fractionSize)) * 3;
					TEST_IS_FALSE(line.collides(lineDirection, point, pointDirection, 2, linePoint, collisionPoint, distance));

					return {};
				}

				template <typename REAL>
				constexpr Ash::Test::Assertion overlaps()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Line = Ash::Geometry::Line2D<Real>;

					Line line1;
					Line line2;

					line1 = Line(Point(0, 4), Point(4, 0));
					line2 = Line(Point(0, 0), Point(4, 4));
					TEST_IS_TRUE(line1.overlaps(line2));

					line1 = Line(Point(0, 4), Point(4, 0));
					line2 = Line(Point(2, 2), Point(6, 6));
					TEST_IS_TRUE(line1.overlaps(line2));

					line1 = Line(Point(0, 4), Point(4, 0));
					line2 = Line(Point(2, 2), Point(6, 6)) + Vector(Real(1, -Real::fractionSize), Real(1, -Real::fractionSize)) * 2;
					TEST_IS_FALSE(line1.overlaps(line2));

					return {};
				}
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion line2D()
			{
				TEST_GENERIC(Ash::Test::Geometry::Line2D::add, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::subtract, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::intersects, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::collides, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::overlaps, REAL);

				return {};
			}
		}

		TEST_UNIT
		(
			testLine2D,

			TEST_CASE_GENERIC(Ash::Test::Geometry::line2D, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::line2D, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::line2D, Ash::LongDouble),
		);
	}
}
