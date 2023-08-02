#pragma once

#include "ash.test.h"
#include "ash.geometry.angle.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			template <template<typename> typename ANGLE, typename REAL>
			constexpr Ash::Test::Assertion sine()
			{
				using Angle = ANGLE<REAL>;
				using Real = REAL;

				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 12).sine().isEqual(-Real(0.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 10).sine().isEqual(-Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  9).sine().isEqual(-Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  8).sine().isEqual(-Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  6).sine().isEqual(-Real(1.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  4).sine().isEqual(-Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  3).sine().isEqual(-Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  2).sine().isEqual(-Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(                            0).sine().isEqual( Real(0.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  2).sine().isEqual( Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  3).sine().isEqual( Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  4).sine().isEqual( Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  6).sine().isEqual( Real(1.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  8).sine().isEqual( Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  9).sine().isEqual( Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 10).sine().isEqual( Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 12).sine().isEqual( Real(0.00),              Real::MatchRelative, Real(3.0)));

				return {};
			}

			template <template<typename> typename ANGLE, typename REAL>
			constexpr Ash::Test::Assertion cosine()
			{
				using Angle = ANGLE<REAL>;
				using Real = REAL;

				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 12).cosine().isEqual(-Real(1.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 10).cosine().isEqual(-Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  9).cosine().isEqual(-Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  8).cosine().isEqual(-Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  6).cosine().isEqual(-Real(0.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  4).cosine().isEqual( Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  3).cosine().isEqual( Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  2).cosine().isEqual( Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle(                            0).cosine().isEqual( Real(1.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  2).cosine().isEqual( Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  3).cosine().isEqual( Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  4).cosine().isEqual( Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  6).cosine().isEqual(-Real(0.00),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  8).cosine().isEqual(-Real(0.50),              Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  9).cosine().isEqual(-Real(0.50).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 10).cosine().isEqual(-Real(0.75).squareRoot(), Real::MatchRelative, Real(3.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 12).cosine().isEqual(-Real(1.00),              Real::MatchRelative, Real(3.0)));

				return {};
			}

			template <template<typename> typename ANGLE, typename REAL>
			constexpr Ash::Test::Assertion tangent()
			{
				using Angle = ANGLE<REAL>;
				using Real = REAL;

				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 12).tangent().isEqual( Real(0.00),                           Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 10).tangent().isEqual( Real(3.00).reciprocal().squareRoot(), Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  9).tangent().isEqual( Real(1.00),                           Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  8).tangent().isEqual( Real(3.00).squareRoot(),              Real::MatchRelative, Real(5.0)));
				if (!Angle::isRadian)
				{
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  6).tangent().isEqual(-Real::infinity,                       Real::MatchRelative, Real(5.0)));
				}
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  4).tangent().isEqual(-Real(3.00).squareRoot(),              Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  3).tangent().isEqual(-Real(1.00),                           Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  2).tangent().isEqual(-Real(3.00).reciprocal().squareRoot(), Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle(                            0).tangent().isEqual( Real(0.00),                           Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  2).tangent().isEqual( Real(3.00).reciprocal().squareRoot(), Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  3).tangent().isEqual( Real(1.00),                           Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  4).tangent().isEqual( Real(3.00).squareRoot(),              Real::MatchRelative, Real(5.0)));
				if (!Angle::isRadian)
				{
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  6).tangent().isEqual( Real::infinity,                       Real::MatchRelative, Real(5.0)));
				}
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  8).tangent().isEqual(-Real(3.00).squareRoot(),              Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  9).tangent().isEqual(-Real(1.00),                           Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 10).tangent().isEqual(-Real(3.00).reciprocal().squareRoot(), Real::MatchRelative, Real(5.0)));
				TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 12).tangent().isEqual(-Real(0.00),                           Real::MatchRelative, Real(5.0)));

				return {};
			}

			template <template<typename> typename ANGLE, typename REAL>
			constexpr Ash::Test::Assertion arcTangent()
			{
				using Angle = ANGLE<REAL>;
				using Real = REAL;

				TEST_IS_TRUE(Angle::arcTangent(-Real::infinity                      ).isEqual(-Angle::perRotation / 24 * 6, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(3.00).squareRoot()             ).isEqual(-Angle::perRotation / 24 * 4, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(1.00)                          ).isEqual(-Angle::perRotation / 24 * 3, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(3.00).reciprocal().squareRoot()).isEqual(-Angle::perRotation / 24 * 2, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.00)                          ).isEqual( 0,                           Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(3.00).reciprocal().squareRoot()).isEqual( Angle::perRotation / 24 * 2, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(1.00)                          ).isEqual( Angle::perRotation / 24 * 3, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(3.00).squareRoot()             ).isEqual( Angle::perRotation / 24 * 4, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real::infinity                      ).isEqual( Angle::perRotation / 24 * 6, Real::MatchRelative, Real(1.0)));

				TEST_IS_TRUE(Angle::arcTangent(-Real(0.00),              -Real(1.00)             ).isEqual(-Angle::perRotation / 24 * 12, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(0.50),              -Real(0.75).squareRoot()).isEqual(-Angle::perRotation / 24 * 10, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(0.50).squareRoot(), -Real(0.50).squareRoot()).isEqual(-Angle::perRotation / 24 *  9, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(0.75).squareRoot(), -Real(0.50)             ).isEqual(-Angle::perRotation / 24 *  8, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(1.00),               Real(0.00)             ).isEqual(-Angle::perRotation / 24 *  6, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(0.75).squareRoot(),  Real(0.50)             ).isEqual(-Angle::perRotation / 24 *  4, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(0.50).squareRoot(),  Real(0.50).squareRoot()).isEqual(-Angle::perRotation / 24 *  3, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent(-Real(0.50),               Real(0.75).squareRoot()).isEqual(-Angle::perRotation / 24 *  2, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.00),               Real(1.00)             ).isEqual( 0,                            Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.50),               Real(0.75).squareRoot()).isEqual( Angle::perRotation / 24 *  2, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.50).squareRoot(),  Real(0.50).squareRoot()).isEqual( Angle::perRotation / 24 *  3, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.75).squareRoot(),  Real(0.50)             ).isEqual( Angle::perRotation / 24 *  4, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(1.00),              -Real(0.00)             ).isEqual( Angle::perRotation / 24 *  6, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.75).squareRoot(), -Real(0.50)             ).isEqual( Angle::perRotation / 24 *  8, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.50).squareRoot(), -Real(0.50).squareRoot()).isEqual( Angle::perRotation / 24 *  9, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.50),              -Real(0.75).squareRoot()).isEqual( Angle::perRotation / 24 * 10, Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Angle::arcTangent( Real(0.00),              -Real(1.00)             ).isEqual( Angle::perRotation / 24 * 12, Real::MatchRelative, Real(1.0)));

				return {};
			}
			
			template <template<typename> typename ANGLE, typename REAL>
			constexpr Ash::Test::Assertion angle()
			{
				TEST_GENERIC(Ash::Test::Geometry::sine, ANGLE,  REAL);

				TEST_GENERIC(Ash::Test::Geometry::cosine, ANGLE,  REAL);

				TEST_GENERIC(Ash::Test::Geometry::tangent, ANGLE,  REAL);

				TEST_GENERIC(Ash::Test::Geometry::arcTangent, ANGLE,  REAL);

				return {};
			}

			template <template<typename> typename FROM_ANGLE, template<typename> typename TO_ANGLE, typename REAL>
			constexpr Ash::Test::Assertion convert()
			{
				using FromAngle = FROM_ANGLE<REAL>;
				using ToAngle = TO_ANGLE<REAL>;
				using Real = REAL;

				for (int n = -24; n <= 24; n++)
				{
					FromAngle from = FromAngle(FromAngle::perRotation / 48 * n);
					ToAngle to(from);
					TEST_IS_TRUE(from.isEqual(FromAngle(to), Real::MatchRelative));
				}

				return {};
			}

			template <typename REAL>
			constexpr Ash::Test::Assertion convert()
			{
				TEST_GENERIC(Ash::Test::Geometry::convert, Ash::Geometry::Radian, Ash::Geometry::Degree,  REAL);
				TEST_GENERIC(Ash::Test::Geometry::convert, Ash::Geometry::Radian, Ash::Geometry::Gradian, REAL);

				TEST_GENERIC(Ash::Test::Geometry::convert, Ash::Geometry::Degree, Ash::Geometry::Radian,  REAL);
				TEST_GENERIC(Ash::Test::Geometry::convert, Ash::Geometry::Degree, Ash::Geometry::Gradian, REAL);

				TEST_GENERIC(Ash::Test::Geometry::convert, Ash::Geometry::Gradian, Ash::Geometry::Radian, REAL);
				TEST_GENERIC(Ash::Test::Geometry::convert, Ash::Geometry::Gradian, Ash::Geometry::Degree, REAL);

				return {};
			}
		}

		TEST_UNIT
		(
			testAngle,

			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Radian, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Radian, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Radian, Ash::LongDouble),

			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Degree, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Degree, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Degree, Ash::LongDouble),

			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Gradian, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Gradian, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Gradian, Ash::LongDouble),

			TEST_CASE_GENERIC(Ash::Test::Geometry::convert, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::convert, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::convert, Ash::LongDouble),
		);
	}
}
