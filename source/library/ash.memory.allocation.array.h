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

				static constexpr size_t minCapacity = Ash::Size(MINIMUM_CAPACITY).roundUp((BLOCK_SIZE != 0) ? BLOCK_SIZE : 1).getValueOr(MINIMUM_CAPACITY);

				static constexpr size_t maxCapacity = std::numeric_limits<size_t>::max();

				constexpr void clear()
				{
					release();

					m_Content = nullptr;
					m_Capacity = 0;
					m_Length = 0;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value copyFrom(const Array &value)
				{
					if (this == &value)
					{
						return Ash::Error::none;
					}

					if (value.m_Length == 0)
					{
						clear();
						return Ash::Error::none;
					}

					if (m_Capacity != value.m_Capacity)
					{
						return reallocateAndReplace(value.m_Capacity, value.m_Content, value.m_Length);
					}

					if (value.m_Length > m_Length)
					{
						Ash::Memory::copyForward(m_Content, value.m_Content, m_Length);
						Ash::Memory::copyConstruct(&m_Content[m_Length], &value.m_Content[m_Length], value.m_Length - m_Length);
					}
					else
					{
						Ash::Memory::copyForward(m_Content, value.m_Content, value.m_Length);
						Ash::Memory::destroy(&m_Content[value.m_Length], m_Length - value.m_Length);
					}

					m_Length = value.m_Length;

					return Ash::Error::none;
				}

				constexpr void moveFrom(Array &value)
				{
					if (this != &value)
					{
						release();

						m_Content = value.m_Content;
						m_Length = value.m_Length;
						m_Capacity = value.m_Capacity;

						value.m_Content = nullptr;
						value.m_Length = 0;
						value.m_Capacity = 0;
					}
				}

				constexpr size_t getCapacity() const { return m_Capacity; }

				[[nodiscard]]
				constexpr Ash::Error::Value setCapacity(size_t capacity)
				{
					if (capacity < m_Length)
					{
						return Ash::Memory::Error::capacityBelowLength;
					}

					if (capacity == 0)
					{
						return Ash::Error::none;
					}

					capacity = (capacity < minCapacity) ? minCapacity : Ash::Size(capacity).roundUp((BLOCK_SIZE != 0) ? BLOCK_SIZE : 1).getValueOr(capacity);
					return (capacity != m_Capacity) ? reallocateWithLength(capacity, m_Length) : Ash::Error::none;
				}

				constexpr size_t getLength() const { return m_Length; }

				[[nodiscard]]
				constexpr Ash::Error::Value setLength(size_t length)
				{
					if (length == 0)
					{
						clear();
						return Ash::Error::none;
					}

					size_t capacity = ((length < m_Length) || (length > m_Capacity)) ? getCapacity(length) : m_Capacity;
					if (capacity != m_Capacity)
					{
						return reallocateWithLength(capacity, length);
					}

					if (length < m_Length)
					{
						Ash::Memory::destroy(&m_Content[length], m_Length - length);
					}
					else
					{
						Ash::Memory::defaultConstruct(&m_Content[m_Length], length - m_Length);
					}

					m_Length = length;

					return Ash::Error::none;
				}

			protected:
				constexpr Array() : m_Content(nullptr), m_Length(0), m_Capacity(0) {}

				constexpr Array(const Type *content, size_t contentLength) : m_Content(nullptr), m_Length(0), m_Capacity(0)
				{
					if (contentLength != 0)
					{
						acquire(getCapacity(contentLength)).throwOnError();
						Ash::Memory::copyConstruct(m_Content, content, contentLength);
						m_Length = contentLength;
					}
				}

				constexpr ~Array() { release(); }

				constexpr Type &operator [] (size_t offset)
				{
					return m_Content[offset];
				}

				constexpr const Type &operator [] (size_t offset) const
				{
					return m_Content[offset];
				}

				static constexpr size_t getCapacity(size_t length)
				{
					if (length <= minCapacity)
					{
						return minCapacity;
					}

					Ash::Size capacity(length);

					if constexpr (PERCENTAGE_INCREASE != 0)
					{
						capacity = capacity.add(capacity.percent(PERCENTAGE_INCREASE));
					}

					return capacity.roundUp((BLOCK_SIZE != 0) ? BLOCK_SIZE : 1).getValueOr(length);
				}

				template
				<
					typename VALUE_TYPE,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<std::remove_cvref_t<VALUE_TYPE>, Type>,
																Ash::Type::Requirement::IsClass<std::remove_cvref_t<VALUE_TYPE>, Type>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value reallocate(size_t capacity, VALUE_TYPE &&value, size_t retainStartLength, size_t retainEndLength)
				{
					Ash::Error::assert(retainStartLength <= m_Length, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= m_Length, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= m_Length - retainStartLength, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainStartLength <= capacity, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= capacity, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= capacity - retainStartLength, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(capacity - retainStartLength - retainEndLength >= 1, Ash::Memory::Error::readAccessOutOfBound);

					Type *previousContent = m_Content;
					size_t previousLength = m_Length;

					Ash::Error::Value error = acquire(capacity);
					if (error.hasErrorSet())
					{
						return error;
					}

					Ash::Memory::construct(m_Content[retainStartLength], std::forward<VALUE_TYPE>(value));
					Ash::Memory::moveConstruct(m_Content, previousContent, retainStartLength);
					Ash::Memory::moveConstruct(&m_Content[retainStartLength + 1], &previousContent[previousLength - retainEndLength], retainEndLength);
					m_Length = retainStartLength + retainEndLength + 1;

					release(previousContent, retainStartLength, previousLength - retainStartLength - retainEndLength);

					return Ash::Error::none;
				}

				template
				<
					typename VALUE_TYPE,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<std::remove_cvref_t<VALUE_TYPE>, Type>,
																Ash::Type::Requirement::IsClass<std::remove_cvref_t<VALUE_TYPE>, Type>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value reallocateAndSet(size_t capacity, size_t offset, VALUE_TYPE &&value)
				{
					return reallocate(capacity, value, offset, 0);
				}

				template
				<
					typename VALUE_TYPE,
					typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<std::remove_cvref_t<VALUE_TYPE>, Type>,
																Ash::Type::Requirement::IsClass<std::remove_cvref_t<VALUE_TYPE>, Type>>::IsValid
				>
				[[nodiscard]]
				constexpr Ash::Error::Value reallocateAndInsert(size_t capacity, size_t offset, VALUE_TYPE &&value)
				{
					return reallocate(capacity, value, offset, m_Length - offset);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value reallocate(size_t capacity, const Type *content, size_t contentLength, size_t retainStartLength, size_t retainEndLength)
				{
					Ash::Error::assert(retainStartLength <= m_Length, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= m_Length, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= m_Length - retainStartLength, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainStartLength <= capacity, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= capacity, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(retainEndLength <= capacity - retainStartLength, Ash::Memory::Error::readAccessOutOfBound);
					Ash::Error::assert(capacity - retainStartLength - retainEndLength >= contentLength, Ash::Memory::Error::readAccessOutOfBound);

					Type *previousContent = m_Content;
					size_t previousLength = m_Length;

					Ash::Error::Value error = acquire(capacity);
					if (error.hasErrorSet())
					{
						return error;
					}

					if (content != nullptr)
					{
						Ash::Memory::copyConstruct(&m_Content[retainStartLength], content, contentLength);
					}
					else
					{
						Ash::Memory::defaultConstruct(&m_Content[retainStartLength], contentLength);
					}

					Ash::Memory::moveConstruct(m_Content, previousContent, retainStartLength);
					Ash::Memory::moveConstruct(&m_Content[retainStartLength + contentLength], &previousContent[previousLength - retainEndLength], retainEndLength);
					m_Length = retainStartLength + retainEndLength + contentLength;

					release(previousContent, retainStartLength, previousLength - retainStartLength - retainEndLength);

					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value reallocateAndSet(size_t capacity, size_t offset, const Type *content, size_t contentLength)
				{
					return reallocate(capacity, content, contentLength, offset, 0);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value reallocateAndInsert(size_t capacity, size_t offset, const Type *content, size_t contentLength)
				{
					return reallocate(capacity, content, contentLength, offset, m_Length - offset);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value reallocateAndReplace(size_t capacity, const Type *content, size_t contentLength)
				{
					return reallocate(capacity, content, contentLength, 0, 0);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value reallocateAndRemove(size_t capacity, size_t offset, size_t length)
				{
					return reallocate(capacity, nullptr, 0, offset, m_Length - offset - length);
				}


				[[nodiscard]]
				constexpr Ash::Error::Value reallocateWithLength(size_t capacity, size_t length)
				{
					return reallocate(capacity, nullptr, (length > m_Length) ? length - m_Length : 0, m_Length, 0);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value acquire(size_t capacity)
				{
					size_t length;
					if (!Size(capacity).multiply(sizeof(Type)).getValue(length))
					{
						return Ash::Memory::Error::lengthOverflow;
					}

					Type *content = static_cast<Type *>(::operator new(length, std::nothrow));
					if (content == nullptr)
					{
						return Ash::Memory::Error::allocationFailure;
					}

					m_Content = content;
					m_Capacity = capacity;
					m_Length = 0;

					return Ash::Error::none;
				}

				constexpr void release()
				{
					release(m_Content, 0, m_Length);
				}

				constexpr void release(Type *content, size_t destroyOffset = 0, size_t destroyLength = 0)
				{
					Ash::Memory::destroy(&content[destroyOffset], destroyLength);
					::operator delete(content);
				}

				[[nodiscard]]
				constexpr Ash::Error::Value copyFrom(const Type *content, size_t contentLength)
				{
					if (contentLength == 0)
					{
						clear();
						return Ash::Error::none;
					}

					size_t capacity = ((contentLength > m_Capacity) || (contentLength < m_Length)) ? getCapacity(contentLength) : m_Capacity;
					if (capacity != m_Capacity)
					{
						return reallocateAndReplace(capacity, content, contentLength);
					}

					if (contentLength > m_Length)
					{
						Ash::Memory::copyForward(m_Content, content, m_Length);
						Ash::Memory::copyConstruct(&m_Content[m_Length], &content[m_Length], contentLength - m_Length);
					}
					else
					{
						Ash::Memory::copyForward(m_Content, content, contentLength);
						Ash::Memory::destroy(&m_Content[contentLength], m_Length - contentLength);
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

					if (length > m_Capacity)
					{
						return reallocateAndSet(getCapacity(length), offset, content, contentLength);
					}

					if (length > m_Length)
					{
						Ash::Memory::copyConstruct(&m_Content[m_Length], &content[m_Length - offset], contentLength - m_Length + offset);
						contentLength = m_Length - offset;
						m_Length = length;
					}

					Ash::Memory::copy(&m_Content[offset], content, contentLength);

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

					if (length > m_Capacity)
					{
						return reallocateAndInsert(getCapacity(length), offset, std::forward<VALUE_TYPE>(value));
					}

					if (offset == m_Length)
					{
						Ash::Memory::construct(m_Content[m_Length], std::forward<VALUE_TYPE>(value));
					}
					else
					{
						Type valueCopy = std::forward<VALUE_TYPE>(value);

						Ash::Memory::construct(m_Content[m_Length], std::move(m_Content[m_Length - 1]));
						Ash::Memory::moveBackward(&m_Content[offset + 1], &m_Content[offset], m_Length - offset - 1);
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

					if (length > m_Capacity)
					{
						return reallocateAndInsert(getCapacity(length), offset, content, contentLength);
					}

					if (!std::is_constant_evaluated() && (content >= &m_Content[0]) && (content < &m_Content[m_Length]))
					{
						if (contentLength < m_Length - offset)
						{
							Ash::Memory::copyConstruct(&m_Content[m_Length], &m_Content[m_Length - contentLength], contentLength);
							Ash::Memory::copyBackward(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset - contentLength);
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
							Ash::Memory::copyConstruct(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset);
							Ash::Memory::copyConstruct(&m_Content[m_Length], &content[m_Length - offset], contentLength + offset - m_Length);
							Ash::Memory::copyBackward(&m_Content[offset], content, m_Length - offset);
						}
					}
					else if (contentLength < m_Length - offset)
					{
						Ash::Memory::moveConstruct(&m_Content[m_Length], &m_Content[m_Length - contentLength], contentLength);
						Ash::Memory::moveBackward(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset - contentLength);
						Ash::Memory::copyForward(&m_Content[offset], content, contentLength);
					}
					else
					{
						Ash::Memory::moveConstruct(&m_Content[offset + contentLength], &m_Content[offset], m_Length - offset);
						Ash::Memory::copyConstruct(&m_Content[m_Length], &content[m_Length - offset], contentLength + offset - m_Length);
						Ash::Memory::copyForward(&m_Content[offset], content, m_Length - offset);
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

					if (length > m_Capacity)
					{
						return reallocateAndSet(getCapacity(length), m_Length, std::forward<VALUE_TYPE>(value));
					}

					Ash::Memory::construct(m_Content[m_Length], std::forward<VALUE_TYPE>(value));
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

					if (length > m_Capacity)
					{
						return reallocateAndSet(getCapacity(length), m_Length, content, contentLength);
					}

					Ash::Memory::copyConstruct(&m_Content[m_Length], content, contentLength);
					m_Length = length;

					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value remove(size_t offset, size_t length = 1)
				{
					if (length == 0)
					{
						return Ash::Error::none;
					}

					if ((offset > m_Length) || (length > m_Length - offset))
					{
						return Ash::Memory::Error::writeAccessOutOfBound;
					}

					if ((offset == 0) && (length == m_Length))
					{
						clear();
						return Ash::Error::none;
					}

					size_t capacity = getCapacity(m_Length - length);
					if (capacity != m_Capacity)
					{
						return reallocateAndRemove(capacity, offset, length);
					}

					Ash::Memory::moveForward(&m_Content[offset], &m_Content[offset + length], m_Length - offset - length);
					if (m_Length - offset - length < length)
					{
						Ash::Memory::destroy(&m_Content[m_Length - length], offset + 2 * length - m_Length);
					}

					m_Length = m_Length - length;

					return Ash::Error::none;
				}

			private:
				Type  *m_Content;
				size_t m_Length;
				size_t m_Capacity;
			};
		}
	}
}
