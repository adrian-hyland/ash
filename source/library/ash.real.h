#pragma once

#include <cmath>
#include <limits>
#include "ash.type.h"
#include "ash.integer.bit.h"


namespace Ash
{
	namespace Generic
	{
		class Real {};
	}

	template
	<
		typename FLOAT,
		typename = Ash::Type::IsFloatingPoint<FLOAT>
	>
	class Real : Ash::Generic::Real
	{
	public:
		using Type = FLOAT;

		using Exponent = int;

		using Size = int;

		static constexpr Exponent minExponent = std::numeric_limits<Type>::min_exponent - 1;

		static constexpr Exponent maxExponent = std::numeric_limits<Type>::max_exponent - 1;

		static constexpr Exponent minSubNormalExponent = std::numeric_limits<Type>::min_exponent - std::numeric_limits<Type>::digits;

		static constexpr Size fractionSize = std::numeric_limits<Type>::digits - 1;

		static constexpr Size exponentSize = Ash::Integer::getBitSize<unsigned int>(maxExponent - minExponent);

		static constexpr Type epsilon = std::numeric_limits<Type>::epsilon();

		static constexpr Type infinity = std::numeric_limits<Type>::infinity();

		static constexpr Type invalid = std::numeric_limits<Type>::quiet_NaN();

		static constexpr Type min = std::numeric_limits<Type>::min();

		static constexpr Type max = std::numeric_limits<Type>::max();

		static constexpr Type minSubNormal = std::numeric_limits<Type>::denorm_min();

		static constexpr Type pi = 3.1415926535897932384626433832795029L;

		constexpr Real() : m_Value(0.0) {}

		constexpr Real(const Real &real) : m_Value(real.m_Value) {}

		template
		<
			typename VALUE,
			typename = Ash::Type::IsNumeric<VALUE>
		>
		constexpr Real(VALUE value) : m_Value(value) {}

		constexpr Real(const Real &real, Exponent exponent) : m_Value(std::ldexp(real.m_Value, exponent)) {}

		template
		<
			typename VALUE,
			typename = Ash::Type::IsNumeric<VALUE>
		>
		constexpr Real(VALUE value, Exponent exponent) : m_Value(std::ldexp(Type(value), exponent)) {}

		constexpr operator Type () const { return m_Value; }

		constexpr bool isValid() const { return !std::isnan(m_Value); }

		constexpr bool isInfinite() const { return std::isinf(m_Value); }

		constexpr bool isNegative() const { return std::signbit(m_Value); }

		constexpr bool isPositive() const { return !isNegative(); }

		constexpr Real negate() const { return -m_Value; }

		constexpr Real getAbsolute() const { return std::fabs(m_Value); }

		enum MatchType
		{
			MatchAbsolute,
			MatchRelative
		};

		constexpr Real match(Real value, MatchType matchType = MatchAbsolute) const
		{
			return (matchType == MatchAbsolute) ? matchAbsolute(value) : matchRelative(value);
		}

		constexpr bool isEqual(Real value, MatchType matchType = MatchAbsolute, Real tolerance = 1.0) const
		{
			Real error = match(value, matchType);
			
			return error.isValid() && (error <= tolerance);
		}

		constexpr Real square() const { return m_Value * m_Value; }

		constexpr Real squareRoot() const { return std::sqrt(m_Value); }

		constexpr Real modulus(Real n) const { return std::fmod(m_Value, n); }

		constexpr Real reciprocal() const { return 1 / m_Value; }

	protected:
		constexpr Real matchAbsolute(Real value) const
		{
			if (m_Value == value)
			{
				return 0.0;
			}
			else if (!isValid() || !value.isValid() || isInfinite() || value.isInfinite())
			{
				return invalid;
			}
			else if ((getAbsolute() < min) && (value.getAbsolute() < min))
			{
				return Real(m_Value - value).getAbsolute() / minSubNormal / 2;
			}
			else if (isPositive() ? value.isNegative() : value.isPositive())
			{
				return invalid;
			}
			else
			{
				Exponent thisExponent = 0;
				Exponent valueExponent = 0;
				Exponent exponent = 0;

				Real thisFraction = std::frexp(m_Value, &thisExponent);
				Real valueFraction = std::frexp(value, &valueExponent);
				Real fraction = 0;

				if (thisExponent == valueExponent + 1)
				{
					thisFraction = thisFraction * 2;
					thisExponent = thisExponent - 1;
				}
				else if (thisExponent + 1 == valueExponent)
				{
					valueFraction = valueFraction * 2;
				}
				else if (thisExponent != valueExponent)
				{
					return invalid;
				}

				fraction = thisFraction - valueFraction;
				exponent = (thisExponent <= minExponent) ? fractionSize - minExponent + thisExponent - 1 : fractionSize;

				return { fraction.getAbsolute(), exponent };
			}
		}

		constexpr Real matchRelative(Real value) const
		{
			if (m_Value == value)
			{
				return 0.0;
			}
			else if (!isValid() || !value.isValid() || isInfinite() || value.isInfinite())
			{
				return invalid;
			}
			else if ((getAbsolute() < 1) && (value.getAbsolute() < 1))
			{
				return Real(m_Value - value).getAbsolute() / epsilon / 2;
			}
			else if (isPositive() ? value.isNegative() : value.isPositive())
			{
				return invalid;
			}
			else if (getAbsolute() > value.getAbsolute())
			{
				return (Real(1.0) - value / m_Value) / epsilon;
			}
			else
			{
				return (Real(1.0) - m_Value / value) / epsilon;
			}
		}

	private:
		Type m_Value;
	};

	using Float = Real<float>;

	using Double = Real<double>;

	using LongDouble = Real<long double>;
}
