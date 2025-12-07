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
				typename TYPE
			>
			class Reference : Ash::Memory::Generic::Reference
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = true;

				static constexpr bool isFixedCapacity = true;

				static constexpr bool isReference = true;

				static constexpr size_t minCapacity = 1;

				static constexpr size_t maxCapacity = std::numeric_limits<size_t>::max();

				constexpr void clear()
				{
					m_Content = nullptr;
					m_Length = 0;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value copyFrom(const Reference &value)
				{
					m_Content = value.m_Content;
					m_Length = value.m_Length;
					return Ash::Error::none;
				}

				constexpr void moveFrom(Reference &value)
				{
					m_Content = value.m_Content;
					m_Length = value.m_Length;
				}

				constexpr size_t getCapacity() const { return m_Length; }

				[[nodiscard]]
				constexpr Ash::Error::Value setCapacity(size_t capacity)
				{
					return (capacity != m_Length) ? Ash::Memory::Error::capacityIsFixed : Ash::Error::none;
				}

				constexpr size_t getLength() const { return m_Length; }

				[[nodiscard]]
				constexpr Ash::Error::Value setLength(size_t length)
				{
					return (length == m_Length) ? Ash::Error::none : Ash::Memory::Error::lengthIsFixed;
				}

			protected:
				constexpr Reference() : m_Content(nullptr), m_Length(0) {}

				constexpr Reference(Type *content, size_t contentLength) : m_Content(content), m_Length(contentLength) {}

				template
				<
					typename ALLOCATION_TYPE = Type,
					typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
				>
				constexpr Type &operator [] (size_t offset)
				{
					return m_Content[offset];
				}

				constexpr const Type &operator [] (size_t offset) const
				{
					return m_Content[offset];
				}

				[[nodiscard]]
				constexpr Ash::Error::Value copyFrom(Type *content, size_t contentLength)
				{
					m_Content = content;
					m_Length = contentLength;
					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(size_t offset, const Type *content, size_t contentLength)
				{
					if (contentLength == 0)
					{
						return Ash::Error::none;
					}

					if (offset > m_Length)
					{
						return Ash::Memory::Error::writeAccessOutOfBound;
					}

					size_t length = 0;
					if (!Ash::Size(offset).add(contentLength).getValue(length))
					{
						return Ash::Memory::Error::lengthOverflow;
					}

					if (length > m_Length)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					Ash::Memory::copy(&m_Content[offset], content, contentLength);

					return Ash::Error::none;
				}

			private:
				Type  *m_Content;
				size_t m_Length;
			};
		}
	}
}
