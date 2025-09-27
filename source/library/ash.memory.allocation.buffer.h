#pragma once

#include "ash.size.h"
#include "ash.memory.core.h"
#include "ash.memory.generic.h"


namespace Ash
{
	namespace Memory
	{
		namespace Allocation
		{
			template
			<
				typename TYPE,
				size_t   CAPACITY
			>
			class Buffer : Ash::Memory::Generic::Allocation
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = false;

				static constexpr bool isFixedCapacity = true;

				static constexpr bool isReference = false;

				static constexpr size_t maxCapacity = CAPACITY;

				constexpr size_t getCapacity() const { return CAPACITY; }

				constexpr size_t getLength() const { return m_Length; }

				constexpr bool setLength(size_t length)
				{
					if ((length > CAPACITY) || (m_Length > CAPACITY))
					{
						return false;
					}

					if (m_Length > length)
					{
						Ash::Memory::clear(&m_Content[length], m_Length - length);
					}

					m_Length = length;

					return true;
				}

				constexpr bool increaseLength(size_t length)
				{
					if ((length > CAPACITY) || (m_Length > CAPACITY - length))
					{
						return false;
					}

					m_Length = m_Length + length;

					return true;
				}

				constexpr bool decreaseLength(size_t length)
				{
					if (length > m_Length)
					{
						return false;
					}

					m_Length = m_Length - length;

					Ash::Memory::clear(&m_Content[m_Length], length);

					return true;
				}

			protected:
				constexpr Buffer() : m_Content(), m_Length(0) {}

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Buffer &value) { copy(value.m_Content, value.m_Length); }

				constexpr void move(Buffer &value) { move(value.m_Content, value.m_Length); }

				constexpr void copy(const Type *content, size_t length)
				{
					setLength((length < CAPACITY) ? length : CAPACITY);

					Ash::Memory::copyForward(m_Content, content, m_Length);
				}

				constexpr void move(Type *content, size_t length)
				{
					setLength((length < CAPACITY) ? length : CAPACITY);

					Ash::Memory::moveForward(m_Content, content, m_Length);
				}

			private:
				Type   m_Content[CAPACITY];
				size_t m_Length;
			};
		}
	}
}
