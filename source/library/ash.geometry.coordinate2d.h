#pragma once

#include "ash.type.h"
#include "ash.geometry.angle.h"


namespace Ash
{
	namespace Geometry
	{
		namespace Generic
		{
			struct Coordinate2D {};
		}
		
		template
		<
			typename REAL,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		struct Coordinate2D : Ash::Geometry::Generic::Coordinate2D
		{
			using Real = REAL;

			using MatchType = typename Real::MatchType;

			using Angle = Ash::Geometry::Radian<Real>;

			static constexpr Coordinate2D invalid = Coordinate2D(Real::invalid, Real::invalid);

			constexpr Coordinate2D() : x(Real(0.0)), y(Real(0.0)) {}

			constexpr Coordinate2D(Real xValue, Real yValue) : x(xValue), y(yValue) {}

			template
			<
				typename ANGLE,
				typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
			>
			constexpr Coordinate2D(Real radius, ANGLE angle) : x(radius * angle.cosine()), y(radius * angle.sine()) {}

			constexpr Angle getAngle() const { return Angle::arcTangent(y, x); }

			constexpr Real getRadius() const { return std::hypot(x, y); }

			constexpr bool isValid() const { return x.isValid() && y.isValid(); }

			constexpr bool isEqual(Coordinate2D coordinate, MatchType matchType = MatchType::MatchRelative, Real tolerance = 1.0) const { return x.isEqual(coordinate.x, matchType, tolerance) && y.isEqual(coordinate.y, matchType, tolerance); }

			template
			<
				typename ANGLE,
				typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
			>
			constexpr Coordinate2D rotate(ANGLE angle) const
			{
				return { x * angle.cosine() - y * angle.sine(), y * angle.cosine() + x * angle.sine() };
			}

			Real x;

			Real y;
		};
	}
}
