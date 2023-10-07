#pragma once

#include "ash.type.h"
#include "ash.iterate.h"
#include "ash.integer.value.h"


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
			auto     START,
			auto     END,
			typename = Ash::Type::IsInteger<decltype(START)>,
			typename = Ash::Type::IsInteger<decltype(END)>
		>
		class Cycle : Ash::Integer::Generic::Cycle
		{
		public:
			using Value = Ash::Integer::Value<START, END>;

			using Type = Ash::Type::Option<int, intmax_t, sizeof(Value) < sizeof(int)>;

			using Size = std::make_unsigned_t<Type>;

			using Iterate = Ash::Iterate<Value, true, 2>;

			static constexpr Value minimum = (START > END) ? END : START;

			static constexpr Value maximum = (START > END) ? START : END;

			static constexpr Size size = Size(maximum) - minimum + 1;

			constexpr Cycle() : m_Value(minimum) {}

			constexpr Cycle(const Cycle &cycle) : m_Value(cycle.m_Value) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Cycle(VALUE value) : m_Value(reduce(value)) {}

			constexpr operator Type () const { return m_Value; }

			constexpr Cycle inverse() const { return identity() - m_Value; }

			constexpr Cycle &operator ++ () { m_Value = (m_Value < maximum) ? m_Value + 1 : minimum; return *this; }

			constexpr Cycle &operator -- () { m_Value = (m_Value > minimum) ? m_Value - 1 : maximum; return *this; }

			constexpr Cycle operator ++ (int) { Cycle result = *this; ++(*this); return result; }

			constexpr Cycle operator -- (int) { Cycle result = *this; --(*this); return result; }

			static constexpr Cycle identity() { return reduce(Type(size)); }

			static constexpr Iterate iterate() { return iterateFrom(minimum); }

			static constexpr Iterate iterateFrom(Cycle from)
			{
				return Ash::Iterate<Value>::between(from, maximum) + Ash::Iterate<Value>::from(minimum, from - minimum);
			}

			static constexpr Iterate iterateBetween(Cycle from, Cycle to)
			{
				if (from < to)
				{
					return Ash::Iterate<Value>::between(from, to) + Ash::Iterate<Value>();
				}
				else
				{
					return Ash::Iterate<Value>::between(from, maximum) + Ash::Iterate<Value>::between(minimum, to);
				}
			}

		protected:
			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			static constexpr Value reduce(VALUE value)
			{
				if (value < Type(minimum))
				{
					return Value(Type(maximum) - (Type(maximum) - value) % size);
				}
				else if (value > Type(maximum))
				{
					return Value(Type(minimum) + (value - Type(minimum)) % size);
				}
				
				return Value(value);
			}

		private:
			Value m_Value;
		};
	}
}
