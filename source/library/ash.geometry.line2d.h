#pragma once

#include "ash.geometry.point2d.h"


namespace Ash
{
	namespace Geometry
	{
		template <typename REAL>
		struct Line2D
		{
			using Real = REAL;

			using Point = Ash::Geometry::Point2D<Real>;

			using Vector = Ash::Geometry::Vector2D<Real>;

			constexpr Line2D() : startPoint(), endPoint() {};

			constexpr Line2D(Point startPointValue, Point endPointValue) : startPoint(startPointValue), endPoint(endPointValue) {};

			constexpr Line2D operator + (Vector vector) const { return { startPoint + vector, endPoint + vector }; }

			constexpr Line2D operator - (Vector vector) const { return { startPoint - vector, endPoint - vector }; }

			constexpr bool intersects(Point point, Vector direction, Point &intersection, Real &distance) const
			{
				if (!Point::intersects(startPoint, endPoint - startPoint, point, direction, 0, 1, &distance))
				{
					return false;
				}
				
				intersection = point + direction * distance;

				return true;
			}

			constexpr bool collides(Vector lineDirection, Point point, Vector pointDirection, Real maxPointDistance, Point &linePoint, Point &collisionPoint, Real &pointDistance) const
			{
				Real lineDistance;

				if (!Point::intersects(startPoint, endPoint - startPoint, point, pointDirection - lineDirection, 0, 1, 0, maxPointDistance, &lineDistance, &pointDistance))
				{
					return false;
				}

				linePoint = startPoint + (endPoint - startPoint) * lineDistance;
				collisionPoint = point + pointDirection * pointDistance;

				return true;
			}

			constexpr bool overlaps(Line2D line) const
			{
				return Point::intersects(startPoint, endPoint - startPoint, line.startPoint, line.endPoint - line.startPoint, 0, 1, 0, 1);
			}

			Point startPoint;

			Point endPoint;
		};
	}
}
