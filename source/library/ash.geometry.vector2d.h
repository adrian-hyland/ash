#pragma once

#include "ash.type.h"
#include "ash.geometry.coordinate2d.h"


namespace Ash
{
	namespace Geometry
	{
		template
		<
			typename REAL,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		struct Vector2D : Ash::Geometry::Coordinate2D<REAL>
		{
			using Real = REAL;

			using Coordinate = Ash::Geometry::Coordinate2D<Real>;

			constexpr Vector2D() : Coordinate(0, 0) {}

			constexpr Vector2D(Real x, Real y) : Coordinate(x, y) {}

			constexpr Vector2D(Coordinate coordinate) : Coordinate(coordinate) {}

			template
			<
				typename ANGLE,
				typename = Ash::Type::IsClass<ANGLE, Ash::Geometry::Generic::Angle>
			>
			constexpr Vector2D(Real radius, ANGLE angle) : Coordinate(radius, angle) {}

			constexpr Vector2D operator + (Vector2D vector) const { return { Coordinate::x + vector.x, Coordinate::y + vector.y }; }

			constexpr Vector2D operator - (Vector2D vector) const { return { Coordinate::x - vector.x, Coordinate::y - vector.y }; }

			constexpr Vector2D operator * (Real scalar) const { return { Coordinate::x * scalar, Coordinate::y * scalar }; }

			constexpr Vector2D operator / (Real scalar) const { return { Coordinate::x / scalar, Coordinate::y / scalar }; }

			constexpr Vector2D operator - () const { return { -Coordinate::x, -Coordinate::y }; }

			constexpr Vector2D normalise() const { return *this / Coordinate::getRadius(); }

			constexpr Vector2D reflect(Vector2D vector) const { return (*this * 2 * dotProduct(vector) / dotProduct(*this)) - vector; }

			constexpr Real dotProduct(Vector2D vector) const { return Coordinate::x * vector.x + Coordinate::y * vector.y; }

			constexpr Real crossProduct(Vector2D vector) const { return Coordinate::x * vector.y - Coordinate::y * vector.x; }
		};
	}
}
