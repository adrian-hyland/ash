#pragma once

#include "ash.geometry.vector2d.h"


namespace Ash
{
	namespace Geometry
	{
		template <typename REAL>
		struct Point2D : Coordinate2D<REAL>
		{
			using Real = REAL;

			using Coordinate = Coordinate2D<Real>;

			using Vector = Vector2D<Real>;

			constexpr Point2D() : Coordinate(0, 0) {}

			constexpr Point2D(Real x, Real y) : Coordinate(x, y) {}

			constexpr Point2D(Coordinate coordinate) : Coordinate(coordinate) {}

			constexpr Point2D operator + (Vector vector) const { return { Coordinate::x + vector.x, Coordinate::y + vector.y }; }

			constexpr Vector operator - (Point2D point) const { return { Coordinate::x - point.x, Coordinate::y - point.y }; }

			constexpr Point2D operator - () const { return { -Coordinate::x, -Coordinate::y }; }

			static constexpr bool intersects(Point2D point1, Vector direction1, Point2D point2, Vector direction2, Real *distance1 = nullptr, Real *distance2 = nullptr)
			{
				return intersects(point1, direction1, point2, direction2, -Real::infinity, Real::infinity, -Real::infinity, Real::infinity, distance1, distance2);
			}

			static constexpr bool intersects(Point2D point1, Vector direction1, Point2D point2, Vector direction2, Real minDistance1, Real maxDistance1, Real *distance2 = nullptr)
			{
				return intersects(point1, direction1, point2, direction2, minDistance1, maxDistance1, -Real::infinity, Real::infinity, nullptr, distance2);
			}

			static constexpr bool intersects(Point2D point1, Vector direction1, Point2D point2, Vector direction2, Real minDistance1, Real maxDistance1, Real minDistance2, Real maxDistance2, Real *distance1 = nullptr, Real *distance2 = nullptr)
			{
				Real divisor = direction1.crossProduct(direction2);
				if (divisor == 0)
				{
					return false;
				}

				Real discard = 0;
				if (distance1 == nullptr)
				{
					distance1 = &discard;
				}
				*distance1 = direction2.crossProduct(point1 - point2) / divisor;
				if ((*distance1 < minDistance1) || (*distance1 > maxDistance1))
				{
					return false;
				}

				if (distance2 == nullptr)
				{
					distance2 = &discard;
				}
				*distance2 = (point2 - point1).crossProduct(direction1) / divisor;
				if ((*distance2 < minDistance2) || (*distance2 > maxDistance2))
				{
					return false;
				}

				return true;
			}
		};
	}
}
