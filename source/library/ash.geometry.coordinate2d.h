#pragma once

#include "ash.geometry.angle.h"


namespace Ash
{
	namespace Geometry
	{
		template <typename REAL>
		struct Coordinate2D
		{
			using Real = REAL;

			using Angle = Ash::Geometry::Radian<Real>;

			constexpr Coordinate2D(Real xValue, Real yValue) : x(xValue), y(yValue) {}

			template <int PER_ROTATION>
			constexpr Coordinate2D(Real radius, Ash::Geometry::Angle<Real, PER_ROTATION> angle) : x(radius * angle.cosine()), y(radius * angle.sine()) {}

			constexpr Angle getAngle() const { return Angle::arcTangent(y, x); }

			constexpr Real getRadius() const { return std::hypot(x, y); }

			constexpr bool isEqual(Coordinate2D coordinate, typename Real::MatchType matchType = Real::MatchAbsolute, Real tolerance = 1.0) const { return x.isEqual(coordinate.x, matchType, tolerance) && y.isEqual(coordinate.y, matchType, tolerance); }

			Real x;

			Real y;
		};
	}
}
