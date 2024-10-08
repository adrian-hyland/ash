#include "ash.geometry.angle.h"
#include "ash.test.geometry.angle.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			namespace Angle
			{
				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion sine()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 12).sine().isEqual(-Real(0.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 10).sine().isEqual(-Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  9).sine().isEqual(-Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  8).sine().isEqual(-Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  6).sine().isEqual(-Real(1.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  4).sine().isEqual(-Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  3).sine().isEqual(-Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  2).sine().isEqual(-Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(                            0).sine().isEqual( Real(0.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  2).sine().isEqual( Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  3).sine().isEqual( Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  4).sine().isEqual( Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  6).sine().isEqual( Real(1.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  8).sine().isEqual( Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  9).sine().isEqual( Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 10).sine().isEqual( Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 12).sine().isEqual( Real(0.00),              Real::Match::Relative, Real(3.0)));

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion cosine()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 12).cosine().isEqual(-Real(1.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 10).cosine().isEqual(-Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  9).cosine().isEqual(-Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  8).cosine().isEqual(-Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  6).cosine().isEqual(-Real(0.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  4).cosine().isEqual( Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  3).cosine().isEqual( Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  2).cosine().isEqual( Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle(                            0).cosine().isEqual( Real(1.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  2).cosine().isEqual( Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  3).cosine().isEqual( Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  4).cosine().isEqual( Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  6).cosine().isEqual(-Real(0.00),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  8).cosine().isEqual(-Real(0.50),              Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  9).cosine().isEqual(-Real(0.50).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 10).cosine().isEqual(-Real(0.75).squareRoot(), Real::Match::Relative, Real(3.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 12).cosine().isEqual(-Real(1.00),              Real::Match::Relative, Real(3.0)));

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion tangent()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 12).tangent().isEqual( Real(0.00),                           Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 * 10).tangent().isEqual( Real(3.00).reciprocal().squareRoot(), Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  9).tangent().isEqual( Real(1.00),                           Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  8).tangent().isEqual( Real(3.00).squareRoot(),              Real::Match::Relative, Real(5.0)));
					if (!Angle::isRadian)
					{
						TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  6).tangent().isEqual(-Real::infinity,                       Real::Match::Relative, Real(5.0)));
					}
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  4).tangent().isEqual(-Real(3.00).squareRoot(),              Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  3).tangent().isEqual(-Real(1.00),                           Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle(-Angle::perRotation / 24 *  2).tangent().isEqual(-Real(3.00).reciprocal().squareRoot(), Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle(                            0).tangent().isEqual( Real(0.00),                           Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  2).tangent().isEqual( Real(3.00).reciprocal().squareRoot(), Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  3).tangent().isEqual( Real(1.00),                           Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  4).tangent().isEqual( Real(3.00).squareRoot(),              Real::Match::Relative, Real(5.0)));
					if (!Angle::isRadian)
					{
						TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  6).tangent().isEqual( Real::infinity,                       Real::Match::Relative, Real(5.0)));
					}
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  8).tangent().isEqual(-Real(3.00).squareRoot(),              Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 *  9).tangent().isEqual(-Real(1.00),                           Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 10).tangent().isEqual(-Real(3.00).reciprocal().squareRoot(), Real::Match::Relative, Real(5.0)));
					TEST_IS_TRUE(Angle( Angle::perRotation / 24 * 12).tangent().isEqual(-Real(0.00),                           Real::Match::Relative, Real(5.0)));

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion arcTangent()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					TEST_IS_TRUE(Angle::arcTangent(-Real::infinity                      ).isEqual(-Angle::perRotation / 24 * 6, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(3.00).squareRoot()             ).isEqual(-Angle::perRotation / 24 * 4, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(1.00)                          ).isEqual(-Angle::perRotation / 24 * 3, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(3.00).reciprocal().squareRoot()).isEqual(-Angle::perRotation / 24 * 2, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.00)                          ).isEqual( 0,                           Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(3.00).reciprocal().squareRoot()).isEqual( Angle::perRotation / 24 * 2, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(1.00)                          ).isEqual( Angle::perRotation / 24 * 3, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(3.00).squareRoot()             ).isEqual( Angle::perRotation / 24 * 4, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real::infinity                      ).isEqual( Angle::perRotation / 24 * 6, Real::Match::Relative, Real(1.0)));

					TEST_IS_TRUE(Angle::arcTangent(-Real(0.00),              -Real(1.00)             ).isEqual(-Angle::perRotation / 24 * 12, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(0.50),              -Real(0.75).squareRoot()).isEqual(-Angle::perRotation / 24 * 10, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(0.50).squareRoot(), -Real(0.50).squareRoot()).isEqual(-Angle::perRotation / 24 *  9, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(0.75).squareRoot(), -Real(0.50)             ).isEqual(-Angle::perRotation / 24 *  8, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(1.00),               Real(0.00)             ).isEqual(-Angle::perRotation / 24 *  6, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(0.75).squareRoot(),  Real(0.50)             ).isEqual(-Angle::perRotation / 24 *  4, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(0.50).squareRoot(),  Real(0.50).squareRoot()).isEqual(-Angle::perRotation / 24 *  3, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent(-Real(0.50),               Real(0.75).squareRoot()).isEqual(-Angle::perRotation / 24 *  2, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.00),               Real(1.00)             ).isEqual( 0,                            Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.50),               Real(0.75).squareRoot()).isEqual( Angle::perRotation / 24 *  2, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.50).squareRoot(),  Real(0.50).squareRoot()).isEqual( Angle::perRotation / 24 *  3, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.75).squareRoot(),  Real(0.50)             ).isEqual( Angle::perRotation / 24 *  4, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(1.00),              -Real(0.00)             ).isEqual( Angle::perRotation / 24 *  6, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.75).squareRoot(), -Real(0.50)             ).isEqual( Angle::perRotation / 24 *  8, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.50).squareRoot(), -Real(0.50).squareRoot()).isEqual( Angle::perRotation / 24 *  9, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.50),              -Real(0.75).squareRoot()).isEqual( Angle::perRotation / 24 * 10, Real::Match::Relative, Real(1.0)));
					TEST_IS_TRUE(Angle::arcTangent( Real(0.00),              -Real(1.00)             ).isEqual( Angle::perRotation / 24 * 12, Real::Match::Relative, Real(1.0)));

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion matchAbsolute()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					Angle angle = Angle::perRotation / 2;
					size_t exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
					Real error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_LTE(angle.match(angle + error, Angle::Match::Absolute), Real(1.0));
					TEST_IS_EQ(angle.match(angle - error, Angle::Match::Absolute), Real(0.5));

					for (int n = -15; n < 0; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle <= -1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(-angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_EQ(angle.match(angle + error, Angle::Match::Absolute), Real(0.5));
						TEST_IS_EQ(angle.match(angle - error, Angle::Match::Absolute), Real(0.5));
					}

					angle = 0.0;
					error = Real::minSubNormal;
					TEST_IS_EQ(angle.match(error, Angle::Match::Absolute), Real(0.5));
					TEST_IS_EQ(angle.match(error, Angle::Match::Absolute), Real(0.5));

					for (int n = 1; n <= 15; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle >= 1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_EQ(angle.match(angle + error, Angle::Match::Absolute), Real(0.5));
						TEST_IS_EQ(angle.match(angle - error, Angle::Match::Absolute), Real(0.5));
					}

					angle = -Angle::perRotation / 2;
					exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
					error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_EQ(angle.match(angle + error, Angle::Match::Absolute), Real(0.5));
					TEST_IS_LTE(angle.match(angle - error, Angle::Match::Absolute), Real(1.0));

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion matchRelative()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					Angle angle = Angle::perRotation / 2;
					size_t exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
					Real error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_LTE(angle.match(angle + error, Angle::Match::Absolute), Real(1.5));
					TEST_IS_EQ(angle.match(angle - error, Angle::Match::Absolute), Real(0.5));

					for (int n = -15; n < 0; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle <= -1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(-angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_LTE(angle.match(angle + error, Angle::Match::Relative), Real(1.0));
						TEST_IS_LTE(angle.match(angle - error, Angle::Match::Relative), Real(1.0));
					}

					angle = 0.0;
					error = Real(1, -Real::fractionSize);
					TEST_IS_EQ(angle.match(error, Angle::Match::Relative), Real(0.5));
					TEST_IS_EQ(angle.match(error, Angle::Match::Relative), Real(0.5));

					for (int n = 1; n <= 15; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle >= 1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_LTE(angle.match(angle + error, Angle::Match::Relative), Real(1.0));
						TEST_IS_LTE(angle.match(angle - error, Angle::Match::Relative), Real(1.0));
					}

					angle = -Angle::perRotation / 2;
					exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
					error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_EQ(angle.match(angle + error, Angle::Match::Absolute), Real(0.5));
					TEST_IS_LTE(angle.match(angle - error, Angle::Match::Absolute), Real(1.5));

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion isEqualAbsolute()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					Angle angle = Angle::perRotation / 2;
					size_t exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
					Real error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Absolute, Real(1.0)));
					TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Absolute, Real(0.5)));

					for (int n = -15; n < 0; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle <= -1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(-angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Absolute, Real(0.5)));
						TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Absolute, Real(0.5)));
					}

					angle = 0.0;
					error = Real::minSubNormal;
					TEST_IS_TRUE(angle.isEqual(error, Angle::Match::Absolute, Real(0.5)));
					TEST_IS_TRUE(angle.isEqual(error, Angle::Match::Absolute, Real(0.5)));

					for (int n = 1; n <= 15; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle >= 1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Absolute, Real(0.5)));
						TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Absolute, Real(0.5)));
					}

					angle = -Angle::perRotation / 2;
					exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
					error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Absolute, Real(0.5)));
					TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Absolute, Real(1.0)));

					return {};
				}

				template
				<
					typename ANGLE,
					typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
				>
				static Ash::Test::Assertion isEqualRelative()
				{
					using Angle = ANGLE;
					using Real = Angle::Real;

					Angle angle = Angle::perRotation / 2;
					size_t exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
					Real error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Absolute, Real(1.5)));
					TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Absolute, Real(0.5)));

					for (int n = -15; n < 0; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle <= -1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(-angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Relative, Real(1.0)));
						TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Relative, Real(1.0)));
					}

					angle = 0.0;
					error = Real(1, -Real::fractionSize);
					TEST_IS_TRUE(angle.isEqual(error, Angle::Match::Relative, Real(0.5)));
					TEST_IS_TRUE(angle.isEqual(error, Angle::Match::Relative, Real(0.5)));

					for (int n = 1; n <= 15; n++)
					{
						angle = Angle::perRotation * n / 32;
						if (angle >= 1.0)
						{
							exponent = Ash::Integer::getBitLength<unsigned int>(angle) - 1;
						}
						else
						{
							exponent = -Ash::Integer::getBitLength<unsigned int>(angle.reciprocal());
						}
						error = Real(1, -Real::fractionSize + exponent);
						TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Relative, Real(1.0)));
						TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Relative, Real(1.0)));
					}

					angle = -Angle::perRotation / 2;
					exponent = Ash::Integer::getBitLength<unsigned int>(-angle) - 1;
					error = Real(1, -Real::fractionSize + exponent);
					TEST_IS_TRUE(angle.isEqual(angle + error, Angle::Match::Absolute, Real(0.5)));
					TEST_IS_TRUE(angle.isEqual(angle - error, Angle::Match::Absolute, Real(1.5)));

					return {};
				}

				template
				<
					typename FROM_ANGLE,
					typename TO_ANGLE,
					typename = Ash::Type::IsClass<FROM_ANGLE, Ash::Geometry::Generic::Angle>,
					typename = Ash::Type::IsClass<TO_ANGLE, Ash::Geometry::Generic::Angle>,
					typename = Ash::Type::IsSame<typename FROM_ANGLE::Real, typename TO_ANGLE::Real>
				>
				static Ash::Test::Assertion convertAngle()
				{
					using FromAngle = FROM_ANGLE;
					using ToAngle = TO_ANGLE;
					using Real = FromAngle::Real;

					for (int n = -24; n <= 24; n++)
					{
						FromAngle from = FromAngle(FromAngle::perRotation / 48 * n);
						ToAngle to(from);
						TEST_IS_TRUE(from.isEqual(FromAngle(to), Real::Match::Relative));
					}

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion convert()
				{
					using Radian = Ash::Geometry::Radian<REAL>;
					using Degree = Ash::Geometry::Degree<REAL>;
					using Gradian = Ash::Geometry::Gradian<REAL>;

					TEST_GENERIC(Ash::Test::Geometry::Angle::convertAngle, Radian, Degree);
					TEST_GENERIC(Ash::Test::Geometry::Angle::convertAngle, Radian, Gradian);

					TEST_GENERIC(Ash::Test::Geometry::Angle::convertAngle, Degree, Radian);
					TEST_GENERIC(Ash::Test::Geometry::Angle::convertAngle, Degree, Gradian);

					TEST_GENERIC(Ash::Test::Geometry::Angle::convertAngle, Gradian, Radian);
					TEST_GENERIC(Ash::Test::Geometry::Angle::convertAngle, Gradian, Degree);

					return {};
				}
			}

			template
			<
				typename ANGLE,
				typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
			>
			static Ash::Test::Assertion angle()
			{
				TEST_GENERIC(Ash::Test::Geometry::Angle::sine, ANGLE);

				TEST_GENERIC(Ash::Test::Geometry::Angle::cosine, ANGLE);

				TEST_GENERIC(Ash::Test::Geometry::Angle::tangent, ANGLE);

				TEST_GENERIC(Ash::Test::Geometry::Angle::arcTangent, ANGLE);

				TEST_GENERIC(Ash::Test::Geometry::Angle::matchAbsolute, ANGLE);

				TEST_GENERIC(Ash::Test::Geometry::Angle::matchRelative, ANGLE);

				TEST_GENERIC(Ash::Test::Geometry::Angle::isEqualAbsolute, ANGLE);

				TEST_GENERIC(Ash::Test::Geometry::Angle::isEqualRelative, ANGLE);

				return {};
			}
		}

		TEST_UNIT
		(
			testAngle,

			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Radian<Ash::Float>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Radian<Ash::Double>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Radian<Ash::LongDouble>),

			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Degree<Ash::Float>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Degree<Ash::Double>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Degree<Ash::LongDouble>),

			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Gradian<Ash::Float>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Gradian<Ash::Double>),
			TEST_CASE_GENERIC(Ash::Test::Geometry::angle, Ash::Geometry::Gradian<Ash::LongDouble>),

			TEST_CASE_GENERIC(Ash::Test::Geometry::Angle::convert, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Angle::convert, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Geometry::Angle::convert, Ash::LongDouble),
		);
	}
}
