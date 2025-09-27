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
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			class ArrayBuffer : Ash::Memory::Generic::Dynamic
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = false;

				static constexpr bool isFixedCapacity = false;

				static constexpr bool isReference = false;

				static constexpr size_t maxCapacity = std::numeric_limits<size_t>::max();

				constexpr size_t getCapacity() const { return m_Capacity; }

				constexpr size_t getLength() const { return m_Length; }

				constexpr bool setLength(size_t length)
				{
					return (length < m_Length) ? decreaseLength(m_Length - length) : increaseLength(length - m_Length);
				}

				constexpr bool increaseLength(size_t increase)
				{
					size_t length = 0;

					if (!Size(m_Length).add(increase).getValue(length))
					{
						return false;
					}
					
					if (length > m_Capacity)
					{
						return newContent(getIncreaseCapacity(length), length);
					}

					m_Length = length;

					return true;
				}

				constexpr bool decreaseLength(size_t decrease)
				{
					size_t length = 0;
					
					if (!Size(m_Length).subtract(decrease).getValue(length))
					{
						return false;
					}

					if (length == 0)
					{
						deleteContent<true>();
						return true;
					}
					
					size_t capacity = getDecreaseCapacity(m_Capacity);

					if ((length < capacity) && (capacity < m_Capacity))
					{
						return newContent(capacity, length);
					}

					Ash::Memory::clear(&m_Content[length], m_Length - length);
					m_Length = length;

					return true;
				}

			protected:
				constexpr ArrayBuffer() : m_Buffer(), m_Content(m_Buffer), m_Length(0), m_Capacity(CAPACITY) {}

				constexpr ~ArrayBuffer() { deleteContent<false>(); }

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const ArrayBuffer &value) { copy(value.m_Content, value.m_Length); }

				constexpr void move(ArrayBuffer &value)
				{
					if (m_Content != m_Buffer)
					{
						delete [] m_Content;
						m_Content = m_Buffer;
						m_Capacity = CAPACITY;
					}

					if (value.m_Content == value.m_Buffer)
					{
						Ash::Memory::moveForward(m_Buffer, value.m_Buffer, value.m_Length);
					}
					else
					{
						m_Content = value.m_Content;
						m_Capacity = value.m_Capacity;
					}
					m_Length = value.m_Length;

					value.m_Content = value.m_Buffer;
					value.m_Length = 0;
					value.m_Capacity = CAPACITY;
				}

				constexpr void copy(const Type *content, size_t length)
				{
					setLength(length);

					Ash::Memory::copyForward(m_Content, content, length);
				}

				static constexpr size_t minimumCapacity = Size(CAPACITY).roundUp(BLOCK_SIZE).getValueOr(CAPACITY);

				static constexpr size_t getIncreaseCapacity(size_t length)
				{
					if (length <= CAPACITY)
					{
						return CAPACITY;
					}

					if (length <= minimumCapacity)
					{
						return minimumCapacity;
					}

					Size capacity(length);

					if constexpr (PERCENTAGE_INCREASE != 0)
					{
						capacity = capacity.add(capacity.percent(PERCENTAGE_INCREASE));
					}

					return capacity.roundUp(BLOCK_SIZE).getValueOr(length);
				}

				static constexpr size_t getDecreaseCapacity(size_t length)
				{
					if (length <= CAPACITY)
					{
						return CAPACITY;
					}

					if (length <= minimumCapacity)
					{
						return minimumCapacity;
					}

					Size capacity(length);

					if constexpr (PERCENTAGE_INCREASE != 0)
					{
						capacity = capacity.subtract(capacity.reversePercent(PERCENTAGE_INCREASE));
					}

					return capacity.roundUp(BLOCK_SIZE).getValueOr(length);
				}

				constexpr bool newContent(size_t capacity, size_t length)
				{
					Type *content = (capacity <= CAPACITY) ? m_Buffer : new Type[capacity];

					if (content != m_Content)
					{
						Ash::Memory::moveForward(content, m_Content, (length < m_Length) ? length : m_Length);
					}

					if (m_Content != m_Buffer)
					{
						delete [] m_Content;
					}

					m_Content = content;
					m_Length = length;
					m_Capacity = capacity;

					return true;
				}

				template
				<
					bool CLEAR_BUFFER = true
				>
				constexpr void deleteContent()
				{
					if (m_Content != m_Buffer)
					{
						delete [] m_Content;
						m_Content = m_Buffer;
					}
					else if constexpr (CLEAR_BUFFER)
					{
						Ash::Memory::clear(m_Buffer, m_Length);
					}

					m_Capacity = CAPACITY;
					m_Length = 0;
				}

			private:
				Type   m_Buffer[CAPACITY];
				Type  *m_Content;
				size_t m_Length;
				size_t m_Capacity;
			};
		}
	}
}