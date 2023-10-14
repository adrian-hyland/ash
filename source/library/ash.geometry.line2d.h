#pragma once

#include "ash.type.h"
#include "ash.geometry.point2d.h"


namespace Ash
{
	namespace Geometry
	{
		template
		<
			typename REAL,
			typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
		>
		struct Line2D
		{
			using Real = REAL;

			using Point = Ash::Geometry::Point2D<Real>;

			using Vector = Ash::Geometry::Vector2D<Real>;

			using Angle = typename Vector::Angle;

			static constexpr Line2D invalid() { return Line2D(Point::invalid(), Point::invalid()); }

			constexpr Line2D() : startPoint(), endPoint() {};

			constexpr Line2D(Point startPointValue, Point endPointValue) : startPoint(startPointValue), endPoint(endPointValue) {};

			constexpr bool isValid() const { return startPoint.isValid() && endPoint.isValid(); }

			constexpr Line2D operator + (Vector vector) const { return { startPoint + vector, endPoint + vector }; }

			constexpr Line2D operator - (Vector vector) const { return { startPoint - vector, endPoint - vector }; }

			constexpr operator Vector () const { return endPoint - startPoint; }

			constexpr Angle getAngle() const { return Vector(*this).getAngle(); }

			constexpr Real getLength() const { return Vector(*this).getRadius(); }

			constexpr Point getPoint(Real distance) const { return startPoint + Vector(*this) * distance; }

			constexpr bool intersects(Point point, Vector pointDirection, Real &pointDistance) const
			{
				return Point::intersects(startPoint, *this, point, pointDirection, 0, 1, &pointDistance);
			}

			constexpr bool collides(Vector lineDirection, Point point, Vector pointDirection, Real maxPointDistance, Real &lineDistance, Real &pointDistance) const
			{
				return Point::intersects(startPoint, *this, point, pointDirection - lineDirection, 0, 1, 0, maxPointDistance, &lineDistance, &pointDistance);
			}

			constexpr bool overlaps(Line2D line) const
			{
				return Point::intersects(startPoint, *this, line.startPoint, line, 0, 1, 0, 1);
			}

			Point startPoint;

			Point endPoint;
		};
	}
}
