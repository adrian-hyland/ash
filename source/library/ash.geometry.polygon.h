#pragma once

#include "ash.type.h"
#include "ash.memory.h"
#include "ash.iterate.h"
#include "ash.geometry.point2d.h"
#include "ash.geometry.line2d.h"


namespace Ash
{
	namespace Geometry
	{
		namespace Polygon
		{
			enum Form
			{
				Closed,
				Open,
				Chain = Open
			};

			enum Overlap
			{
				None    = 0,
				Inside  = 1,
				Outside = 2,
			};

			template
			<
				typename ALLOCATION,
				typename REAL,
				Form     FORM,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			class Value;

			template
			<
				typename REAL,
				Form     FORM,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			using View = Value<Ash::Memory::Allocation::Reference<const Ash::Geometry::Point2D<REAL>>, REAL, FORM>;

			template
			<
				typename REAL,
				Form     FORM,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			using Area = Value<Ash::Memory::Allocation::Reference<Ash::Geometry::Point2D<REAL>>, REAL, FORM>;

			template
			<
				typename REAL,
				Form     FORM,
				size_t   CAPACITY,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			using Buffer = Value<Ash::Memory::Allocation::Buffer<Ash::Geometry::Point2D<REAL>, CAPACITY>, REAL, FORM>;

			template
			<
				typename REAL,
				Form     FORM,
				size_t   CAPACITY,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			using Sequence = Value<Ash::Memory::Allocation::Sequence<Ash::Geometry::Point2D<REAL>, CAPACITY>, REAL, FORM>;

			template
			<
				typename REAL,
				Form     FORM,
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			using Array = Value<Ash::Memory::Allocation::Array<Ash::Geometry::Point2D<REAL>, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>, REAL, FORM>;

			template
			<
				typename REAL,
				Form     FORM,
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			using ArrayBuffer = Value<Ash::Memory::Allocation::ArrayBuffer<Ash::Geometry::Point2D<REAL>, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>, REAL, FORM>;

			template
			<
				typename REAL,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			struct Edge
			{
				using Real = REAL;

				using Point = Ash::Geometry::Point2D<Real>;

				using Line = Ash::Geometry::Line2D<Real>;
				
				constexpr Edge() : m_StartPoint(&nullPoint), m_EndPoint(&nullPoint) {}

				constexpr Edge(const Point &start, const Point &end) : m_StartPoint(&start), m_EndPoint(&end) {}

				constexpr operator Line () const { return Line(*m_StartPoint, *m_EndPoint); }

			private:
				static constexpr Point nullPoint = Point::invalid();

				const Point *m_StartPoint;
				const Point *m_EndPoint;
			};

			template
			<
				typename REAL,
				typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
			>
			class EdgePoint : public Ash::Geometry::Polygon::Edge<REAL>
			{
			public:
				using Real = REAL;

				using Point = Ash::Geometry::Point2D<Real>;

				using Line = Ash::Geometry::Line2D<Real>;

				using Edge = Ash::Geometry::Polygon::Edge<Real>;

				constexpr EdgePoint() : Edge(), m_Distance(0) {}

				constexpr EdgePoint(const EdgePoint &edgePoint) : Edge(edgePoint), m_Distance(edgePoint.m_Distance) {}

				constexpr EdgePoint(const Point &startPoint, const Point &endPoint, Real distance) : Edge(startPoint, endPoint), m_Distance(distance) {}

				constexpr EdgePoint(const Edge &edge, Real distance) : Edge(edge), m_Distance(distance) {}

				constexpr operator Point () const { return Line(*this).getPoint(m_Distance); }

				constexpr Real getDistance() const { return m_Distance; }

			private:
				Real m_Distance;
			};

			template
			<
				typename ALLOCATION,
				typename REAL,
				Form     FORM,
				typename,
				typename
			>
			class Value : public Ash::Memory::Value<ALLOCATION, Ash::Geometry::Point2D<REAL>>
			{
			public:
				using Real = REAL;

				using Point = Ash::Geometry::Point2D<Real>;

				using Line = Ash::Geometry::Line2D<Real>;

				using Vector = Ash::Geometry::Vector2D<Real>;

				using Edge = Ash::Geometry::Polygon::Edge<Real>;

				using EdgePoint = Ash::Geometry::Polygon::EdgePoint<Real>;

				using Memory = Ash::Memory::Value<ALLOCATION, Point>;

				static constexpr Ash::Geometry::Polygon::Form form = FORM;

				static constexpr bool isClosed = (form == Ash::Geometry::Polygon::Form::Closed);

				template
				<
					bool IS_FORWARD = true
				>
				class IterateEdge
				{
				public:
					static constexpr bool isForward = IS_FORWARD;

					static constexpr bool isReverse = !isForward;

					using Iterate = Ash::Iterate<const Point *, isForward, isClosed ? 2 : 1>;

					using Reverse = IterateEdge<isReverse>;

					constexpr IterateEdge() : m_Start(), m_End() {}

					constexpr IterateEdge(const Iterate &start, const Iterate &end) : m_Start(start), m_End(end) {}

					static constexpr IterateEdge all(const Value &polygon)
					{
						if constexpr (polygon.isClosed)
						{
							if (polygon.getLength() < 2)
							{
								return IterateEdge();
							}
							else
							{
								return IterateEdge(polygon.cycleBetween(0, polygon.getLength() - 1), polygon.cycleBetween(1, 0));
							}
						}
						else
						{
							if (polygon.getLength() < 2)
							{
								return IterateEdge();
							}
							else
							{
								return IterateEdge(polygon.iterateTo(polygon.getLength() - 2), polygon.iterateFrom(1));
							}
						}
					}

					static constexpr IterateEdge between(const Value &polygon, size_t startFrom, size_t endTo)
					{
						if constexpr (polygon.isClosed)
						{
							if ((polygon.getLength() < 2) || (startFrom >= polygon.getLength()) || (endTo > polygon.getLength()))
							{
								return IterateEdge();
							}
							else
							{
								size_t startTo = (endTo == 0) ? polygon.getLength() - 1 : endTo - 1;
								size_t endFrom = (startFrom == polygon.getLength() - 1) ? 0 : startFrom + 1;
								return IterateEdge(polygon.cycleBetween(startFrom, startTo), polygon.cycleBetween(endFrom, endTo));
							}
						}
						else
						{
							if ((polygon.getLength() < 2) || (startFrom >= polygon.getLength() - 1) || (endTo >= polygon.getLength() - 1) || (endTo <= startFrom))
							{
								return IterateEdge();
							}
							else
							{
								return IterateEdge(polygon.iterateBetween(startFrom, endTo - 1), polygon.iterateBetween(startFrom + 1, endTo));
							}
						}
					}

					constexpr Reverse reverse() const
					{
						return Reverse(m_End.reverse(), m_Start.reverse());
					}

					constexpr Edge operator * () const { return Edge(*m_Start, *m_End); }

					constexpr IterateEdge &operator ++ ()
					{
						++m_Start;
						++m_End;
						return *this;
					}

					constexpr bool operator != (const IterateEdge &value) const
					{
						return (m_Start.operator != (value.m_Start)) && (m_End.operator != (value.m_End));
					}

					constexpr IterateEdge begin() const { return *this; }

					constexpr IterateEdge end() const { return *this; }

				private:
					Iterate m_Start;
					Iterate m_End;
				};

				constexpr Value(Form form = Form::Closed) : Memory() {}

				constexpr Value(std::initializer_list<Point> pointList) : Memory(pointList) {}

				constexpr Value(const Point *points, size_t pointCount) : Memory(points, pointCount) {}

				constexpr Value(const Value &value) : Memory(value) {}

				constexpr Value(Value &&value) noexcept : Memory(std::move(value)) {}

				constexpr Value &operator = (const Value &value)
				{
					Memory::operator = (value);
					return *this;
				}

				constexpr Value &operator = (Value &&value) noexcept
				{
					Memory::operator = (std::move(value));
					return *this;
				}

				constexpr operator Ash::Geometry::Polygon::View<Real, form> () const { return { Memory::at(0), Memory::getLength() }; }

				constexpr operator Ash::Geometry::Polygon::Area<Real, form> () { return { Memory::at(0), Memory::getLength() }; }

				constexpr size_t getEdgeCount() const
				{
					size_t length = Memory::getLength();
					if (length < 2)
					{
						return 0;
					}
					else if constexpr (isClosed)
					{
						return length;
					}
					else
					{
						return length - 1;
					}
				}

				constexpr IterateEdge<> iterateEdges() const
				{
					return IterateEdge<>::all(*this);
				}

				constexpr IterateEdge<> iterateEdgesFrom(size_t startFrom, size_t endTo) const
				{
					return IterateEdge<>::between(*this, startFrom, endTo);
				}

				constexpr Overlap overlaps(Line line, Vector relativePosition) const
				{
					Point linePoint;
					Vector lineDirection;
					Real distance;
					bool isBehind = false;
					bool isInFront = false;

					linePoint = line.startPoint + relativePosition;
					lineDirection = line;

					for (Line edge : iterateEdges())
					{
						if (edge.intersects(linePoint, lineDirection, distance))
						{
							if ((distance >= 0) && (distance <= 1))
							{
								return Overlap(Overlap::Inside | Overlap::Outside);
							}

							if constexpr (isClosed)
							{
								if (distance < 0)
								{
									isBehind = !isBehind;
								}
								else
								{
									isInFront = !isInFront;
								}
							}
						}
					}

					if constexpr (!isClosed)
					{
						return Overlap::None;
					}

					return (isBehind || isInFront) ? Overlap::Inside : Overlap::Outside;
				}

				template
				<
					typename VALUE_ALLOCATION,
					Form     VALUE_FORM,
					typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>
				>
				constexpr Overlap overlaps(const Value<VALUE_ALLOCATION, Real, VALUE_FORM> &polygon, Vector relativePosition) const
				{
					Overlap overlap = Overlap::None;

					if constexpr (!isClosed)
					{
						for (Line edge : iterateEdges())
						{
							overlap = Overlap(overlap | polygon.overlaps(edge, -relativePosition));
							if (overlap == Overlap(Overlap::Inside | Overlap::Outside))
							{
								return overlap;
							}
						}

						return (overlap == Overlap::Inside) ? Overlap::Outside : Overlap::None;
					}

					for (Line edge : polygon.iterateEdges())
					{
						overlap = Overlap(overlap | overlaps(edge, relativePosition));
						if (overlap == Overlap(Overlap::Inside | Overlap::Outside))
						{
							return overlap;
						}
					}

					if (overlap != Overlap::Outside)
					{
						return overlap;
					}

					if constexpr (VALUE_FORM != Ash::Geometry::Polygon::Form::Closed)
					{
						return Overlap::None;
					}

					for (Line edge : iterateEdges())
					{
						if (polygon.overlaps(edge, -relativePosition) != Overlap::Inside)
						{
							return Overlap::None;
						}
					}

					return Overlap::Outside;
				}

				bool collides(Vector direction, Point point, Vector pointDirection, Real maxDistance, EdgePoint &edgePoint, Real &pointDistance) const
				{
					Real distance;
					Real lineDistance;
					bool collision = false;

					pointDistance = maxDistance;
					for (Edge edge : iterateEdges())
					{
						if (Line(edge).collides(direction, point, pointDirection, pointDistance, lineDistance, distance))
						{
							collision = true;
							edgePoint = { edge, lineDistance };
							pointDistance = distance;
						}
					}

					return collision;
				}

				template
				<
					typename VALUE_ALLOCATION,
					Form     VALUE_FORM,
					typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>
				>
				bool collides(Vector direction, const Value<VALUE_ALLOCATION, Real, VALUE_FORM> &polygon, Vector polygonDirection, Vector relativePosition, Real maxDistance, EdgePoint &edgePoint, EdgePoint &polygonEdgePoint, Real &distance) const
				{
					EdgePoint collisionEdgePoint;
					Real collisionDistance;
					bool collision = false;

					distance = maxDistance;

					for (const Point &point : polygon.iterate())
					{
						if (collides(direction, point + relativePosition, polygonDirection, distance, collisionEdgePoint, collisionDistance))
						{
							collision = true;
							edgePoint = collisionEdgePoint;
							polygonEdgePoint = EdgePoint(point, point, 0);
							distance = collisionDistance;
						}
					}

					for (const Point &point : Memory::iterate())
					{
						if (polygon.collides(polygonDirection, point - relativePosition, direction, distance, collisionEdgePoint, collisionDistance))
						{
							collision = true;
							edgePoint = EdgePoint(point, point, 0);
							polygonEdgePoint = collisionEdgePoint;
							distance = collisionDistance;
						}
					}

					return collision;
				}
			};
		}
	}
}
