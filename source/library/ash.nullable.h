#pragma once

#include <utility>
#include "ash.memory.h"


namespace Ash
{
	template
	<
		typename TYPE
	>
	class Nullable
	{
	public:
		using Type = TYPE;

		constexpr Nullable() : m_IsNull(true), m_Value() {}

		constexpr Nullable(const Nullable &nullable) : m_IsNull(true), m_Value()
		{
			if (!nullable.m_IsNull)
			{
				copyValue(m_Value, nullable.m_Value);
				m_IsNull = nullable.m_IsNull;
			}
		}

		constexpr Nullable(Nullable &&nullable) : m_IsNull(true), m_Value()
		{
			m_IsNull = !nullable.remove(m_Value);
		}

		constexpr Nullable &operator = (const Nullable &nullable)
		{
			if (this != &nullable)
			{
				copyValue(m_Value, nullable.m_Value);
				m_IsNull = nullable.m_IsNull;
			}
			return *this;
		}

		constexpr Nullable &operator = (Nullable &&nullable)
		{
			if (this != &nullable)
			{
				clear();
				m_IsNull = !nullable.remove(m_Value);
			}
			return *this;
		}

		constexpr Nullable &operator = (const Type &value)
		{
			copyValue(m_Value, value);
			m_IsNull = false;
			return *this;
		}

		constexpr Nullable &operator = (Type &&value)
		{
			moveValue(m_Value, std::move(value));
			m_IsNull = false;
			return *this;
		}

		constexpr operator const Type * () const { return m_IsNull ? nullptr : &m_Value; }

		constexpr bool isNull() const { return m_IsNull; }

		constexpr const Type *getAt() const { return m_IsNull ? nullptr : &m_Value; }

		constexpr const Type &getOr(const Type &defaultValue) const { return m_IsNull ? defaultValue : m_Value; }

		constexpr Type *setAt()
		{
			m_IsNull = false;
			return &m_Value;
		}

		constexpr void clear()
		{
			if (!m_IsNull)
			{
				clearValue(m_Value);
				m_IsNull = true;
			}
		}

	protected:
		static constexpr void copyValue(Type &to, const Type &from)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				Ash::Memory::copy(to, from);
			}
			else
			{
				to = from;
			}
		}

		static constexpr void moveValue(Type &to, Type &&from)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				Ash::Memory::move(to, from);
			}
			else
			{
				to = std::move(from);
			}
		}

		static constexpr void clearValue(Type &value)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				Ash::Memory::clear(value);
			}
			else
			{
				value = Type();
			}
		}

		constexpr bool remove(Type &value)
		{
			if (m_IsNull)
			{
				return false;
			}
			moveValue(value, std::move(m_Value));
			clearValue(m_Value);
			m_IsNull = true;
			return true;
		}

	private:
		bool m_IsNull;
		Type m_Value;
	};
}
