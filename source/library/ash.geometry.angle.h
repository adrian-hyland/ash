#pragma once

#include "ash.type.h"
#include "ash.real.h"


namespace Ash
{
	namespace Geometry
	{
		namespace Generic
		{
			class Angle {};
		}

		template
		<
			typename REAL,
			int      PER_ROTATION = 0,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		class Angle;

		template
		<
			typename REAL,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		using Radian = Angle<REAL>;

		template
		<
			typename REAL,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		using Degree = Angle<REAL, 360>;

		template
		<
			typename REAL,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		using Gradian = Angle<REAL, 400>;

		template
		<
			typename REAL,
			int      PER_ROTATION,
			typename
		>
		class Angle : public REAL, Ash::Geometry::Generic::Angle
		{
		public:
			using Real = REAL;

			using MatchType = typename Real::MatchType;

			static constexpr bool isRadian = (PER_ROTATION == 0);

			static constexpr Real perRotation = (isRadian) ? 2 * Real::pi : PER_ROTATION;

			constexpr Angle() : Real(0) {}

			constexpr Angle(Real value) : Real(normalise(value)) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsNumeric<VALUE>
			>
			constexpr Angle(VALUE value) : Real(normalise(value)) {}

			template
			<
				typename ANGLE,
				typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
			>
			constexpr Angle(const ANGLE &angle) : Real(normalise(angle * perRotation / angle.perRotation)) {}

			constexpr Real match(Angle angle, MatchType matchType = MatchType::MatchRelative) const
			{
				Real thisValue = *this;

				if ((thisValue != 0) && (angle != 0))
				{
					if (thisValue.isPositive())
					{
						if (angle.isNegative())
						{
							thisValue = thisValue - perRotation;
						}
					}
					else if (angle.isPositive())
					{
						thisValue = thisValue + perRotation;
					}
				}

				return thisValue.match(angle, matchType);
			}

			constexpr bool isEqual(Angle angle, MatchType matchType = MatchType::MatchRelative, Real tolerance = 1.0) const
			{
				Real error = match(angle, matchType);
			
				return error.isValid() && (error <= tolerance);
			}

			constexpr Real sine() const
			{
				if (*this <= -perRotation * 3 / 8)
				{
					return -std::sin(toRadian(*this + perRotation / 2));
				}
				else if (*this >= perRotation * 3 / 8)
				{
					return -std::sin(toRadian(*this - perRotation / 2));
				}
				else if (*this < -perRotation / 8)
				{
					return -std::cos(toRadian(*this + perRotation / 4));
				}
				else if (*this > perRotation / 8)
				{
					return std::cos(toRadian(*this - perRotation / 4));
				}
				else
				{
					return std::sin(toRadian(*this));
				}
			}

			constexpr Real cosine() const
			{
				if (*this <= -perRotation * 3 / 8)
				{
					return -std::cos(toRadian(*this + perRotation / 2));
				}
				else if (*this >= perRotation * 3 / 8)
				{
					return -std::cos(toRadian(*this - perRotation / 2));
				}
				else if (*this < -perRotation / 8)
				{
					return std::sin(toRadian(*this + perRotation / 4));
				}
				else if (*this > perRotation / 8)
				{
					return -std::sin(toRadian(*this - perRotation / 4));
				}
				else
				{
					return std::cos(toRadian(*this));
				}
			}

			constexpr Real tangent() const
			{
				if (!isRadian)
				{
					if (*this == perRotation / 4)
					{
						return Real::infinity;
					}
					else if (*this == -perRotation / 4)
					{
						return -Real::infinity;
					}
				}

				return std::tan(toRadian(*this));
			}

			static constexpr Angle arcTangent(Real value) { return fromRadian(std::atan(value)); }

			static constexpr Angle arcTangent(Real y, Real x) { return fromRadian(std::atan2(y, x)); }

		protected:
			static constexpr Real normalise(Real angle)
			{
				if (angle < -perRotation / 2)
				{
					return Real(angle - perRotation / 2).modulus(perRotation) + perRotation / 2;
				}
				else if (angle > perRotation / 2)
				{
					return Real(angle + perRotation / 2).modulus(perRotation) - perRotation / 2;
				}

				return angle;
			}

			static constexpr Real toRadian(Real angle)
			{
				if (!isRadian)
				{
					angle = angle * Radian<Real>::perRotation / perRotation;
				}

				return angle;
			}

			static constexpr Real fromRadian(Real angle)
			{
				if (!isRadian)
				{
					angle = angle * perRotation / Radian<Real>::perRotation;
				}
				
				return angle;
			}
		};
	}
}
