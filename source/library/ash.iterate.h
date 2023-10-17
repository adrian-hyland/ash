#pragma once

#include <limits>
#include <cstddef>
#include <cstdint>
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
		typename = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template Or<Ash::Type::Requirement::IsPointer>::IsValid
	>
	class Iteration
	{
	public:
		using Type = TYPE;

		using ReferenceType = std::add_lvalue_reference_t<std::remove_pointer_t<Type>>;

		static constexpr bool isForward = IS_FORWARD;

		static constexpr bool isReverse = !isForward;

		using Reverse = Iteration<Type, isReverse>;

		constexpr Iteration() : m_Start(), m_End(), m_IsComplete(true) {}

		constexpr Iteration(Type start, Type end, bool isComplete = false) : m_Start(start), m_End(end), m_IsComplete(isComplete)
		{
			if constexpr (Ash::Type::isPointer<Type>)
			{
				m_IsComplete = m_IsComplete || (m_Start == nullptr) || (m_End == nullptr);
			}
		}

		template
		<
			typename RETURN_TYPE = Type,
			typename = Ash::Type::IsInteger<RETURN_TYPE>
		>
		constexpr operator Type () const { return m_Start; }

		template
		<
			typename RETURN_TYPE = Type,
			typename = Ash::Type::IsPointer<RETURN_TYPE>
		>
		constexpr operator ReferenceType () const { return *m_Start; }

		constexpr bool isComplete() const { return m_IsComplete; }

		constexpr bool advance()
		{
			bool isAtEnd = (m_Start == m_End);

			if constexpr (Ash::Type::isSignedInteger<Type>)
			{
				if constexpr (isForward)
				{
					m_Start = m_Start + std::uintmax_t(1);
				}
				else
				{
					m_Start = m_Start - std::uintmax_t(1);
				}
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

			return !isAtEnd;
		}

		constexpr Reverse reverse() const
		{
			return Reverse(m_End, m_Start, m_IsComplete);
		}

		static constexpr Iteration between(Type start, Type end)
		{
			if constexpr (isForward)
			{
				return Iteration(start, end, start > end);
			}
			else
			{
				return Iteration(start, end, start < end);
			}
		}

		static constexpr Iteration from(Type value, size_t count)
		{
			if (count == 0)
			{
				return { value, value, true };
			}

			count--;

			if constexpr (isForward)
			{
				if constexpr (Ash::Type::isInteger<Type>)
				{
					size_t max = size_t(std::numeric_limits<Type>::max()) - value;
					if (count > max)
					{
						return { value, std::numeric_limits<Type>::max() };
					}
				}

				return { value, Type(value + count) };
			}
			else
			{
				if constexpr (Ash::Type::isInteger<Type>)
				{
					size_t max = (std::numeric_limits<Type>::min() < 0) ? size_t(-(std::numeric_limits<Type>::min() + 1)) + value + 1 : value;
					if (count > max)
					{
						return { value, std::numeric_limits<Type>::min() };
					}
				}

				return { value, Type(value - count) };
			}
		}

	private:
		Type m_Start;
		Type m_End;
		bool m_IsComplete;
	};

	template
	<
		typename TYPE,
		bool     IS_FORWARD = true,
		size_t   DEPTH = 1,
		typename = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template Or<Ash::Type::Requirement::IsPointer>::IsValid
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

		using ReferenceType = typename Iteration::ReferenceType;

		static constexpr bool hasNext = Ash::Type::isNotSame<Next, End>;

		template
		<
			typename ITERATE_NEXT = Next,
			typename = Ash::Type::IsSame<ITERATE_NEXT, End>
		>
		constexpr Iterate() : m_Iteration() {}

		constexpr Iterate(const Iterate &iterate) : Next(iterate), m_Iteration(iterate.m_Iteration) {}

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
		static constexpr Iterate between(Type from, Type to) { return Iterate(Iteration::between(from, to)); }

		template
		<
			typename ITERATE_NEXT = Next,
			typename = Ash::Type::IsSame<ITERATE_NEXT, End>
		>
		static constexpr Iterate from(Type value, size_t count) { return Iterate(Iteration::from(value, count)); }

		constexpr Iteration getIteration() const { return m_Iteration; }

		constexpr Reverse reverse() const
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

		template
		<
			typename RETURN_TYPE = Type,
			typename = Ash::Type::IsInteger<RETURN_TYPE>
		>
		constexpr Type operator * () const { return m_Iteration; }

		template
		<
			typename RETURN_TYPE = Type,
			typename = Ash::Type::IsPointer<RETURN_TYPE>
		>
		constexpr ReferenceType operator * () const { return m_Iteration; }

		constexpr Iterate &operator ++ ()
		{
			if (!m_Iteration.advance())
			{
				getNextIteration();
			}
			return *this;
		}

		constexpr bool operator != (const Iterate &value) const
		{
			(void)value;
			return !m_Iteration.isComplete();
		}

		constexpr Iterate begin() const { return *this; }

		constexpr Iterate end() const { return *this; }

	protected:
		constexpr void getNextIteration()
		{
			if constexpr(hasNext)
			{
				m_Iteration = Next::getIteration();
				Next::getNextIteration();
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
