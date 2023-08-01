#pragma once

#include "ash.real.h"


namespace Ash
{
	namespace Geometry
	{
		template <typename REAL, int PER_ROTATION=0>
		class Angle : public REAL
		{
		public:
			using Real = REAL;

			static constexpr bool isRadian = (PER_ROTATION == 0);

			static constexpr Real perRotation = (isRadian) ? 2 * Real::pi : PER_ROTATION;

			constexpr Angle() : Real(0) {}

			template <typename VALUE>
			constexpr Angle(VALUE value) : Real(normalise(value)) {}

			template <int VALUE_ROTATION>
			constexpr Angle(const Angle<REAL, VALUE_ROTATION> &angle) : Real(normalise(angle / angle.perRotation * perRotation)) {}

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
				if (!isRadian)
				{
					if (angle < -perRotation / 2)
					{
						return Real(angle - perRotation / 2).modulus(perRotation) + perRotation / 2;
					}
					else if (angle > perRotation / 2)
					{
						return Real(angle + perRotation / 2).modulus(perRotation) - perRotation / 2;
					}
				}

				return angle;
			}

			static constexpr Real toRadian(Real angle)
			{
				if (!isRadian)
				{
					angle = angle * Real::pi / (perRotation / 2);
				}

				return angle;
			}

			static constexpr Real fromRadian(Real angle)
			{
				if (!isRadian)
				{
					angle = angle * (perRotation / 2) / Real::pi;
				}
				
				return angle;
			}
		};

		template <typename REAL>
		using Radian = Angle<REAL>;

		template <typename REAL>
		using Degree = Angle<REAL, 360>;

		template <typename REAL>
		using Gradian = Angle<REAL, 400>;
	}
}
