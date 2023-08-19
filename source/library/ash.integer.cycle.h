#pragma once

#include <algorithm>
#include <limits>
#include "ash.type.h"


namespace Ash
{
	namespace Integer
	{
		namespace Generic
		{
			class Cycle {};
		}

		template
		<
			typename INTEGER,
			INTEGER  START,
			INTEGER  END = 0,
			typename = Ash::Type::IsInteger<INTEGER>
		>
		class Cycle : Ash::Integer::Generic::Cycle
		{
		public:
			using Type = INTEGER;

			static constexpr Type minValue = std::min(START, END);

			static constexpr Type maxValue = std::max(START, END);

			static_assert(Type(maxValue - minValue) <= std::numeric_limits<Type>::max() - 1, "Size of the integer cycle is too large");

			static constexpr Type size = maxValue - minValue + 1;

			constexpr Cycle() : m_Value(minValue) {}

			constexpr Cycle(const Cycle &cycle) : m_Value(cycle.m_Value) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Cycle(VALUE value) : m_Value(reduce(value)) {}

			constexpr operator Type () const { return m_Value; }

			constexpr Cycle inverse() const { return identity() - m_Value; }

			constexpr Cycle &operator ++ () { m_Value = (m_Value < maxValue) ? m_Value + 1 : minValue; return *this; }

			constexpr Cycle &operator -- () { m_Value = (m_Value > minValue) ? m_Value - 1 : maxValue; return *this; }

			constexpr Cycle operator ++ (int) { Cycle result = *this; ++(*this); return result; }

			constexpr Cycle operator -- (int) { Cycle result = *this; --(*this); return result; }

			static constexpr Cycle identity() { return reduce(size); }

			using Iterate = Type (*)(Type);

			template
			<
				Iterate ITERATE
			>
			class Range
			{
			public:
				constexpr Range(Cycle startValue, Cycle endValue) : m_Value(startValue), m_EndValue(endValue), m_IsAtEnd(false) {}

				constexpr const Cycle &operator * () const { return m_Value; }

				constexpr Range &operator ++ () { m_Value = ITERATE(m_Value); m_IsAtEnd = (m_Value == m_EndValue); return *this; }

				friend constexpr bool operator != (const Range &left, const Range &right) { return (left.m_Value != right.m_Value) || (left.m_IsAtEnd != right.m_IsAtEnd); };

				constexpr Range begin() const { return Range(m_Value, m_EndValue, false); }

				constexpr Range end() const { return Range(m_EndValue, m_EndValue, true); }

			protected:
				constexpr Range(Cycle startValue, Cycle endValue, bool isAtEnd) : m_Value(startValue), m_EndValue(endValue), m_IsAtEnd(isAtEnd) {}

			private:
				Cycle m_Value;
				Cycle m_EndValue;
				bool  m_IsAtEnd;
			};

			template
			<
				Iterate ITERATE
			>
			static constexpr Range<ITERATE> getRange(Cycle startValue = minValue) { return Range<ITERATE>(startValue, startValue); }

			template
			<
				Iterate ITERATE
			>
			static constexpr Range<ITERATE> getRange(Cycle startValue, Cycle endValue) { return Range<ITERATE>(startValue, ITERATE(endValue)); }

			static constexpr Type forward(Type value) { return value + 1; };

			static constexpr Range<forward> getRange(Cycle startValue = minValue) { return getRange<forward>(startValue); }

			static constexpr Range<forward> getRange(Cycle startValue, Cycle endValue) { return getRange<forward>(startValue, endValue); }

			static constexpr Type backward(Type value) { return value - 1; };

			static constexpr Range<backward> getRangeReversed(Cycle startValue = maxValue) { return getRange<backward>(startValue); }

			static constexpr Range<backward> getRangeReversed(Cycle startValue, Cycle endValue) { return getRange<backward>(startValue, endValue); }

		protected:
			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			static constexpr Type reduce(VALUE value)
			{
				if (value < minValue)
				{
					return Type(maxValue - (maxValue - value) % size);
				}
				else if (value > maxValue)
				{
					return Type(minValue + (value - minValue) % size);
				}
				else
				{
					return Type(value);
				}
			}

		private:
			Type m_Value;
		};
	}
}
