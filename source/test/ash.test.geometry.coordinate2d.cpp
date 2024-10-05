#include "ash.geometry.coordinate2d.h"
#include "ash.test.geometry.coordinate2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			namespace Coordinate2D
			{
				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion construct()
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
						TEST_IS_TRUE(Ash::Geometry::Degree<Real>(coord.getAngle()).isEqual(angle[n], Real::Match::Relative));
						TEST_IS_TRUE(coord.getRadius().isEqual(1, Real::Match::Relative));
					}

					for (size_t n = 0; n < sizeof(angle) / sizeof(angle[0]); n++)
					{
						Coordinate coord(Real(1), angle[n]);

						TEST_IS_TRUE(coord.x.isEqual(x[n], Real::Match::Relative, Real(3.0)));
						TEST_IS_TRUE(coord.y.isEqual(y[n], Real::Match::Relative, Real(3.0)));
						TEST_IS_TRUE(Ash::Geometry::Degree<Real>(coord.getAngle()).isEqual(angle[n], Real::Match::Relative));
						TEST_IS_TRUE(coord.getRadius().isEqual(1, Real::Match::Relative));
					}
					
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
					using Coordinate = Ash::Geometry::Coordinate2D<Real>;

					Coordinate coordinate;

					coordinate = Coordinate(0, 0);
					TEST_IS_TRUE(coordinate.isValid());

					coordinate = Coordinate::invalid();
					TEST_IS_FALSE(coordinate.isValid());
					TEST_IS_FALSE(coordinate.getRadius().isValid());
					TEST_IS_FALSE(coordinate.getAngle().isValid());
					TEST_IS_FALSE(coordinate.rotate(Ash::Geometry::Degree<Real>(90)).isValid());

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion rotate()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;
					using Coordinate = Ash::Geometry::Coordinate2D<Real>;

					Coordinate coordinate(5.0, Angle(0.0));
					Coordinate rotatedCoordinate = coordinate;
					Angle angle;

					TEST_IS_TRUE(rotatedCoordinate.x.isEqual(Real(5.0), Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.y.isEqual(Real(0.0), Real::Match::Relative));

					for (int n = 0; n < 8; n++)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					}

					angle = Angle::perRotation * 8 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(coordinate.y, coordinate.x), Real::Match::Relative));

					for (int n = 9; n < 16; n++)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					}

					angle = Angle::perRotation * 16 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(-coordinate.x, coordinate.y), Real::Match::Relative));

					for (int n = 0; n < -8; n--)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					}

					angle = Angle::perRotation * -8 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(coordinate.y, -coordinate.x), Real::Match::Relative));

					for (int n = -9; n < -16; n--)
					{
						angle = Angle::perRotation * n / 32;
						rotatedCoordinate = coordinate.rotate(angle);
						TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
						TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					}

					angle = Angle::perRotation * -16 / 32;
					rotatedCoordinate = coordinate.rotate(angle);
					TEST_IS_TRUE(rotatedCoordinate.getAngle().isEqual(angle, Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.getRadius().isEqual(Real(5.0), Real::Match::Relative));
					TEST_IS_TRUE(rotatedCoordinate.isEqual(Coordinate(-coordinate.x, -coordinate.y), Real::Match::Relative));

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

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::invalid, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::invalid, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::invalid, Ash::LongDouble),

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Radian<Ash::Float>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Radian<Ash::Double>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Radian<Ash::LongDouble>),

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Degree<Ash::Float>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Degree<Ash::Double>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Degree<Ash::LongDouble>),

			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Gradian<Ash::Float>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Gradian<Ash::Double>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Coordinate2D::rotate, Ash::Geometry::Gradian<Ash::LongDouble>),
		);
	}
}
