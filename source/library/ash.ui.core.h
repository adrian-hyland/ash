#pragma once

#include "ash.integer.h"
#include "ash.real.h"


namespace Ash
{
	namespace UI
	{
		struct Size
		{
			using Dimension = uint16_t;

			Dimension width;
			Dimension height;

			static constexpr Size minSize() { return Size(0, 0); }

			static constexpr Size maxSize() { return Size(std::numeric_limits<Dimension>::max(), std::numeric_limits<Dimension>::max()); }

			constexpr Size() : width(0), height(0) {}

			constexpr Size(Dimension widthValue, Dimension heightValue) : width(widthValue), height(heightValue) {}

			constexpr bool hasZeroArea() const { return (width == 0) || (height == 0); }

			constexpr bool operator == (const Size size) const { return (width == size.width) && (height == size.height); }

			constexpr bool operator != (const Size size) const { return !operator == (size); }

			constexpr Size operator + (Size size) const { return { Dimension(width + size.width), Dimension(height + size.height) }; }

			constexpr Size operator - (Size size) const { return { Dimension(width - size.width), Dimension(height - size.height) }; }

			constexpr Size operator * (Dimension value) const { return { Dimension(width * value), Dimension(height * value) }; }

			constexpr Size operator / (Dimension value) const { return { Dimension(width / value), Dimension(height / value) }; }

			template
			<
				typename PERCENT,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<PERCENT>, Ash::Type::Requirement::IsFloatingPoint<PERCENT>, Ash::Type::Requirement::IsClass<PERCENT, Ash::Generic::Real>>::IsValid
			>
			constexpr Size getPercent(PERCENT percent)
			{
				return { multiplyAndDivide(width, percent, 100), multiplyAndDivide(height, percent, 100) };
			}

		protected:
			template
			<
				typename MULTIPLIER,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<MULTIPLIER>, Ash::Type::Requirement::IsFloatingPoint<MULTIPLIER>, Ash::Type::Requirement::IsClass<MULTIPLIER, Ash::Generic::Real>>::IsValid
			>
			static constexpr Dimension multiplyAndDivide(Dimension value, MULTIPLIER multiplier, Dimension divisor)
			{
				if constexpr (Ash::Type::isInteger<MULTIPLIER>)
				{
					using Result = Ash::Integer::Unsigned::WithBitSize<Ash::Integer::bitSize<Dimension> * 2>::Type;

					if (multiplier != divisor)
					{
						value = (Result(value) * Result(multiplier) + Result(divisor) / 2) / Result(divisor);
					}

					return value;
				}
				else
				{
					return (value * multiplier + divisor / 2) / divisor;
				}
			}
		};

		struct Position
		{
			using Dimension = Ash::Integer::Value::WithBitSize<sizeof(Ash::UI::Size::Dimension) * 8 * 2, true>::Type;

			Dimension x;
			Dimension y;

			constexpr Position() : x(0), y(0) {}

			constexpr Position(Dimension xValue, Dimension yValue) : x(xValue), y(yValue) {}

			constexpr bool operator == (const Position position) const { return (x == position.x) && (y == position.y); }

			constexpr bool operator != (const Position position) const { return !operator == (position); }
		};

		struct Boundary
		{
			Ash::UI::Position position;
			Ash::UI::Size     size;

			constexpr Boundary() : position(), size() {}

			constexpr Boundary(Ash::UI::Position positionValue, Ash::UI::Size sizeValue) : position(positionValue), size(sizeValue) {}

			constexpr Boundary(Ash::UI::Position::Dimension x, Ash::UI::Position::Dimension y, Ash::UI::Size::Dimension width, Ash::UI::Size::Dimension height) : position(x, y), size(width, height) {}

			constexpr bool operator == (const Boundary &boundary) const { return (position == boundary.position) && (size == boundary.size); }

			constexpr bool operator != (const Boundary &boundary) const { return !operator == (boundary); }

			constexpr Boundary getUnion(const Boundary &boundary) const
			{
				Boundary result;

				if (position.x < boundary.position.x)
				{
					result.position.x = position.x;
					result.size.width = boundary.position.x + boundary.size.width - position.x;
				}
				else
				{
					result.position.x = boundary.position.x;
					result.size.width = position.x + size.width - boundary.position.x;
				}

				if (position.y < boundary.position.y)
				{
					result.position.y = position.y;
					result.size.height = boundary.position.y + boundary.size.height - position.y;
				}
				else
				{
					result.position.y = boundary.position.y;
					result.size.height = position.y + size.height - boundary.position.y;
				}

				return result;
			}

			constexpr Boundary getIntersection(const Boundary &boundary) const
			{
				Boundary result;

				if (position.x < boundary.position.x)
				{
					result.position.x = boundary.position.x;
					if (position.x + size.width > boundary.position.x)
					{
						result.size.width = position.x + size.width - boundary.position.x;
					}
					else
					{
						result.size.width = 0;
					}
				}
				else
				{
					result.position.x = position.x;
					if (boundary.position.x + boundary.size.width > position.x)
					{
						result.size.width = boundary.position.x + boundary.size.width - position.x;
					}
					else
					{
						result.size.width = 0;
					}
				}

				if (position.y < boundary.position.y)
				{
					result.position.y = boundary.position.y;
					if (position.y + size.height > boundary.position.y)
					{
						result.size.height = position.y + size.height - boundary.position.y;
					}
					else
					{
						result.size.height = 0;
					}
				}
				else
				{
					result.position.y = position.y;
					if (boundary.position.y + boundary.size.height > position.y)
					{
						result.size.height = boundary.position.y + boundary.size.height - position.y;
					}
					else
					{
						result.size.height = 0;
					}
				}

				return result;
			}
		};

		struct AspectRatio : public Ash::UI::Size
		{
			using Dimension = Ash::UI::Size::Dimension;

			constexpr AspectRatio() : Ash::UI::Size() {}

			constexpr AspectRatio(Dimension widthValue, Dimension heightValue) : Ash::UI::Size()
			{
				Dimension gcd = Ash::Integer::greatestCommonDivisor(widthValue, heightValue);
				if (gcd != 0)
				{
					width = widthValue / gcd;
					height = heightValue / gcd;
				}
			}

			constexpr AspectRatio(Ash::UI::Size size) : AspectRatio(size.width, size.height) {}

			constexpr bool isValid() const { return !hasZeroArea(); }

			template
			<
				typename FRACTION,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsFloatingPoint<FRACTION>, Ash::Type::Requirement::IsClass<FRACTION, Ash::Generic::Real>>::IsValid
			>
			constexpr FRACTION asFraction() const { return FRACTION(width) / FRACTION(height); }

			template
			<
				typename PERCENT = Dimension,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<PERCENT>, Ash::Type::Requirement::IsFloatingPoint<PERCENT>, Ash::Type::Requirement::IsClass<PERCENT, Ash::Generic::Real>>::IsValid
			>
			constexpr Ash::UI::Size withMaxSize(Ash::UI::Size maxSize, PERCENT percentage = 100) const
			{
				return (width * maxSize.height > height * maxSize.width) ? withMaxWidth(maxSize.width, percentage) : withMaxHeight(maxSize.height, percentage);
			}

			template
			<
				typename PERCENT = Dimension,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<PERCENT>, Ash::Type::Requirement::IsFloatingPoint<PERCENT>, Ash::Type::Requirement::IsClass<PERCENT, Ash::Generic::Real>>::IsValid
			>
			constexpr Ash::UI::Size withMaxWidth(Dimension maxWidth, PERCENT percentage = 100) const
			{
				Ash::UI::Size size;

				size.width = multiplyAndDivide(maxWidth, percentage, 100);
				size.width = size.width - size.width % width;
				size.height = multiplyAndDivide(size.width, height, width);

				return size;
			}

			template
			<
				typename PERCENT = Dimension,
				typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<PERCENT>, Ash::Type::Requirement::IsFloatingPoint<PERCENT>, Ash::Type::Requirement::IsClass<PERCENT, Ash::Generic::Real>>::IsValid
			>
			constexpr Ash::UI::Size withMaxHeight(Dimension maxHeight, PERCENT percentage = 100) const
			{
				Ash::UI::Size size;

				size.height = multiplyAndDivide(maxHeight, percentage, 100);
				size.height = size.height - size.height % height;
				size.width = multiplyAndDivide(size.height, width, height);

				return size;
			}
		};

		struct Frame
		{
			using Dimension = Ash::UI::Size::Dimension;

			Dimension left;
			Dimension right;
			Dimension top;
			Dimension bottom;

			constexpr Frame() : left(), right(), top(0), bottom(0) {}

			constexpr Frame(Dimension leftValue, Dimension rightValue, Dimension topValue, Dimension bottomValue) : left(leftValue), right(rightValue), top(topValue), bottom(bottomValue) {}

			constexpr Frame(Ash::UI::Size outsideSize, Ash::UI::Boundary insideBoundary) : left(0), right(0), top(0), bottom(0)
			{
				if (insideBoundary.position.x < 0)
				{
					insideBoundary.size.width = (-insideBoundary.position.x < insideBoundary.size.width) ? insideBoundary.size.width + insideBoundary.position.x : 0;
					insideBoundary.position.x = 0;
				}
				else if (insideBoundary.position.x >= outsideSize.width)
				{
					insideBoundary.size.width = 0;
					insideBoundary.position.x = outsideSize.width;
				}
				else if (insideBoundary.size.width > outsideSize.width - insideBoundary.position.x)
				{
					insideBoundary.size.width = outsideSize.width - insideBoundary.position.x;
				}

				left = insideBoundary.position.x;
				right = outsideSize.width - insideBoundary.size.width - left;

				if (insideBoundary.position.y < 0)
				{
					insideBoundary.size.height = (-insideBoundary.position.y < insideBoundary.size.height) ? insideBoundary.size.height + insideBoundary.position.y : 0;
					insideBoundary.position.y = 0;
				}
				else if (insideBoundary.position.y >= outsideSize.height)
				{
					insideBoundary.size.height = 0;
					insideBoundary.position.y = outsideSize.height;
				}
				else if (insideBoundary.size.height > outsideSize.height - insideBoundary.position.y)
				{
					insideBoundary.size.height = outsideSize.height - insideBoundary.position.y;
				}

				top = insideBoundary.position.y;
				bottom = outsideSize.height - insideBoundary.size.height - top;
			}

			constexpr operator Ash::UI::Size () { return { Ash::UI::Size::Dimension(left + right), Ash::UI::Size::Dimension(top + bottom) }; }
		};

		struct Strut
		{
			using Dimension = Ash::UI::Size::Dimension;

			Dimension start;
			Dimension end;
			Dimension size;

			constexpr Strut() : start(0), end(0), size(0) {}

			constexpr Strut(Dimension startValue, Dimension endValue, Dimension sizeValue) : start(startValue), end(endValue), size(sizeValue) {}
		};

		struct FrameStruts
		{
			Ash::UI::Strut left;
			Ash::UI::Strut right;
			Ash::UI::Strut top;
			Ash::UI::Strut bottom;

			constexpr FrameStruts() : left(), right(), top(), bottom() {}

			constexpr FrameStruts(Ash::UI::Strut leftValue, Ash::UI::Strut rightValue, Ash::UI::Strut topValue, Ash::UI::Strut bottomValue) : left(leftValue), right(rightValue), top(topValue) {}
		};
	}
}
