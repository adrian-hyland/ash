#pragma once

#include "ash.test.h"
#include "ash.geometry.coordinate2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			template <typename REAL>
			constexpr Ash::Test::Assertion coordinate2D()
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
		}

		TEST_UNIT
		(
			testCoordinate2D,

			TEST_CASE_GENERIC(Ash::Test::Geometry::coordinate2D, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::coordinate2D, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::coordinate2D, Ash::LongDouble)
		);
	}
}
