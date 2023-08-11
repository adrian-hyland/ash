#pragma once

#include "ash.test.h"
#include "ash.geometry.coordinate2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			namespace Coordinate2D
			{
				template <typename REAL>
				constexpr Ash::Test::Assertion construct()
				{
					using Real = REAL;
					using Coordinate = Ash::Geometry::Coordinate2D<Real>;

					Ash::Geometry::Degree<Real> angle[] =
					{
						Real(-180),
						Real(-150),
						Real(-135),
						Real(-120),
						Real(-90),
						Real(-60),
						Real(-45),
						Real(-30),
						Real(0),
						Real(30),
						Real(45),
						Real(60),
						Real(90),
						Real(120),
						Real(135),
						Real(150),
						Real(180)
					};

					Real x[] =
					{
						Real(1,00).negate(),
						Real(0.75).squareRoot().negate(),
						Real(0.50).squareRoot().negate(),
						Real(0.50).negate(),
						Real(0.00),
						Real(0.50),
						Real(0.50).squareRoot(),
						Real(0.75).squareRoot(),
						Real(1.00),
						Real(0.75).squareRoot(),
						Real(0.50).squareRoot(),
						Real(0.50),
						Real(0.00),
						Real(0.50).negate(),
						Real(0.50).squareRoot().negate(),
						Real(0.75).squareRoot().negate(),
						Real(1.00).negate()
					};

					Real y[] =
					{
						Real(0.00),
						Real(0.50).negate(),
						Real(0.50).squareRoot().negate(),
						Real(0.75).squareRoot().negate(),
						Real(1.00).negate(),
						Real(0.75).squareRoot().negate(),
						Real(0.50).squareRoot().negate(),
						Real(0.50).negate(),
						Real(0.00),
						Real(0.50),
						Real(0.50).squareRoot(),
						Real(0.75).squareRoot(),
						Real(1.00),
						Real(0.75).squareRoot(),
						Real(0.50).squareRoot(),
						Real(0.50),
						Real(0.00)
					};

					for (size_t n = 0; n < sizeof(angle) / sizeof(angle[0]); n++)
					{
						Coordinate coord(x[n], y[n]);

						TEST_IS_EQ(coord.x, x[n]);
						TEST_IS_EQ(coord.y, y[n]);
						TEST_IS_TRUE(Ash::Geometry::Degree<Real>(coord.getAngle()).isEqual(angle[n], Real::MatchRelative));
						TEST_IS_TRUE(coord.getRadius().isEqual(1, Real::MatchRelative));
					}

					for (size_t n = 0; n < sizeof(angle) / sizeof(angle[0]); n++)
					{
						Coordinate coord(Real(1), angle[n]);

						TEST_IS_TRUE(coord.x.isEqual(x[n], Real::MatchRelative, Real(3.0)));
						TEST_IS_TRUE(coord.y.isEqual(y[n], Real::MatchRelative, Real(3.0)));
						TEST_IS_TRUE(Ash::Geometry::Degree<Real>(coord.getAngle()).isEqual(angle[n], Real::MatchRelative));
						TEST_IS_TRUE(coord.getRadius().isEqual(1, Real::MatchRelative));
					}
					
					return {};
				}

				template <template<typename> typename ANGLE, typename REAL>
				constexpr Ash::Test::Assertion rotate()
				{
					using Real = REAL;
					using Angle = ANGLE<Real>;
					using Coordinate = Ash::Geometry::Coordinate2D<Real>;

					Coordinate coordinate(5.0, Angle(0.0));
					Coordinate rotatedCoordinate = coordinate;
					Angle angle;

					TEST_IS_TRUE(rotatedCoordinate.x.isEqual(Real(5.0), Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.y.isEqual(Real(0.0), Real::MatchRelative));

					for (int n = 0; n < 8; n++)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					}

					angle = Angle::perRotation * 8 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(coordinate.y, coordinate.x), Real::MatchRelative));

					for (int n = 9; n < 16; n++)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					}

					angle = Angle::perRotation * 16 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(-coordinate.x, coordinate.y), Real::MatchRelative));

					for (int n = 0; n < -8; n--)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					}

					angle = Angle::perRotation * -8 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(coordinate.y, -coordinate.x), Real::MatchRelative));

					for (int n = -9; n < -16; n--)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					}

					angle = Angle::perRotation * -16 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::MatchRelative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(-coordinate.x, -coordinate.y), Real::MatchRelative));

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testCoordinate2D,

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::construct, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::construct, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::construct, Ash::LongDouble),

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Radian, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Radian, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Radian, Ash::LongDouble),

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Degree, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Degree, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Degree, Ash::LongDouble),

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Gradian, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Gradian, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Gradian, Ash::LongDouble),
		);
	}
}
