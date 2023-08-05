#pragma once

#include "ash.real.h"


namespace Ash
{
	namespace Geometry
	{
		template <typename REAL, int PER_ROTATION=0>
		class Angle;

		template <typename REAL>
		using Radian = Angle<REAL>;

		template <typename REAL>
		using Degree = Angle<REAL, 360>;

		template <typename REAL>
		using Gradian = Angle<REAL, 400>;

		template <typename REAL, int PER_ROTATION>
		class Angle : public REAL
		{
		public:
			using Real = REAL;

			using MatchType = typename Real::MatchType;

			static constexpr bool isRadian = (PER_ROTATION == 0);

			static constexpr Real perRotation = (isRadian) ? 2 * Real::pi : PER_ROTATION;

			constexpr Angle() : Real(0) {}

			template <typename VALUE>
			constexpr Angle(VALUE value) : Real(normalise(value)) {}

			template <int VALUE_ROTATION>
			constexpr Angle(const Angle<REAL, VALUE_ROTATION> &angle) : Real(normalise(angle * perRotation / angle.perRotation)) {}

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

			constexpr Real sine() const { return std::sin(toRadian(*this)); }

			constexpr Real cosine() const { return std::cos(toRadian(*this)); }

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
