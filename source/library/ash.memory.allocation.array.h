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
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			class Array : Ash::Memory::Generic::Dynamic
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
						deleteContent();
						return true;
					}
					
					size_t capacity = getDecreaseCapacity(m_Capacity);

					if ((length < capacity) && (capacity < m_Capacity))
					{
						return newContent(capacity, length);
					}

					for (size_t n = length; n < m_Length; n++)
					{
						m_Content[n] = Type();
					}
					m_Length = length;

					return true;
				}

			protected:
				constexpr Array() : m_Content(nullptr), m_Length(0), m_Capacity(0) {}

				constexpr ~Array() { deleteContent(); }

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Array &value) { copy(value.m_Content, value.m_Length); }

				constexpr void move(Array &value)
				{
					delete [] m_Content;

					m_Content = value.m_Content;
					m_Length = value.m_Length;
					m_Capacity = value.m_Capacity;

					value.m_Content = nullptr;
					value.m_Length = 0;
					value.m_Capacity = 0;
				}

				constexpr void copy(const Type *content, size_t length)
				{
					setLength(length);

					Ash::Memory::copyForward(m_Content, content, length);
				}

				static constexpr size_t minimumCapacity = Size(MINIMUM_CAPACITY).roundUp(BLOCK_SIZE).getValueOr(MINIMUM_CAPACITY);

				static constexpr size_t getIncreaseCapacity(size_t length)
				{
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
					Type *content = new Type[capacity];

					Ash::Memory::moveForward(content, m_Content, (length < m_Length) ? length : m_Length);

					delete [] m_Content;

					m_Content = content;
					m_Length = length;
					m_Capacity = capacity;

					return true;
				}

				constexpr void deleteContent()
				{
					delete [] m_Content;

					m_Content = nullptr;
					m_Capacity = 0;
					m_Length = 0;
				}

			private:
				Type  *m_Content;
				size_t m_Length;
				size_t m_Capacity;
			};
		}
	}
}
