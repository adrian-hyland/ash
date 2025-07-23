#pragma once

#include <utility>
#include <algorithm>
#include <cstring>
#include <cwchar>
#include "ash.type.h"
#include "ash.size.h"
#include "ash.iterate.h"


namespace Ash
{
	namespace Memory
	{
		template
		<
			typename TYPE
		>
		constexpr size_t copyForward(TYPE *to, const TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					copyForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					to[n] = from[n];
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t copyBackward(TYPE *to, const TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					copyForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					to[n] = from[n];
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t copy(TYPE *to, const TYPE *from, size_t length)
		{
			return ((from < to) && (from + length > to)) ? copyBackward(to, from, length) : copyForward(to, from, length);
		}

		template
		<
			typename TYPE
		>
		constexpr size_t moveForward(TYPE *to, TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					moveForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					to[n] = std::move(from[n]);
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t moveBackward(TYPE *to, TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					moveForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					to[n] = std::move(from[n]);
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t move(TYPE *to, TYPE *from, size_t length)
		{
			return ((from < to) && (from + length > to)) ? moveBackward(to, from, length) : moveForward(to, from, length);
		}

		template
		<
			typename TYPE
		>
		constexpr size_t clear(TYPE *content, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					clear(content[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated())
					{
						::memset(content, 0, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					content[n] = TYPE();
				}
			}
			return length;
		}

		template
		<
			typename TYPE,
			size_t   TO_SIZE,
			size_t   FROM_SIZE
		>
		constexpr size_t copy(TYPE (&to)[TO_SIZE], const TYPE (&from)[FROM_SIZE])
		{
			return copyForward(to, from, std::min(TO_SIZE, FROM_SIZE));
		}

		template
		<
			typename TYPE,
			size_t   TO_SIZE,
			size_t   FROM_SIZE
		>
		constexpr size_t move(TYPE (&to)[TO_SIZE], TYPE (&from)[FROM_SIZE])
		{
			return moveForward(to, from, std::min(TO_SIZE, FROM_SIZE));
		}

		template
		<
			typename TYPE,
			size_t   SIZE
		>
		constexpr size_t clear(TYPE (&content)[SIZE])
		{
			return clear(content, SIZE);
		}

		namespace Generic
		{
			class Allocation {};

			class Dynamic : Allocation {};

			class Reference : Allocation {};
		}

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

				static constexpr size_t maxCapacity = std::numeric_limits<size_t>::max();

				constexpr size_t getCapacity() const { return m_Length; }
				
				constexpr size_t getLength() const { return m_Length; }

				constexpr bool setLength(size_t length) const { return length == m_Length; }

				constexpr bool decreaseLength(size_t length) { return length == 0; }

				constexpr bool increaseLength(size_t length) { return length == 0; }

			protected:
				constexpr Reference() : m_Content(nullptr), m_Length(0) {}

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Reference &value) { copy(value.m_Content, value.m_Length); }

				constexpr void move(Reference &value) { move(value.m_Content, value.m_Length); }

				constexpr void copy(Type *content, size_t length)
				{
					m_Content = content;
					m_Length = length;
				}

				constexpr void move(Type *content, size_t length)
				{
					m_Content = content;
					m_Length = length;
				}

			private:
				Type  *m_Content;
				size_t m_Length;
			};
		}

		template
		<
			typename ALLOCATION,
			typename TYPE = std::remove_cv_t<typename ALLOCATION::Type>,
			typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
		>
		class Value;

		template
		<
			typename TYPE,
			size_t   MINIMUM_CAPACITY    = 32,
			size_t   PERCENTAGE_INCREASE = 50,
			size_t   BLOCK_SIZE          = 32
		>
		using Array = Value<Allocation::Array<TYPE, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>>;

		template
		<
			typename TYPE,
			size_t   CAPACITY            = 32,
			size_t   PERCENTAGE_INCREASE = 50,
			size_t   BLOCK_SIZE          = 32
		>
		using ArrayBuffer = Value<Allocation::ArrayBuffer<TYPE, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>>;

		template
		<
			typename TYPE,
			size_t   CAPACITY
		>
		using Buffer = Value<Allocation::Buffer<TYPE, CAPACITY>>;

		template
		<
			typename TYPE,
			size_t   CAPACITY
		>
		using Sequence = Value<Allocation::Sequence<TYPE, CAPACITY>>;

		template
		<
			typename TYPE
		>
		using Area = Value<Allocation::Reference<TYPE>>;

		template
		<
			typename TYPE
		>
		using View = Value<Allocation::Reference<const TYPE>>;

		template
		<
			typename ALLOCATION,
			typename TYPE,
			typename
		>
		class Value : public ALLOCATION
		{
		public:
			using Allocation = ALLOCATION;

			using Type = TYPE;

			constexpr Value() : Allocation() {}

			constexpr Value(std::initializer_list<Type> content) : Allocation() { Allocation::copy(content.begin(), content.size()); }

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Value(Type *content, size_t length) : Allocation() { Allocation::copy(content, length); }

			constexpr Value(const Type *content, size_t length) : Allocation() { Allocation::copy(content, length); }

			template
			<
				typename VALUE_ALLOCATION,
				typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			constexpr Value(const Value<VALUE_ALLOCATION, Type> &value) : Allocation() { Allocation::copy(value.at(0), value.getLength()); }

			constexpr Value(const Value &value) : Allocation() { Allocation::copy(value); }

			constexpr Value(Value &&value) noexcept : Allocation() { Allocation::move(value); }

			constexpr Value &operator = (const Value &value)
			{
				if (this != &value)
				{
					Allocation::copy(value);
				}
				return *this;
			}

			constexpr Value &operator = (Value &&value) noexcept
			{
				if (this != &value)
				{
					Allocation::move(value);
				}
				return *this;
			}

			constexpr Area<Type> getArea(size_t offset, size_t length)
			{
				if (offset > Allocation::getLength())
				{
					offset = Allocation::getLength();
				}

				if (length > Allocation::getLength() - offset)
				{
					length = Allocation::getLength() - offset;
				}

				return { &(*this)[offset], length };
			}

			constexpr Area<Type> getArea(size_t offset = 0)
			{
				if (offset > Allocation::getLength())
				{
					offset = Allocation::getLength();
				}

				return { &(*this)[offset], Allocation::getLength() - offset };
			}

			constexpr View<Type> getView(size_t offset, size_t length) const
			{
				if (offset > Allocation::getLength())
				{
					offset = Allocation::getLength();
				}

				if (length > Allocation::getLength() - offset)
				{
					length = Allocation::getLength() - offset;
				}

				return { &(*this)[offset], length };
			}

			constexpr View<Type> getView(size_t offset = 0) const
			{
				if (offset > Allocation::getLength())
				{
					offset = Allocation::getLength();
				}

				return { &(*this)[offset], Allocation::getLength() - offset };
			}

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Type *at(size_t offset)
			{
				return (offset < Allocation::getLength()) ? &(*this)[offset] : nullptr;
			}

			constexpr const Type *at(size_t offset) const
			{
				return (offset < Allocation::getLength()) ? &(*this)[offset] : nullptr;
			}

			constexpr const Type &getOr(size_t offset, const Type &defaultValue) const
			{
				return (offset < Allocation::getLength()) ? (*this)[offset] : defaultValue;
			}

			constexpr bool get(size_t offset, Type &value) const
			{
				if (offset < Allocation::getLength())
				{
					value = (*this)[offset];
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr bool set(size_t offset, const Type &value)
			{
				if ((offset < Allocation::getLength()) || ((offset == Allocation::getLength()) && Allocation::increaseLength(1)))
				{
					(*this)[offset] = value;
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr bool set(size_t offset, Type &&value)
			{
				if ((offset < Allocation::getLength()) || ((offset == Allocation::getLength()) && Allocation::increaseLength(1)))
				{
					(*this)[offset] = std::move(value);
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr bool set(size_t offset, View<Type> value)
			{
				size_t valueLength = value.getLength();

				if ((offset <= Allocation::getLength()) && ((valueLength <= Allocation::getLength() - offset) || Allocation::increaseLength(valueLength - Allocation::getLength() + offset)))
				{
					copyForward(offset, value);
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr void clear()
			{
				if (!Allocation::setLength(0))
				{
					Ash::Memory::clear(&(*this)[0], Allocation::getLength());
				}
			}

			constexpr bool insert(size_t offset, Type &&value)
			{
				if ((offset <= Allocation::getLength()) && Allocation::increaseLength(1))
				{
					shiftRight(offset, 1);
					(*this)[offset] = std::move(value);
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr bool insert(size_t offset, const Type &value)
			{
				if ((offset <= Allocation::getLength()) && Allocation::increaseLength(1))
				{
					shiftRight(offset, 1);
					(*this)[offset] = value;
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr bool insert(size_t offset, View<Type> value)
			{
				size_t valueLength = value.getLength();

				if (valueLength == 0)
				{
					return true;
				}
				else if ((offset <= Allocation::getLength()) && Allocation::increaseLength(valueLength))
				{
					shiftRight(offset, valueLength);
					copyForward(offset, value);
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr bool append(Type &&value)
			{
				return set(Allocation::getLength(), std::move(value));
			}

			constexpr bool append(const Type &value)
			{
				return set(Allocation::getLength(), value);
			}

			constexpr bool append(View<Type> value)
			{
				return set(Allocation::getLength(), value);
			}

			constexpr bool remove(size_t offset, size_t length = 1)
			{
				if ((offset >= Allocation::getLength()) || (length > Allocation::getLength() - offset))
				{
					return false;
				}

				if (length != 0)
				{
					shiftLeft(offset, length);
					if (!Allocation::decreaseLength(length))
					{
						for (size_t n = Allocation::getLength() - length; n < Allocation::getLength(); n++)
						{
							(*this)[n] = Type();
						}
					}
				}

				return true;
			}

			constexpr bool remove(size_t offset, Type &value)
			{
				if (offset >= Allocation::getLength())
				{
					return false;
				}

				value = std::move((*this)[offset]);

				return remove(offset);
			}

			constexpr size_t match(size_t offset, View<Type> value) const
			{
				size_t length = 0;

				if (offset < Allocation::getLength())
				{
					for (Type element: value)
					{
						if ((length >= Allocation::getLength() - offset) || (element != (*this)[offset + length]))
						{
							return length;
						}
						length++;
					}
				}

				return length;
			}

			constexpr size_t find(size_t offset, const Type &value) const
			{
				if (offset >= Allocation::getLength())
				{
					return Allocation::getLength();
				}

				if (!std::is_constant_evaluated())
				{
					if constexpr (Ash::Type::isByteSizeInteger<Type>)
					{
						const Type *location = (const Type *)memchr(&(*this)[offset], value, Allocation::getLength() - offset);
						return (location != nullptr) ? location - &(*this)[0] : Allocation::getLength();
					}
					else if constexpr (Ash::Type::isSame<Type, wchar_t>)
					{
						const Type *location = (const Type *)wmemchr(&(*this)[offset], value, Allocation::getLength() - offset);
						return (location != nullptr) ? location - &(*this)[0] : Allocation::getLength();
					}
				}

				for (; (offset < Allocation::getLength()) && ((*this)[offset] != value); offset++)
					;

				return offset;
			}

			constexpr size_t reverseFind(size_t offset, const Type &value) const
			{
				if (offset >= Allocation::getLength())
				{
					return Allocation::getLength();
				}

				for (; offset > 0; offset--)
				{
					if ((*this)[offset] == value)
					{
						return offset;
					}
				}

				return ((*this)[offset] == value) ? offset : Allocation::getLength();
			}

			template
			<
				size_t ALIGNMENT,
				size_t ALIGNMENT_OFFSET = 0
			>
			constexpr size_t find(size_t offset, const Type &value) const
			{
				if constexpr (ALIGNMENT > 1)
				{
					size_t alignment = offset - (offset % ALIGNMENT) + ALIGNMENT_OFFSET;
					if (alignment < offset)
					{
						if ((Allocation::getLength() < ALIGNMENT) || (alignment >= Allocation::getLength() - ALIGNMENT))
						{
							return Allocation::getLength();
						}
						alignment = alignment + ALIGNMENT;
					}
					else if (alignment >= Allocation::getLength())
					{
						return Allocation::getLength();
					}
					offset = alignment;

					for (; (offset < Allocation::getLength()) && ((*this)[offset] != value); offset = offset + ALIGNMENT)
						;

					return offset;
				}
				else
				{
					return find(offset, value);
				}
			}

			template
			<
				size_t ALIGNMENT,
				size_t ALIGNMENT_OFFSET = 0
			>
			constexpr size_t reverseFind(size_t offset, const Type &value) const
			{
				if constexpr (ALIGNMENT > 1)
				{
					size_t alignment = offset - (offset % ALIGNMENT) + ALIGNMENT_OFFSET;
					if (alignment > offset)
					{
						if ((alignment < ALIGNMENT) || (alignment - ALIGNMENT >= Allocation::getLength()))
						{
							return Allocation::getLength();
						}
						alignment = alignment - ALIGNMENT;
					}
					else if (alignment >= Allocation::getLength())
					{
						return Allocation::getLength();
					}
					offset = alignment;

					for (; offset >= ALIGNMENT; offset = offset - ALIGNMENT)
					{
						if ((*this)[offset] == value)
						{
							return offset;
						}
					}

					return ((*this)[offset] == value) ? offset : Allocation::getLength();
				}
				else
				{
					return reverseFind(offset, value);
				}
			}

			constexpr size_t find(size_t offset, View<Type> value) const
			{
				if ((value.getLength() > 0) && (value.getLength() <= Allocation::getLength()))
				{
					for (; offset <= Allocation::getLength() - value.getLength(); offset++)
					{
						if (match(offset, value) == value.getLength())
						{
							return offset;
						}
					}
				}

				return Allocation::getLength();
			}

			constexpr size_t reverseFind(size_t offset, View<Type> value) const
			{
				if ((value.getLength() > 0) && (value.getLength() <= Allocation::getLength() - value.getLength()))
				{
					for (; offset > 0; offset--)
					{
						if (match(offset, value) == value.getLength())
						{
							return offset;
						}
					}
					if (match(0, value) == value.getLength())
					{
						return 0;
					}
				}

				return Allocation::getLength();
			}

			constexpr void shiftLeft(size_t offset, size_t count)
			{
				if ((count > 0) && (offset < Allocation::getLength()) && (count < Allocation::getLength() - offset))
				{
					Ash::Memory::moveForward(&(*this)[offset], &(*this)[offset + count], Allocation::getLength() - count - offset);
				}
			}

			constexpr void shiftRight(size_t offset, size_t count)
			{
				if ((count > 0) && (offset < Allocation::getLength()) && (count < Allocation::getLength() - offset))
				{
					Ash::Memory::moveBackward(&(*this)[offset + count], &(*this)[offset], Allocation::getLength() - count - offset);
				}
			}

			constexpr void rotateLeft()
			{
				if (Allocation::getLength() > 1)
				{
					Type value = std::move((*this)[0]);
					shiftLeft(0, 1);
					(*this)[Allocation::getLength() - 1] = std::move(value);
				}
			}

			constexpr void rotateLeft(size_t count)
			{
				count = count % Allocation::getLength();
				if (count > 0)
				{
					Type value;
					Type *start = &(*this)[0];
					Type *end = &(*this)[Allocation::getLength() - count];
					Type *offset = start;
					Type *nextOffset = start + count;

					value = std::move(*offset);
					for (size_t n = 1; n < Allocation::getLength(); n++)
					{
						if (nextOffset == start)
						{
							*offset = std::move(value);
							nextOffset++;
							start = nextOffset;
							value = std::move(*offset);
						}
						else
						{
							*offset = std::move(*nextOffset);
						}
						offset = nextOffset;
						nextOffset = offset + ((offset < end) ? count : count - Allocation::getLength());
					}
					*offset = std::move(value);
				}
			}

			constexpr void rotateRight()
			{
				if (Allocation::getLength() > 1)
				{
					Type value = std::move((*this)[Allocation::getLength() - 1]);
					shiftRight(0, 1);
					(*this)[0] = std::move(value);
				}
			}

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Type *begin() { return (Allocation::getLength() != 0) ? &(*this)[0] : nullptr; }

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Type *end() { return (Allocation::getLength() != 0) ? &(*this)[Allocation::getLength()] : nullptr; }

			constexpr const Type *begin() const { return (Allocation::getLength() != 0) ? &(*this)[0] : nullptr; }

			constexpr const Type *end() const { return (Allocation::getLength() != 0) ? &(*this)[Allocation::getLength()] : nullptr; }

			using Iterate = Ash::Iterate<Type *>;

			using IterateConst = Ash::Iterate<const Type *>;

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Iterate iterate() { return Iterate::from(at(0), Allocation::getLength()); }

			constexpr IterateConst iterate() const { return IterateConst::from(at(0), Allocation::getLength()); }

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Iterate iterateBetween(size_t from, size_t to)
			{
				size_t count = 0;

				if (from < Allocation::getLength() && (from <= to))
				{
					count = (to < Allocation::getLength()) ? to - from + 1 : Allocation::getLength() - from + 1;
				}

				return Iterate::from(at(from), count);
			}

			constexpr IterateConst iterateBetween(size_t from, size_t to) const
			{
				size_t count = 0;

				if (from < Allocation::getLength() && (from <= to))
				{
					count = (to < Allocation::getLength()) ? to - from + 1 : Allocation::getLength() - from + 1;
				}

				return IterateConst::from(at(from), count);
			}

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Iterate iterateFrom(size_t from) { return iterateBetween(from, Allocation::getLength() - 1); }

			constexpr IterateConst iterateFrom(size_t from) const { return iterateBetween(from, Allocation::getLength() - 1); }

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Iterate iterateTo(size_t to) { return iterateBetween(0, to); }

			constexpr IterateConst iterateTo(size_t to) const { return iterateBetween(0, to); }

			using Cycle = Ash::Iterate<Type *, true, 2>;

			using CycleConst = Ash::Iterate<const Type *, true, 2>;

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Cycle cycleBetween(size_t from, size_t to)
			{
				if ((from >= Allocation::getLength()) || (to >= Allocation::getLength()))
				{
					return Iterate() + Iterate();
				}
				else if (from <= to)
				{
					return iterateBetween(from, to) + Iterate();
				}
				else
				{
					return iterateFrom(from) + iterateTo(to);
				}
			}

			constexpr CycleConst cycleBetween(size_t from, size_t to) const
			{
				if ((from >= Allocation::getLength()) || (to >= Allocation::getLength()))
				{
					return IterateConst() + IterateConst();
				}
				else if (from <= to)
				{
					return iterateBetween(from, to) + IterateConst();
				}
				else
				{
					return iterateFrom(from) + iterateTo(to);
				}
			}

			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Cycle cycleFrom(size_t from, size_t count)
			{
				if ((count == 0) || (from >= Allocation::getLength()) || (count > Allocation::getLength()))
				{
					return Iterate() + Iterate();
				}
				else if (from <= Allocation::getLength() - count)
				{
					return iterateBetween(from, from + count - 1) + Iterate();
				}
				else
				{
					return iterateFrom(from) + iterateTo(from + count - Allocation::getLength() - 1);
				}
			}

			constexpr CycleConst cycleFrom(size_t from, size_t count) const
			{
				if ((count == 0) || (from >= Allocation::getLength()) || (count > Allocation::getLength()))
				{
					return IterateConst() + IterateConst();
				}
				else if (from <= Allocation::getLength() - count)
				{
					return iterateBetween(from, from + count - 1) + IterateConst();
				}
				else
				{
					return iterateFrom(from) + iterateTo(from + count - Allocation::getLength() - 1);
				}
			}

		protected:
			template
			<
				typename ALLOCATION_TYPE = typename Allocation::Type,
				typename = Ash::Type::IsNotConstant<ALLOCATION_TYPE>
			>
			constexpr Type &operator [] (size_t offset)
			{
				return Allocation::getContent()[offset];
			}

			constexpr const Type &operator [] (size_t offset) const
			{
				return Allocation::getContent()[offset];
			}

			constexpr void copyForward(size_t offset, View<Type> value)
			{
				Ash::Memory::copyForward(&(*this)[offset], value.at(0), value.getLength());
			}
		};
	}
}
