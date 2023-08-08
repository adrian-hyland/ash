#pragma once

#include "ash.test.h"
#include "ash.geometry.vector2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			template <typename REAL>
			constexpr Ash::Test::Assertion addVector()
			{
				using Real = REAL;
				using Vector = Ash::Geometry::Vector2D<Real>;
				using Angle = Ash::Geometry::Degree<Real>;

				Vector vector;
				for (Angle angle = -180; angle < 180; angle = angle + 45)
				{
					vector = vector + Vector(5, angle);
				}

				TEST_IS_TRUE(vector.isEqual(Vector(0, 0), Real::MatchRelative, 2));

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion subtractVector()
			{
				using Real = REAL;
				using Vector = Ash::Geometry::Vector2D<Real>;
				using Angle = Ash::Geometry::Degree<Real>;

				Vector vector;
				for (Angle angle = -180; angle < 180; angle = angle + 45)
				{
					vector = vector - Vector(5, angle);
				}

				TEST_IS_TRUE(vector.isEqual(Vector(0, 0), Real::MatchRelative, 2));

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion multiplyVector()
			{
				using Real = REAL;
				using Vector = Ash::Geometry::Vector2D<Real>;

				Vector vector = Vector(-2, 3) * 4;
				TEST_IS_TRUE(vector.x.isEqual(-8, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(12, Real::MatchRelative));

				vector = vector * -5;
				TEST_IS_TRUE(vector.x.isEqual(40, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(-60, Real::MatchRelative));

				vector = vector * 1;
				TEST_IS_TRUE(vector.x.isEqual(40, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(-60, Real::MatchRelative));

				vector = vector * 0;
				TEST_IS_TRUE(vector.x.isEqual(0, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(0, Real::MatchRelative));

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion divideVector()
			{
				using Real = REAL;
				using Vector = Ash::Geometry::Vector2D<Real>;

				Vector vector = Vector(-40, 60) / 4;
				TEST_IS_TRUE(vector.x.isEqual(-10, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(15, Real::MatchRelative));

				vector = vector / -5;
				TEST_IS_TRUE(vector.x.isEqual(2, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(-3, Real::MatchRelative));

				vector = vector / 1;
				TEST_IS_TRUE(vector.x.isEqual(2, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(-3, Real::MatchRelative));

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion negateVector()
			{
				using Real = REAL;
				using Vector = Ash::Geometry::Vector2D<Real>;

				Vector vector = -Vector(1, 2);
				TEST_IS_TRUE(vector.x.isEqual(-1, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(-2, Real::MatchRelative));

				vector = -vector;
				TEST_IS_TRUE(vector.x.isEqual(1, Real::MatchRelative));
				TEST_IS_TRUE(vector.y.isEqual(2, Real::MatchRelative));

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion normaliseVector()
			{
				using Real = REAL;
				using Vector = Ash::Geometry::Vector2D<Real>;
				using Angle = Ash::Geometry::Degree<Real>;

				Vector vector = Vector(10, Angle(45)).normalise();

				TEST_IS_TRUE(vector.getRadius().isEqual(1));
				TEST_IS_TRUE(Angle(vector.getAngle()).isEqual(45));

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion crossProductVector()
			{
				using Real = REAL;
				using Vector = Ash::Geometry::Vector2D<Real>;

				Vector vector1;
				Vector vector2;

				vector1 = Vector(3, 4);
				vector2 = Vector(5, 6);
				TEST_IS_TRUE(vector1.crossProduct(vector2).isEqual(-2));
				TEST_IS_TRUE(vector2.crossProduct(vector1).isEqual(2));

				vector1 = Vector(-3, 4);
				vector2 = Vector(-5, 6);
				TEST_IS_TRUE(vector1.crossProduct(vector2).isEqual(2));
				TEST_IS_TRUE(vector2.crossProduct(vector1).isEqual(-2));

				vector1 = Vector(3, -4);
				vector2 = Vector(5, -6);
				TEST_IS_TRUE(vector1.crossProduct(vector2).isEqual(2));
				TEST_IS_TRUE(vector2.crossProduct(vector1).isEqual(-2));

				vector1 = Vector(-3, 4);
				vector2 = Vector(5, -6);
				TEST_IS_TRUE(vector1.crossProduct(vector2).isEqual(-2));
				TEST_IS_TRUE(vector2.crossProduct(vector1).isEqual(2));

				vector1 = Vector(3, -4);
				vector2 = Vector(-5, 6);
				TEST_IS_TRUE(vector1.crossProduct(vector2).isEqual(-2));
				TEST_IS_TRUE(vector2.crossProduct(vector1).isEqual(2));

				vector1 = Vector(-3, -4);
				vector2 = Vector(-5, -6);
				TEST_IS_TRUE(vector1.crossProduct(vector2).isEqual(-2));
				TEST_IS_TRUE(vector2.crossProduct(vector1).isEqual(2));

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion vector2D()
			{
				TEST_GENERIC(Ash::Test::Geometry::addVector, REAL);

				TEST_GENERIC(Ash::Test::Geometry::subtractVector, REAL);

				TEST_GENERIC(Ash::Test::Geometry::multiplyVector, REAL);

				TEST_GENERIC(Ash::Test::Geometry::divideVector, REAL);

				TEST_GENERIC(Ash::Test::Geometry::negateVector, REAL);

				TEST_GENERIC(Ash::Test::Geometry::normaliseVector, REAL);

				TEST_GENERIC(Ash::Test::Geometry::crossProductVector, REAL);

				return {};
			}
		}

		TEST_UNIT
		(
			testVector2D,

			TEST_CASE_GENERIC(Ash::Test::Geometry::vector2D, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::vector2D, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::vector2D, Ash::LongDouble),
		);
	}
}
