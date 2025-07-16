#pragma once

#include "ash.type.h"
#include "ash.iterate.h"
#include "ash.integer.value.h"


namespace Ash::Integer::Generic
{
	/**
	 * @brief  Represents a generic `Cycle` class
	 */
	class Cycle {};
}

namespace Ash::Integer
{
	/**
	 * @brief  A class for representing an integer value that cycles within a specified range.
	 * @tparam START The lower bound of the cycle range.
	 * @tparam END   The upper bound of the cycle range.
	 * @note   The `START` and `END` values are inclusive.
	 */
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
		/**
		 * @brief The type used for representing the cycle value
		 */
		using Value = typename Ash::Integer::Value::WithRange<START, END>::Type;

		/**
		 * @brief The type used for representing the integer value
		 */
		using Type = Ash::Type::Option<int, intmax_t, sizeof(Value) < sizeof(int)>;

		/**
		 * @brief The type used for representing the cycle size
		 */
		using Size = std::make_unsigned_t<Type>;

		/**
		 * @brief The type used for iterating through the cycle
		 */
		using Iterate = Ash::Iterate<Value, true, 2>;

		/**
		 * @brief The minimum value of the range
		 */
		static constexpr Value minimum = (START > END) ? END : START;

		/**
		 * @brief The maximum value of the range
		 */
		static constexpr Value maximum = (START > END) ? START : END;

		/**
		 * @brief The size of the range
		 */
		static constexpr Size size = Size(maximum) - minimum + 1;

		/**
		 * @brief Default constructor
		 * @note  - This creates a `Cycle` object with its value set to `minimum`
		 */
		constexpr Cycle() : m_Value(minimum) {}

		/**
		 * @brief Copy constructor
		 * @param cycle The cycle object to copy
		 */
		constexpr Cycle(const Cycle &cycle) : m_Value(cycle.m_Value) {}

		/**
		 * @brief  Constructs a `Cycle` object with a specified value
		 * @tparam VALUE The type of the value
		 * @param  value The value to initialize `this` with
		 * @note   - The value will be 'reduced' so that it is within the range [`minimum`, `maximum`]. 
		 */
		template
		<
			typename VALUE,
			typename = Ash::Type::IsInteger<VALUE>
		>
		constexpr Cycle(VALUE value) : m_Value(reduce(value)) {}

		/**
		 * @brief  Implicitly converts `this` to its underlying `Type`
		 * @return The value of `this` converted to a `Type`
		 */
		constexpr operator Type () const { return m_Value; }

		/**
		 * @brief  Gets the (arithmertic) inverse value 
		 * @return The inverse value
		 * @note   - The inverse value is the value within the range [`minimum`, `maximum`] that represents the effective 'negative' value.
		 *           I.e. `n + n.inverse() == identity()`
		 */
		constexpr Cycle inverse() const { return identity() - m_Value; }

		/**
		 * @brief  Increments the value of `this`. The value wraps around to the `minimum` value if goes above the `maximum` value.
		 * @return A reference to `this`
		 */
		constexpr Cycle &operator ++ () { m_Value = (m_Value < maximum) ? m_Value + 1 : minimum; return *this; }

		/**
		 * @brief  Decrements the value of `this`. The value wraps around to the `maximum` value if goes below the `minimum` value.
		 * @return A reference to `this`
		 */
		constexpr Cycle &operator -- () { m_Value = (m_Value > minimum) ? m_Value - 1 : maximum; return *this; }

		/**
		 * @brief  Increments the value of `this`. The value wraps around to the `minimum` value if goes above the `maximum` value.
		 * @return A `Cycle` value that contains a copy the original value
		 */
		constexpr Cycle operator ++ (int) { Cycle result = *this; ++(*this); return result; }

		/**
		 * @brief  Decrements the value of `this`. The value wraps around to the `maximum` value if goes below the `minimum` value.
		 * @return A `Cycle` value that contains a copy the original value
		 */
		constexpr Cycle operator -- (int) { Cycle result = *this; --(*this); return result; }

		/**
		 * @brief  Gets the (arithmertic) identity value 
		 * @return The identity value
		 * @note   - The identity value is the value within the range [`minimum`, `maximum`] that represents the effective arithmetic 'zero' value.
		 *           I.e. `n + identity() == n`
		 */
		static constexpr Cycle identity() { return reduce(Type(size)); }

		/**
		 * @brief  Gets an 'Iterate' value for creating an integer sequence within the range [`minimum`, `maximum`]
		 * @return The `Iterate` value for creating an integer sequence covering all values in the cycle
		 * @note   - The returned `Iterate` value can be used in a standard 'for-range' loop.
		 * @note   - Call the `reverse()` method on the return value to iterate through the sequence in reverse order.
		 */
		static constexpr Iterate iterate() { return iterateFrom(minimum); }

		/**
		 * @brief  Gets an `Iterate` value for creating an integer sequence cycling within the range [`minimum`, `maximum`] starting from a specified cycle value.
		 * @param  from The cycle value to start from
		 * @return The `Iterate` value for creating an integer sequence covering all values in the cycle, starting from the value `from`.
		 * @note   - The iteration sequence will wrap around to the `minimum` value if goes above the `maximum` value.
		 * @note   - The returned `Iterate` value can be used in a standard 'for-range' loop.
		 * @note   - Call the `reverse()` method on the return value to iterate through the sequence in reverse order.
		 */
		static constexpr Iterate iterateFrom(Cycle from)
		{
			return Ash::Iterate<Value>::between(from, maximum) + Ash::Iterate<Value>::from(minimum, from - minimum);
		}

		/**
		 * @brief  Gets an `Iterate` value for creating an integer sequence between 2 specified values, cycling within the range [`minimum`, `maximum`]
		 * @param  from The cycle value to start from
		 * @param  to   The cycle value to end on
		 * @return The `Iterate` value for creating an integer sequence between the values `from` and `to` cycling through the range [`minimum`, `maximum`]
		 * @note   - The `from` and `to` parameter values are inclusive.
		 * @note   - The iteration sequence will wrap around to the `minimum` value if goes above the `maximum` value (when `from` is greater then `to`)
		 * @note   - The returned `Iterate` value can be used in a standard 'for-range' loop.
		 * @note   - Call the `reverse()` method on the return value to iterate through the sequence in reverse order.
		 */
		static constexpr Iterate iterateBetween(Cycle from, Cycle to)
		{
			if (from <= to)
			{
				return Ash::Iterate<Value>::between(from, to) + Ash::Iterate<Value>();
			}
			else
			{
				return Ash::Iterate<Value>::between(from, maximum) + Ash::Iterate<Value>::between(minimum, to);
			}
		}

	protected:
		/**
		 * @brief  Reduces a value to so that it has a value that is within the cyclic range [`minimum`, `maximum`]
		 * @tparam VALUE The integer type of the value
		 * @param  value The value to reduce
		 * @return The reduced value
		 */
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
