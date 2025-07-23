#pragma once

#include <limits>
#include <cstddef>
#include <cstdint>
#include "ash.type.h"


namespace Ash::Generic
{
	/**
	 * @brief  Represents a generic `Iterate` class
	 */
	class Iterate {};
}

namespace Ash
{
	/**
	 * @class A class that combines one or more iterations over a range of integer or pointer values
	 * @tparam TYPE        The type of the value to iterate over (this can either be an integer or a pointer type).
	 * @tparam IS_FORWARD  Indicates the direction of iteration. If true, iteration is forward; if false, iteration is backward.
	 * @tparam DEPTH       The recursion depth for nested iterations.
	 */
	template
	<
		typename TYPE,
		bool     IS_FORWARD = true,
		size_t   DEPTH = 1,
		typename = typename Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsPointer<TYPE>>::IsValid
	>
	class Iterate : public Iterate<TYPE, IS_FORWARD, DEPTH - 1>
	{
	public:
		/**
		 * @brief The type used by the iteration
		 */
		using Type = TYPE;

		/**
		 * @brief The reference type used by the iteration
		 */
		using ReferenceType = std::add_lvalue_reference_t<std::remove_pointer_t<Type>>;

		/**
		 * @brief Indicates whether the iteration has a forward direction
		 */
		static constexpr bool isForward = IS_FORWARD;

		/**
		 * @brief Indicates whether the iteration has a reverse direction
		 */
		static constexpr bool isReverse = !isForward;

		/**
		 * @brief The recursion depth of `this` iteration
		 */
		static constexpr size_t depth = DEPTH;

		/**
		 * @brief Type alias for the last iteration
		 */
		using Last = Iterate<Type, isForward, 0>;

		/**
		 * @brief Type alias for the next iteration after `this`
		 */
		using Next = Iterate<Type, isForward, depth - 1>;

		/**
		 * @brief Type alias for an initial iteration
		 */
		using Initial = Iterate<Type, isForward, 1>;

		/**
		 * @brief Type alias for the reverse iteration of `this`
		 */
		using Reverse = Iterate<Type, isReverse, depth>;

		/**
		 * @brief Indicates whether there is another iteration after `this`
		 */
		static constexpr bool hasNext = Ash::Type::isNotSame<Next, Last>;

		/**
		 * @brief Default constructor
		 * @note  - This will create an `Iterate` object with all of its iterations complete.
		 */
		constexpr Iterate() : Next(), m_Start(), m_End(), m_IsComplete(true) {}

		/**
		 * @brief Copy constructor
		 * @param iterate The iterate object to copy.
		 */
		constexpr Iterate(const Iterate &iterate) : Next(iterate), m_Start(iterate.m_Start), m_End(iterate.m_End), m_IsComplete(iterate.m_IsComplete) {}

		/**
		 * @brief  Creates an initial `Iterate` object for iterating between two values
		 * @param  start The start value of the iteration
		 * @param  end   The end value of the iteration
		 * @return An `Iterate` object.
		 * @note   - The `start` and `end` values are inclusive.
		 * @note   - The returned value can be used in range-based for loops.
		 * @note   - Call the `reverse` method on the return value to iterate over the range in the reverse order.
		 * @note   - The returned `Iterate` object will be marked as complete if
		 * @note      -- the class has a forward direction and `start` is greater than `end`
		 * @note      -- or the class has a reverse direction and `start` is less than `end`
		 * @note      -- or the class has a pointer `Type` and either `start` or `end` is a `nullptr`.
		 */
		static constexpr Initial between(Type start, Type end)
		{
			if constexpr (isForward)
			{
				return { start, end, start > end };
			}
			else
			{
				return { start, end, start < end };
			}
		}

		/**
		 * @brief  Creates an initial `Iterate` object for iterating a number of times from a starting value
		 * @param  start The value to start iterating from
		 * @param  count The iteration count
		 * @return The `Iterate` object.
		 * @note   - The returned `Iterate` value can be used in range-based for loops.
		 * @note   - Call the `reverse()` method on the return value to iterate over the range in the reverse order.
		 * @note   - The returned `Iterate` object will be marked as complete if
		 * @note      -- the class has a pointer `Type` and the range of iteration values contain a `nullptr`
		 * @note      -- or the value of `count` is zero.
		 * @note   - If the class has an integer `Type` then the value of `count` will be adjusted to prevent any
		 *           overflow occuring during the iteration.
		 */
		static constexpr Initial from(Type start, size_t count)
		{
			if (count-- == 0)
			{
				return Initial();
			}
			else if constexpr (Ash::Type::isInteger<Type>)
			{
				Type end;
				if constexpr (isForward)
				{
					if (__builtin_add_overflow(start, count, &end))
					{
						end = std::numeric_limits<Type>::max();
					}
				}
				else
				{
					if (__builtin_sub_overflow(start, count, &end))
					{
						end = std::numeric_limits<Type>::min();
					}
				}
				return Initial(start, end);
			}
			else if constexpr (isForward)
			{
				return between(start, start + count);
			}
			else
			{
				return between(start, start - count);
			}
		}

		/**
		 * @brief  Gets the iterations of `this` in reverse
		 * @return An `Iterate` object that has all of the iterations of `this` in reverse
		 * @note   - The `start` and `end` values of each iteration will be reversed as well as the direction.
		 * @note   - The order of each iteration will also be reversed.
		 * @note     Any completed iteration will still remain at the end of the reversed object however.
		 */
		constexpr Reverse reverse() const
		{
			if constexpr (hasNext)
			{
				return Next(*this).reverse() + typename Initial::Reverse(m_End, m_Start, m_IsComplete);
			}
			else
			{
				return typename Initial::Reverse(m_End, m_Start, m_IsComplete);
			}
		}

		/**
		 * @brief  Appends another `Iterate` object to `this`
		 * @tparam ITERATE_DEPTH The recursive depth of the `Iterate` object that is being appended to `this`
		 * @param  iterate The `Iterate` to append
		 * @return An `Iterate` object that has the iteration of `iterate` appended to `this`
		 * @note   - The parameter `iterate` must have the same type and direction as `this`.
		 * @note   - Any iterations that have been completed (either in `this` or in `iterate`) will be placed at the end of the returned object.
		 */
		template
		<
			size_t ITERATE_DEPTH
		>
		constexpr Iterate<Type, isForward, depth + ITERATE_DEPTH> operator + (const Iterate<Type, isForward, ITERATE_DEPTH> &iterate) const
		{
			if (!m_IsComplete || iterate.m_IsComplete)
			{
				if constexpr (hasNext)
				{
					return { Next(*this) + iterate, m_Start, m_End, m_IsComplete };
				}
				else
				{
					return { iterate, m_Start, m_End, m_IsComplete };
				}
			}
			else
			{
				if constexpr (ITERATE_DEPTH > 1)
				{
					return { Iterate<Type, isForward, ITERATE_DEPTH - 1>(iterate) + *this, iterate.m_Start, iterate.m_End, iterate.m_IsComplete };
				}
				else
				{
					return { *this, iterate.m_Start, iterate.m_End, iterate.m_IsComplete };
				}
			}
		}

		/**
		 * @brief  Checks whether the all of the iterations are complete
		 * @return `true` is returned if all of the iterations are complete.
		 * @return `false` is returned if any of the iterations are not yet complete.
		 */
		constexpr bool isComplete() const { return m_IsComplete; }

		/**
		 * @brief  Gets the value of the current iteration (in a range of integers)
		 * @tparam RETURN_TYPE The return type
		 * @return The current iteration value.
		 * @note   - This forms part of the interface that allows `this` to be used in a range based 'for' loop.
		 */
		template
		<
			typename RETURN_TYPE = Type,
			typename = Ash::Type::IsInteger<RETURN_TYPE>
		>
		constexpr Type operator * () const { return m_Start; }

		/**
		 * @brief  Gets a reference to the value that the current iteration points to (in a range of pointers)
		 * @tparam RETURN_TYPE The return type
		 * @return The reference to the value that current iteration points to.
		 * @note   - This forms part of the interface that allows `this` to be used in a range based 'for' loop.
		 */
		template
		<
			typename RETURN_TYPE = Type,
			typename = Ash::Type::IsPointer<RETURN_TYPE>
		>
		constexpr ReferenceType operator * () const { return *m_Start; }

		/**
		 * @brief  Advances the value of of the current iteration
		 * @return A reference to `this`
		 * @note   - If the iteration has a forward direction then its value is incremented; otherwise it is decremented
		 * @note   - The next iteration is started once the current iteration is completed
		 * @note   - This forms part of the interface that allows `this` to be used in a range based 'for' loop.
		 */
		constexpr Iterate &operator ++ ()
		{
			if (m_IsComplete || (m_Start == m_End))
			{
				setNext();
			}
			else
			{
				if constexpr (isForward)
				{
					++m_Start;
				}
				else
				{
					--m_Start;
				}
			}

			return *this;
		}

		/**
		 * @brief  Checks if all of the iterations are complete
		 * @return 'true' is returned if not all of the iterations are complete.
		 * @return `false` is returned if all of the iterations are complete.
		 * @note   - This forms part of the interface that allows `this` to be used in a range based 'for' loop.
		 */
		constexpr bool operator != (const Iterate &value) const
		{
			(void)value;
			return !m_IsComplete;
		}

		/**
		 * @brief  Returns an `Iterate` object used for the start of the iterations
		 * @return The start of the iterations
		 * @note   - This forms part of the interface that allows `this` to be used in a range based 'for' loop.
		 */
		constexpr Iterate begin() const { return *this; }

		/**
		 * @brief  Returns an `Iterate` object used for the end of the iterations
		 * @return The end of the iterations
		 * @note   - This forms part of the interface that allows `this` to be used in a range based 'for' loop.
		 */
		constexpr Iterate end() const { return *this; }

	protected:
		/**
		 * @brief  Constructs an `Iterate` object for iterating over a range of integer or pointer values
		 * @param  start      The start value of the iteration
		 * @param  end        The end value of the iteration
		 * @param  isComplete Indicates whether the iteration should be marked as complete
		 * @note   - The iteration will also be marked as complete if the iteration is for a range of pointers and either `start` or `end` is null
		 */
		constexpr Iterate(Type start, Type end, bool isComplete = false) : Next(), m_Start(start), m_End(end), m_IsComplete(isComplete)
		{
			if constexpr (Ash::Type::isPointer<Type>)
			{
				m_IsComplete = m_IsComplete || (m_Start == nullptr) || (m_End == nullptr);
			}
		}

		/**
		 * @brief  Constructs an `Iterate` object for iterating over a range of integer or pointer values
		 * @param  next       The next iteration
		 * @param  start      The start value of the iteration
		 * @param  end        The end value of the iteration
		 * @param  isComplete Indicates whether the iteration should be marked as complete
		 * @note   - The iteration will also be marked as complete if the iteration is for a range of pointers and either `start` or `end` is null
		 */
		constexpr Iterate(const Next &next, Type start, Type end, bool isComplete = false) : Next(next), m_Start(start), m_End(end), m_IsComplete(isComplete)
		{
			if constexpr (Ash::Type::isPointer<Type>)
			{
				m_IsComplete = m_IsComplete || (m_Start == nullptr) || (m_End == nullptr);
			}
		}

		/**
		 * @brief Sets `this` to the value of the next iteration
		 * @note  - This method is called recursively - with the next iteration set to the value of the subsequent iteration (and so on and so forth..)
		 * @note  - If `this` does not have a `Next` iteration then it will be marked as complete
		 */
		constexpr void setNext()
		{
			if constexpr(hasNext)
			{
				m_Start = Next::m_Start;
				m_End = Next::m_End;
				m_IsComplete = Next::m_IsComplete;
				Next::setNext();
			}
			else
			{
				m_Start = Type();
				m_End = Type();
				m_IsComplete = true;
			}
		}

	private:
		Type m_Start;
		Type m_End;
		bool m_IsComplete;

		template
		<
			typename FRIEND_TYPE,
			bool     FRIEND_IS_FORWARD,
			size_t   FRIEND_DEPTH,
			typename
		>
		friend class Iterate;
	};

	/**
	 * @brief  A class for Defining the last object of the recursive `Iterate` class
	 * @tparam TYPE       The type of the value to iterate over (this can either be an integer or a pointer type).
	 * @tparam IS_FORWARD Indicates the direction of iteration. If true, iteration is forward; if false, iteration is backward.
	 */
	template
	<
		typename TYPE,
		bool     IS_FORWARD
	>
	class Iterate<TYPE, IS_FORWARD, 0> : Generic::Iterate {};
}
