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
			class Buffer : Ash::Memory::Generic::Allocation
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = false;

				static constexpr bool isFixedCapacity = true;

				static constexpr bool isReference = false;

				static constexpr size_t minCapacity = CAPACITY;

				static constexpr size_t maxCapacity = CAPACITY;

				constexpr void clear()
				{
					Ash::Memory::clear(m_Content, m_Length);
					m_Length = 0;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value copyFrom(const Buffer &value)
				{
					if (this != &value)
					{
						Ash::Memory::copyForward(m_Content, value.m_Content, value.m_Length);
						if (value.m_Length < m_Length)
						{
							Ash::Memory::clear(&m_Content[value.m_Length], m_Length - value.m_Length);
						}
						m_Length = value.m_Length;
					}

					return Ash::Error::none;
				}

				constexpr void moveFrom(Buffer &value)
				{
					if (this != &value)
					{
						Ash::Memory::moveForward(m_Content, value.m_Content, value.m_Length);
						if (value.m_Length < m_Length)
						{
							Ash::Memory::clear(&m_Content[value.m_Length], m_Length - value.m_Length);
						}
						m_Length = value.m_Length;

						value.m_Length = 0;
					}
				}

				constexpr size_t getCapacity() const { return CAPACITY; }

				[[nodiscard]]
				constexpr Ash::Error::Value setCapacity(size_t capacity)
				{
					return (capacity != CAPACITY) ? Ash::Memory::Error::capacityIsFixed : Ash::Error::none;
				}

				constexpr size_t getLength() const { return m_Length; }

				[[nodiscard]]
				constexpr Ash::Error::Value setLength(size_t length)
				{
					if (length > CAPACITY)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					if (length < m_Length)
					{
						Ash::Memory::clear(&m_Content[length], m_Length - length);
					}

					m_Length = length;

					return Ash::Error::none;
				}

			protected:
				constexpr Buffer() : m_Content(), m_Length(0) {}

				constexpr Buffer(const Type *content, size_t contentLength) : m_Content(), m_Length(0)
				{
					if (contentLength != 0)
					{
						if (contentLength > CAPACITY)
						{
							Ash::Error::throwUsing(Ash::Memory::Error::capacityOverrun);
						}

						Ash::Memory::copyForward(m_Content, content, contentLength);
						m_Length = contentLength;
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
					if (contentLength < m_Length)
					{
						Ash::Memory::clear(&m_Content[contentLength], m_Length - contentLength);
					}
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

					if (length > maxCapacity)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					Ash::Memory::copy(&m_Content[offset], content, contentLength);
					if (length > m_Length)
					{
						m_Length = length;
					}

					return Ash::Error::none;
				}

				template
				<
					typename VALUE_TYPE,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<std::remove_cvref_t<VALUE_TYPE>, Type>,
																Ash::Type::Requirement::IsClass<std::remove_cvref_t<VALUE_TYPE>, Type>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value insert(size_t offset, VALUE_TYPE &&value)
				{
					if (offset > m_Length)
					{
						return Ash::Memory::Error::writeAccessOutOfBound;
					}

					size_t length = 0;
					if (!Ash::Size(m_Length).add(1).getValue(length))
					{
						return Ash::Memory::Error::lengthOverflow;
					}

					if (length > maxCapacity)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					if (offset == m_Length)
					{
						m_Content[m_Length] = std::forward<VALUE_TYPE>(value);
					}
					else
					{
						Type valueCopy = std::forward<VALUE_TYPE>(value);

						Ash::Memory::moveBackward(&m_Content[offset + 1], &m_Content[offset], m_Length - offset);
						m_Content[offset] = std::move(valueCopy);
					}
					m_Length = length;

					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value insert(size_t offset, const Type *content, size_t contentLength)
				{
					if (contentLength == 0)
					{
						return Ash::Error::none;
					}

					if (offset > m_Length)
					{
						return Ash::Memory::Error::writeAccessOutOfBound;
					}

					size_t length;
					if (!Ash::Size(m_Length).add(contentLength).getValue(length))
					{
						return Ash::Memory::Error::lengthOverflow;
					}

					if (length > maxCapacity)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					if (!std::is_constant_evaluated() && (content >= &m_Content[0]) && (content < &m_Content[m_Length]))
					{
						if (contentLength < m_Length - offset)
						{
							Ash::Memory::copyBackward(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset);
							if (content < &m_Content[offset])
							{
								Ash::Memory::copyBackward(&m_Content[offset], content, contentLength);
							}
							else
							{
								Ash::Memory::copyForward(&m_Content[offset], &content[contentLength], contentLength);
							}
						}
						else
						{
							Ash::Memory::copyForward(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset);
							Ash::Memory::copyBackward(&m_Content[offset], content, contentLength);
						}
					}
					else if (contentLength < m_Length - offset)
					{
						Ash::Memory::moveBackward(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset);
						Ash::Memory::copyForward(&m_Content[offset], content, contentLength);
					}
					else
					{
						Ash::Memory::moveForward(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset);
						Ash::Memory::copyForward(&m_Content[offset], content, contentLength);
					}

					m_Length = length;

					return Ash::Error::none;
				}

				template
				<
					typename VALUE_TYPE,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<std::remove_cvref_t<VALUE_TYPE>, Type>,
																Ash::Type::Requirement::IsClass<std::remove_cvref_t<VALUE_TYPE>, Type>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value append(VALUE_TYPE &&value)
				{
					size_t length = 0;
					if (!Ash::Size(m_Length).add(1).getValue(length))
					{
						return Ash::Memory::Error::lengthOverflow;
					}

					if (length > maxCapacity)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					m_Content[m_Length] = std::forward<VALUE_TYPE>(value);
					m_Length = length;

					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value append(const Type *content, size_t contentLength)
				{
					if (contentLength == 0)
					{
						return Ash::Error::none;
					}

					size_t length;
					if (!Ash::Size(m_Length).add(contentLength).getValue(length))
					{
						return Ash::Memory::Error::lengthOverflow;
					}

					if (length > maxCapacity)
					{
						return Ash::Memory::Error::capacityOverrun;
					}

					Ash::Memory::copyForward(&m_Content[m_Length], content, contentLength);
					m_Length = length;

					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value remove(size_t offset, size_t length = 1)
				{
					if ((offset > m_Length) || (length > m_Length - offset))
					{
						return Ash::Memory::Error::writeAccessOutOfBound;
					}

					Ash::Memory::moveForward(&m_Content[offset], &m_Content[offset + length], m_Length - offset - length);
					if (m_Length - offset - length < length)
					{
						Ash::Memory::clear(&m_Content[m_Length - length], offset + 2 * length - m_Length);
					}
					m_Length = m_Length - length;

					return Ash::Error::none;
				}

			private:
				Type   m_Content[CAPACITY];
				size_t m_Length;
			};
		}
	}
}
