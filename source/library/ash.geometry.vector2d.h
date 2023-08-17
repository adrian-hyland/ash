#pragma once

#include "ash.geometry.coordinate2d.h"


namespace Ash
{
	namespace Geometry
	{
		template <typename REAL>
		struct Vector2D : Coordinate2D<REAL>
		{
			using Real = REAL;

			using Coordinate = Coordinate2D<Real>;

			constexpr Vector2D() : Coordinate(0, 0) {}

			constexpr Vector2D(Real x, Real y) : Coordinate(x, y) {}

			constexpr Vector2D(Coordinate coordinate) : Coordinate(coordinate) {}

			template <int PER_ROTATION>
			constexpr Vector2D(Real radius, Ash::Geometry::Angle<Real, PER_ROTATION> angle) : Coordinate(radius, angle) {}

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
