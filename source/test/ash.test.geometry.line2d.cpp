#include "ash.geometry.line2d.h"
#include "ash.test.geometry.line2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			namespace Line2D
			{
				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion vector()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Line line;
					Vector vector;

					line = Line(Point(1, 1), Point(-1, -1));
					vector = line;
					TEST_IS_TRUE(vector.x.isEqual(-2));
					TEST_IS_TRUE(vector.y.isEqual(-2));

					line = Line(Point(1, -1), Point(-1, 1));
					vector = line;
					TEST_IS_TRUE(vector.x.isEqual(-2));
					TEST_IS_TRUE(vector.y.isEqual(2));

					line = Line(Point(-1, -1), Point(1, 1));
					vector = line;
					TEST_IS_TRUE(vector.x.isEqual(2));
					TEST_IS_TRUE(vector.y.isEqual(2));

					line = Line(Point(-1, 1), Point(1, -1));
					vector = line;
					TEST_IS_TRUE(vector.x.isEqual(2));
					TEST_IS_TRUE(vector.y.isEqual(-2));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion getAngle()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;
					using Angle = Ash::Geometry::Degree<Real>;

					Line line;
					Angle angle;

					line = Line(Point(1, 1), Point(-1, -1));
					angle = line.getAngle();
					TEST_IS_TRUE(angle.isEqual(-135));

					line = Line(Point(1, -1), Point(-1, 1));
					angle = line.getAngle();
					TEST_IS_TRUE(angle.isEqual(135));

					line = Line(Point(-1, -1), Point(1, 1));
					angle = line.getAngle();
					TEST_IS_TRUE(angle.isEqual(45));

					line = Line(Point(-1, 1), Point(1, -1));
					angle = line.getAngle();
					TEST_IS_TRUE(angle.isEqual(-45));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion getLength()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;

					Line line;

					line = Line(Point(1, 1), Point(-1, -1));
					TEST_IS_TRUE(line.getLength().isEqual(Real(8).squareRoot()));

					line = Line(Point(1, -1), Point(-1, 1));
					TEST_IS_TRUE(line.getLength().isEqual(Real(8).squareRoot()));

					line = Line(Point(-1, -1), Point(1, 1));
					TEST_IS_TRUE(line.getLength().isEqual(Real(8).squareRoot()));

					line = Line(Point(-1, 1), Point(1, -1));
					TEST_IS_TRUE(line.getLength().isEqual(Real(8).squareRoot()));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion getPoint()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;

					Line line;

					line = Line(Point(1, 1), Point(-1, -1));
					TEST_IS_TRUE(line.getPoint(0).isEqual(line.startPoint));
					TEST_IS_TRUE(line.getPoint(1).isEqual(line.endPoint));
					TEST_IS_TRUE(line.getPoint(0.5).isEqual(Point(0, 0)));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion add()
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

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion subtract()
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

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion intersects()
				{
					using Real = REAL;
					using Line = Ash::Geometry::Line2D<Real>;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Line line;
					Point point;
					Vector direction;
					Real distance;

					line = Line(Point(2, 2), Point(10, 10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(-1, 1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(-1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, 1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x + Real(1, -Real::fractionSize + 3);
					point.y = point.y - Real(1, -Real::fractionSize);
					direction = Vector(-1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, distance));

					direction = Vector(0, 1);
					TEST_IS_FALSE(line.intersects(point, direction, distance));


					line = Line(Point(-2, 2), Point(-10, 10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(1, 1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, 1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x - Real(1, -Real::fractionSize + 3);
					point.y = point.y - Real(1, -Real::fractionSize);
					direction = Vector(-1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, distance));

					direction = Vector(0, 1);
					TEST_IS_FALSE(line.intersects(point, direction, distance));


					line = Line(Point(-2, -2), Point(-10, -10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(1, -1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, -1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x - Real(1, -Real::fractionSize + 3);
					point.y = point.y + Real(1, -Real::fractionSize);
					direction = Vector(1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, distance));

					direction = Vector(0, -1);
					TEST_IS_FALSE(line.intersects(point, direction, distance));


					line = Line(Point(2, -2), Point(10, -10));
					point = Point(line.endPoint.x, line.startPoint.y);
					direction = Vector(-1, -1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(4));

					direction = Vector(-1, 0);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					direction = Vector(0, -1);
					TEST_IS_TRUE(line.intersects(point, direction, distance));
					TEST_IS_TRUE(distance.isEqual(8));

					point.x = point.x + Real(1, -Real::fractionSize + 3);
					point.y = point.y + Real(1, -Real::fractionSize);
					direction = Vector(-1, 0);
					TEST_IS_FALSE(line.intersects(point, direction, distance));

					direction = Vector(0, -1);
					TEST_IS_FALSE(line.intersects(point, direction, distance));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion collides()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Line = Ash::Geometry::Line2D<Real>;

					Line line;
					Point point;
					Vector lineDirection;
					Vector pointDirection;
					Real lineDistance;
					Real pointDistance;

					line = Line(Point(0, 2), Point(2, 0));
					point = Point(5, 5);
					pointDirection = Vector(-1, -3);
					lineDirection = Vector(3, 1);

					TEST_IS_TRUE(line.collides(lineDirection, point, pointDirection, 1, lineDistance, pointDistance));
					TEST_IS_TRUE(lineDistance.isEqual(0.5));
					TEST_IS_TRUE(pointDistance.isEqual(1));

					line = Line(Point(1, 1), Point(3, -1));
					TEST_IS_TRUE(line.collides(lineDirection, point, pointDirection, 1, lineDistance, pointDistance));
					TEST_IS_TRUE(lineDistance.isEqual(0));
					TEST_IS_TRUE(pointDistance.isEqual(1));

					line = Line(Point(-1, 3), Point(1, 1));
					TEST_IS_TRUE(line.collides(lineDirection, point, pointDirection, 1, lineDistance, pointDistance));
					TEST_IS_TRUE(lineDistance.isEqual(1));
					TEST_IS_TRUE(pointDistance.isEqual(1));

					line = Line(Point(1, 1), Point(3, -1));
					TEST_IS_FALSE(line.collides(lineDirection, point, pointDirection, 1 - Real(1, -Real::fractionSize), lineDistance, pointDistance));

					line = Line(Point(-1, 3), Point(1, 1)) + Vector(-Real(1, -Real::fractionSize), Real(1, -Real::fractionSize)) * 3;
					TEST_IS_FALSE(line.collides(lineDirection, point, pointDirection, 2, lineDistance, pointDistance));

					line = Line(Point(1, 1), Point(3, -1)) + Vector(Real(1, -Real::fractionSize), -Real(1, -Real::fractionSize)) * 3;
					TEST_IS_FALSE(line.collides(lineDirection, point, pointDirection, 2, lineDistance, pointDistance));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion overlaps()
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

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion invalid()
				{
					using Real = REAL;
					using Point = Ash::Geometry::Point2D<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Line = Ash::Geometry::Line2D<Real>;

					Line line = Line::invalid();
					Real lineDistance;
					Real pointDistance;

					TEST_IS_FALSE(Vector(line).isValid());
					TEST_IS_FALSE(line.getAngle().isValid());
					TEST_IS_FALSE(line.getLength().isValid());
					TEST_IS_FALSE(line.getPoint(0).isValid());
					TEST_IS_FALSE((line + Vector(1, 1)).isValid());
					TEST_IS_FALSE((line - Vector(1, 1)).isValid());
					TEST_IS_FALSE(line.intersects(Point(0,0), Vector(1, 1), pointDistance));
					TEST_IS_FALSE(line.collides(Vector(1, 0), Point(0, 0), Vector(0, 1), Real::infinity, lineDistance, pointDistance));
					TEST_IS_FALSE(line.overlaps(line));

					return {};
				}
			}

			template
			<
				typename REAL,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			static Ash::Test::Assertion line2D()
			{
				TEST_GENERIC(Ash::Test::Geometry::Line2D::vector, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::getAngle, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::getLength, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::getPoint, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::add, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::subtract, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::intersects, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::collides, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::overlaps, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Line2D::invalid, REAL);

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
