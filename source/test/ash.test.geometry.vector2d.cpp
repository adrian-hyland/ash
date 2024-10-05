#include "ash.geometry.vector2d.h"
#include "ash.test.geometry.vector2d.h"


namespace Ash
{
	namespace Test
	{
		namespace Geometry
		{
			namespace Vector2D
			{
				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion add()
				{
					using Real = REAL;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Angle = Ash::Geometry::Degree<Real>;

					Vector vector;
					for (Angle angle = -180; angle < 180; angle = angle + 45)
					{
						vector = vector + Vector(5, angle);
					}

					TEST_IS_TRUE(vector.isEqual(Vector(0, 0), Real::Match::Relative, 2));

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
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Angle = Ash::Geometry::Degree<Real>;

					Vector vector;
					for (Angle angle = -180; angle < 180; angle = angle + 45)
					{
						vector = vector - Vector(5, angle);
					}

					TEST_IS_TRUE(vector.isEqual(Vector(0, 0), Real::Match::Relative, 2));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion multiply()
				{
					using Real = REAL;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Vector vector = Vector(-2, 3) * 4;
					TEST_IS_TRUE(vector.x.isEqual(-8, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(12, Real::Match::Relative));

					vector = vector * -5;
					TEST_IS_TRUE(vector.x.isEqual(40, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(-60, Real::Match::Relative));

					vector = vector * 1;
					TEST_IS_TRUE(vector.x.isEqual(40, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(-60, Real::Match::Relative));

					vector = vector * 0;
					TEST_IS_TRUE(vector.x.isEqual(0, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(0, Real::Match::Relative));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion divide()
				{
					using Real = REAL;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Vector vector = Vector(-40, 60) / 4;
					TEST_IS_TRUE(vector.x.isEqual(-10, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(15, Real::Match::Relative));

					vector = vector / -5;
					TEST_IS_TRUE(vector.x.isEqual(2, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(-3, Real::Match::Relative));

					vector = vector / 1;
					TEST_IS_TRUE(vector.x.isEqual(2, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(-3, Real::Match::Relative));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion negate()
				{
					using Real = REAL;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Vector vector = -Vector(1, 2);
					TEST_IS_TRUE(vector.x.isEqual(-1, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(-2, Real::Match::Relative));

					vector = -vector;
					TEST_IS_TRUE(vector.x.isEqual(1, Real::Match::Relative));
					TEST_IS_TRUE(vector.y.isEqual(2, Real::Match::Relative));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion normalise()
				{
					using Real = REAL;
					using Vector = Ash::Geometry::Vector2D<Real>;
					using Angle = Ash::Geometry::Degree<Real>;

					Vector vector = Vector(10, Angle(45)).normalise();

					TEST_IS_TRUE(vector.getRadius().isEqual(1));
					TEST_IS_TRUE(Angle(vector.getAngle()).isEqual(45));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion dotProduct()
				{
					using Real = REAL;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Vector vector1;
					Vector vector2;

					vector1 = Vector(3, 4);
					TEST_IS_TRUE(vector1.dotProduct(vector1).isEqual(25));

					vector1 = Vector(3, 4);
					vector2 = Vector(5, 6);
					TEST_IS_TRUE(vector1.dotProduct(vector2).isEqual(39));
					TEST_IS_TRUE(vector2.dotProduct(vector1).isEqual(39));

					vector1 = Vector(-3, 4);
					vector2 = Vector(-5, 6);
					TEST_IS_TRUE(vector1.dotProduct(vector2).isEqual(39));
					TEST_IS_TRUE(vector2.dotProduct(vector1).isEqual(39));

					vector1 = Vector(3, -4);
					vector2 = Vector(5, -6);
					TEST_IS_TRUE(vector1.dotProduct(vector2).isEqual(39));
					TEST_IS_TRUE(vector2.dotProduct(vector1).isEqual(39));

					vector1 = Vector(-3, 4);
					vector2 = Vector(5, -6);
					TEST_IS_TRUE(vector1.dotProduct(vector2).isEqual(-39));
					TEST_IS_TRUE(vector2.dotProduct(vector1).isEqual(-39));

					vector1 = Vector(3, -4);
					vector2 = Vector(-5, 6);
					TEST_IS_TRUE(vector1.dotProduct(vector2).isEqual(-39));
					TEST_IS_TRUE(vector2.dotProduct(vector1).isEqual(-39));

					vector1 = Vector(-3, -4);
					vector2 = Vector(-5, -6);
					TEST_IS_TRUE(vector1.dotProduct(vector2).isEqual(39));
					TEST_IS_TRUE(vector2.dotProduct(vector1).isEqual(39));

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion crossProduct()
				{
					using Real = REAL;
					using Vector = Ash::Geometry::Vector2D<Real>;

					Vector vector1;
					Vector vector2;

					vector1 = Vector(3, 4);
					TEST_IS_TRUE(vector1.crossProduct(vector1).isEqual(0));

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

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion reflect()
				{
					using Real = REAL;
					using Angle = Ash::Geometry::Degree<Real>;
					using Vector = Ash::Geometry::Vector2D<Real>;

					for (Angle reflectorAngle = -180; reflectorAngle < 180; reflectorAngle = reflectorAngle + 10)
					{
						Vector reflector = Vector(10, reflectorAngle);
						for (Angle angle = -180; angle < 180; angle = angle + 10)
						{
							Vector vector = Vector(10, angle);
							Vector reflection = reflector.reflect(vector);

							vector = vector.rotate(Angle(-reflectorAngle));
							reflection = reflection.rotate(Angle(-reflectorAngle));

							TEST_IS_TRUE(vector.x.isEqual(reflection.x, Real::Match::Relative, 5));
							TEST_IS_TRUE(vector.y.isEqual(reflection.y.negate(), Real::Match::Relative, 5));
						}
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
					using Vector = Ash::Geometry::Vector2D<Real>;

					Vector vector = Vector::invalid();

					TEST_IS_FALSE((vector + Vector(1, 1)).isValid());
					TEST_IS_FALSE((vector - Vector(1, 1)).isValid());
					TEST_IS_FALSE((vector * 2).isValid());
					TEST_IS_FALSE((vector / 2).isValid());
					TEST_IS_FALSE((-vector).isValid());
					TEST_IS_FALSE(vector.normalise().isValid());
					TEST_IS_FALSE(vector.dotProduct(Vector(1, 1)).isValid());
					TEST_IS_FALSE(vector.crossProduct(Vector(1, 1)).isValid());
					TEST_IS_FALSE(vector.reflect(Vector(1, 1)).isValid());

					return {};
				}
			}

			template
			<
				typename REAL,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			static Ash::Test::Assertion vector2D()
			{
				TEST_GENERIC(Ash::Test::Geometry::Vector2D::add, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::subtract, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::multiply, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::divide, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::negate, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::normalise, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::dotProduct, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::crossProduct, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::reflect, REAL);

				TEST_GENERIC(Ash::Test::Geometry::Vector2D::invalid, REAL);

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
