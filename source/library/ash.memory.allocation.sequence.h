#pragma once

#include "ash.size.h"
#include "ash.memory.core.h"
#include "ash.memory.error.h"
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

				static constexpr size_t minCapacity = CAPACITY;

				static constexpr size_t maxCapacity = CAPACITY;

				constexpr void clear()
				{
					Ash::Memory::clear(m_Content, CAPACITY);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value copyFrom(const Sequence &value)
				{
					if (this != &value)
					{
						Ash::Memory::copyForward(m_Content, value.m_Content, CAPACITY);
					}
					return Ash::Error::none;
				}

				constexpr void moveFrom(Sequence &value)
				{
					if (this != &value)
					{
						Ash::Memory::moveForward(m_Content, value.m_Content, CAPACITY);
					}
				}

				constexpr size_t getCapacity() const { return CAPACITY; }

				[[nodiscard]]
				constexpr Ash::Error::Value setCapacity(size_t capacity)
				{
					return (capacity != CAPACITY) ? Ash::Memory::Error::capacityIsFixed : Ash::Error::none;
				}

				constexpr size_t getLength() const { return CAPACITY; }

				[[nodiscard]]
				constexpr Ash::Error::Value setLength(size_t length)
				{
					return (length != CAPACITY) ? Ash::Memory::Error::lengthIsFixed : Ash::Error::none;
				}

			protected:
				constexpr Sequence() : m_Content() {}

				constexpr Sequence(const Type *content, size_t contentLength) : m_Content()
				{
					if (contentLength != 0)
					{
						if (contentLength > CAPACITY)
						{
							Ash::Error::throwUsing(Ash::Memory::Error::capacityOverrun);
						}

						Ash::Memory::copyForward(m_Content, content, contentLength);
					}
				}

				constexpr Type &operator [] (size_t offset)
				{
					return m_Content[offset];
				}

				constexpr const Type &operator [] (size_t offset) const
				{
					return m_Content[offset];
				}

				[[nodiscard]]
				constexpr Ash::Error::Value copyFrom(const Type *content, size_t contentLength)
				{
					if (contentLength == 0)
					{
						clear();
						return Ash::Error::none;
					}

					if (contentLength > maxCapacity)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					Ash::Memory::copyForward(m_Content, content, contentLength);
					if (contentLength < maxCapacity)
					{
						Ash::Memory::clear(&m_Content[contentLength], maxCapacity - contentLength);
					}

					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(size_t offset, const Type *content, size_t contentLength)
				{
					if (contentLength == 0)
					{
						return Ash::Error::none;
					}

					if (offset > maxCapacity)
					{
						return Ash::Memory::Error::writeAccessOutOfBound;
					}

					size_t length = 0;
					if (!Ash::Size(offset).add(contentLength).getValue(length))
					{
						return Ash::Memory::Error::lengthOverflow;
					}

					if (length > maxCapacity)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					Ash::Memory::copy(&m_Content[offset], content, contentLength);

					return Ash::Error::none;
				}

			private:
				Type m_Content[CAPACITY];
			};
		}
	}
}
