#pragma once

#include <limits>
#include <cstddef>
#include "ash.type.h"


namespace Ash
{
	namespace Generic
	{
		class Iterate {};
	}

	template
	<
		typename TYPE,
		bool     IS_FORWARD = true,
		typename = Ash::Type::IsIntegerOrPointer<TYPE>
	>
	class Iteration
	{
	public:
		using Type = TYPE;

		static constexpr bool isForward = IS_FORWARD;

		static constexpr bool isReverse = !isForward;

		using Reverse = Iteration<Type, isReverse>;

		constexpr Iteration() : m_Value(), m_Count(0) {}

		constexpr Iteration(Type value, size_t count) : m_Value(value), m_Count(count) {}

		constexpr operator Type () const { return m_Value; }

		constexpr size_t getCount() const { return m_Count; }

		constexpr bool isComplete() const { return m_Count == 0; }

		constexpr bool advance()
		{
			if (m_Count != 0)
			{
				if constexpr (isForward)
				{
					m_Value++;
				}
				else
				{
					m_Value--;
				}

				m_Count--;
			}

			return !isComplete();
		}

		constexpr Reverse reverse() const
		{
			if constexpr (isForward)
			{
				return Reverse(Type(m_Value + m_Count - 1), m_Count);
			}
			else
			{
				return Reverse(Type(m_Value - m_Count + 1), m_Count);
			}
		}

		static constexpr Iteration from(Type value, size_t count)
		{
			if constexpr (Ash::Type::isInteger<Type> && isForward)
			{
				size_t max = size_t(std::numeric_limits<Type>::max()) - value;
				if ((count > 0) && (count - 1 > max))
				{
					count = max + 1;
				}
			}

			if constexpr (Ash::Type::isInteger<Type> && isReverse)
			{
				size_t max = (std::numeric_limits<Type>::min() < 0) ? size_t(-(std::numeric_limits<Type>::min() + 1)) + value + 1 : value;
				if ((count > 0) & (count - 1 > max))
				{
					count = max + 1;
				}
			}

			return { value, count };
		}

	private:
		Type   m_Value;
		size_t m_Count;
	};

	template
	<
		typename TYPE,
		bool     IS_FORWARD = true,
		size_t   DEPTH = 1,
		typename = Ash::Type::IsIntegerOrPointer<TYPE>
	>
	class Iterate : public Iterate<TYPE, IS_FORWARD, DEPTH - 1>
	{
	public:
		using Type = TYPE;

		static constexpr bool isForward = IS_FORWARD;

		static constexpr bool isReverse = !isForward;

		static constexpr size_t depth = DEPTH;

		using End = Iterate<Type, isForward, 0>;

		using Next = Iterate<Type, isForward, depth - 1>;

		using Reverse = Iterate<Type, isReverse, depth>;

		using Iteration = Ash::Iteration<Type, isForward>;

		static constexpr bool hasNext = Ash::Type::isNotSame<Next, End>;

		template
		<
			typename ITERATE_NEXT = Next,
			typename = Ash::Type::IsSame<ITERATE_NEXT, End>
		>
		constexpr Iterate() : m_Iteration() {}

		template
		<
			typename ITERATE_NEXT = Next,
			typename = Ash::Type::IsSame<ITERATE_NEXT, End>
		>
		constexpr Iterate(Iteration iteration) : m_Iteration(iteration) {}

		template
		<
			typename ITERATE_NEXT = Next,
			typename = Ash::Type::IsNotSame<ITERATE_NEXT, End>
		>
		constexpr Iterate(const Next &next, Iteration iteration) : Next(next), m_Iteration(iteration) {}

		template
		<
			typename ITERATE_NEXT = Next,
			typename = Ash::Type::IsSame<ITERATE_NEXT, End>
		>
		static constexpr Iterate between(Type start, Type end)
		{
			if constexpr (isForward)
			{
				return Iterate(Iteration::from(start, (start <= end) ? end - start + 1 : 0));
			}
			else
			{
				return Iterate(Iteration::from(start, (start >= end) ? start - end + 1 : 0));
			}
		}

		template
		<
			typename ITERATE_NEXT = Next,
			typename = Ash::Type::IsSame<ITERATE_NEXT, End>
		>
		static constexpr Iterate from(Type value, size_t count) { return Iterate(Iteration::from(value, count)); }

		constexpr Iteration getIteration() const { return m_Iteration; }

		constexpr Reverse reverse()
		{
			if constexpr (hasNext)
			{
				return Next(*this).reverse() + Iterate<Type, isReverse>(m_Iteration.reverse());
			}
			else
			{
				return Iterate<Type, isReverse>(m_Iteration.reverse());
			}
		}

		template
		<
			size_t ITERATE_DEPTH
		>
		constexpr Iterate<Type, isForward, depth + ITERATE_DEPTH> operator + (const Iterate<Type, isForward, ITERATE_DEPTH> &iterate) const
		{
			if (!m_Iteration.isComplete() || iterate.getIteration().isComplete())
			{
				if constexpr (hasNext)
				{
					return { Next(*this) + iterate, m_Iteration };
				}
				else
				{
					return { iterate, m_Iteration };
				}
			}
			else
			{
				if constexpr (ITERATE_DEPTH > 1)
				{
					return { Iterate<Type, isForward, ITERATE_DEPTH - 1>(iterate) + *this, iterate.getIteration() };
				}
				else
				{
					return { *this, iterate.getIteration() };
				}
			}
		}

		constexpr Type operator * () const { return m_Iteration; }

		constexpr Iterate &operator ++ ()
		{
			if (!m_Iteration.advance())
			{
				getNextIterator();
			}
			return *this;
		}

		constexpr bool operator != (const Iterate &value) const { return !m_Iteration.isComplete(); }

		constexpr Iterate begin() const { return *this; }

		constexpr Iterate end() const { return *this; }

	protected:
		constexpr void getNextIterator()
		{
			if constexpr(hasNext)
			{
				m_Iteration = Next::getIteration();
				Next::getNextIterator();
			}
			else
			{
				m_Iteration = Iteration();
			}
		}

	private:
		Iteration m_Iteration;
	};

	template
	<
		typename TYPE,
		bool     IS_FORWARD
	>
	class Iterate<TYPE, IS_FORWARD, 0> : Generic::Iterate {};
}
