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
			class Sequence : Ash::Memory::Generic::Allocation
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = true;

				static constexpr bool isFixedCapacity = true;

				static constexpr bool isReference = false;

				static constexpr size_t maxCapacity = CAPACITY;

				constexpr size_t getCapacity() const { return CAPACITY; }

				constexpr size_t getLength() const { return CAPACITY; }

				constexpr bool setLength(size_t length) const { return length == CAPACITY; }

				constexpr bool decreaseLength(size_t length) { return length == 0; }

				constexpr bool increaseLength(size_t length) { return length == 0; }

			protected:
				constexpr Sequence() : m_Content() {}

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Sequence &value) { copy(value.m_Content, CAPACITY); }

				constexpr void move(Sequence &value) { move(value.m_Content, CAPACITY); }

				constexpr void copy(const Type *content, size_t length)
				{
					if (length > CAPACITY)
					{
						length = CAPACITY;
					}

					Ash::Memory::copyForward(m_Content, content, length);
					Ash::Memory::clear(&m_Content[length], CAPACITY - length);
				}

				constexpr void move(Type *content, size_t length)
				{
					if (length > CAPACITY)
					{
						length = CAPACITY;
					}

					Ash::Memory::moveForward(m_Content, content, length);
					Ash::Memory::clear(&m_Content[length], CAPACITY - length);
				}

			private:
				Type m_Content[CAPACITY];
			};
		}
	}
}
