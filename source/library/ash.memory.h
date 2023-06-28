#pragma once

#include <utility>
#include <algorithm>
#include <type_traits>
#include "ash.size.h"


namespace Ash
{
	namespace Memory
	{
		namespace Allocation
		{
			template <typename TYPE, size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			class Dynamic
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = false;

				static constexpr bool isFixedCapacity = false;

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
						clear();
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

				constexpr void clear()
				{
					delete [] m_Content;

					m_Content = nullptr;
					m_Capacity = 0;
					m_Length = 0;
				}

			protected:
				constexpr Dynamic() : m_Content(nullptr), m_Length(0), m_Capacity(0) {}

				inline ~Dynamic()
				{
					delete [] m_Content;
				}

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Dynamic &value)
				{
					copy(value.m_Content, value.m_Length);
				}

				constexpr void move(Dynamic &value)
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

					for (size_t n = 0; n < length; n++)
					{
						m_Content[n] = content[n];
					}
				}

			private:
				static constexpr size_t minimumCapacity = Size(MINIMUM_CAPACITY).roundUp(BLOCK_SIZE).getValueOr(MINIMUM_CAPACITY);

				static constexpr size_t getIncreaseCapacity(size_t length)
				{
					if (length <= minimumCapacity)
					{
						return minimumCapacity;
					}

					Size capacity(length);

					if (length < 100 * PERCENTAGE_INCREASE)
					{
						capacity = capacity.multiply(100 + PERCENTAGE_INCREASE).divide(100);
					}
					else
					{
						capacity = capacity.divide(100).multiply(100 + PERCENTAGE_INCREASE);
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

					if (length < PERCENTAGE_INCREASE * (100 + PERCENTAGE_INCREASE))
					{
						capacity = capacity.multiply(100).divide(100 + PERCENTAGE_INCREASE);
					}
					else
					{
						capacity = capacity.divide(100 + PERCENTAGE_INCREASE).multiply(100);
					}

					return capacity.roundUp(BLOCK_SIZE).getValueOr(length);
				}

				constexpr bool newContent(size_t capacity, size_t length)
				{
					Type *content = new Type[capacity];

					for (size_t n = 0; n < ((length < m_Length) ? length : m_Length); n++)
					{
						content[n] = std::move(m_Content[n]);
					}

					delete [] m_Content;

					m_Content = content;
					m_Length = length;
					m_Capacity = capacity;

					return true;
				}

				Type  *m_Content;
				size_t m_Length;
				size_t m_Capacity;
			};

			template <typename TYPE, size_t N>
			class Fixed
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = false;

				static constexpr bool isFixedCapacity = true;

				constexpr size_t getCapacity() const { return N; }

				constexpr size_t getLength() const { return m_Length; }

				constexpr bool setLength(size_t length)
				{
					if ((length > N) || (m_Length > N))
					{
						return false;
					}

					for (size_t n = length; n < m_Length; n++)
					{
						m_Content[n] = Type();
					}

					m_Length = length;

					return true;
				}

				constexpr bool increaseLength(size_t length)
				{
					if ((length > N) || (m_Length > N - length))
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

					for (size_t n = m_Length; n < m_Length + length; n++)
					{
						m_Content[n] = Type();
					}

					return true;
				}

				constexpr void clear()
				{
					for (size_t n = 0; n < m_Length; n++)
					{
						m_Content[n] = Type();
					}

					m_Length = 0;
				}

			protected:
				constexpr Fixed() : m_Content(), m_Length(0) {}

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Fixed &value)
				{
					copy(value.m_Content, value.m_Length);
				}

				constexpr void move(Fixed &value)
				{
					move(value.m_Content, value.m_Length);
				}

				constexpr void copy(const Type *content, size_t length)
				{
					setLength((length < N) ? length : N);

					for (size_t n = 0; n < m_Length; n++)
					{
						m_Content[n] = content[n];
					}
				}

				constexpr void move(Type *content, size_t length)
				{
					setLength((length < N) ? length : N);

					for (size_t n = 0; n < m_Length; n++)
					{
						m_Content[n] = std::move(content[n]);
					}
				}

			private:
				Type   m_Content[N];
				size_t m_Length;
			};


			template <typename TYPE, size_t N>
			class Constant
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = true;

				static constexpr bool isFixedCapacity = true;

				constexpr size_t getCapacity() const { return N; }

				constexpr size_t getLength() const { return N; }

				constexpr bool setLength(size_t length) const { return length == N; }

				constexpr bool decreaseLength(size_t length) { return false; }

				constexpr bool increaseLength(size_t length) { return false; }

				constexpr void clear()
				{
					for (size_t n = 0; n < N; n++)
					{
						m_Content[n] = Type();
					}
				}

			protected:
				constexpr Constant() : m_Content() {}

				constexpr const Type *getContent() const
				{
					return m_Content;
				}

				constexpr Type *getContent()
				{
					return m_Content;
				}

				constexpr void copy(const Constant &value)
				{
					copy(value.m_Content, N);
				}

				constexpr void move(Constant &value)
				{
					move(value.m_Content, N);
				}

				constexpr void copy(const Type *content, size_t length)
				{
					if (length > N)
					{
						length = N;
					}

					for (size_t n = 0; n < length; n++)
					{
						m_Content[n] = content[n];
					}
					for (size_t n = length; n < N; n++)
					{
						m_Content[n] = Type();
					}
				}

				constexpr void move(Type *content, size_t length)
				{
					if (length > N)
					{
						length = N;
					}

					for (size_t n = 0; n < length; n++)
					{
						m_Content[n] = std::move(content[n]);
					}
					for (size_t n = length; n < N; n++)
					{
						m_Content[n] = Type();
					}
				}

			private:
				Type m_Content[N];
			};


			template <typename TYPE>
			class Area
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = true;

				static constexpr bool isFixedCapacity = true;

				constexpr size_t getCapacity() const { return m_Length; }
				
				constexpr size_t getLength() const { return m_Length; }

				constexpr bool setLength(size_t length) const { return length == m_Length; }

				constexpr bool decreaseLength(size_t length) { return false; }

				constexpr bool increaseLength(size_t length) { return false; }

				constexpr void clear()
				{
					for (size_t n = 0; n < m_Length; n++)
					{
						m_Content[n] = Type();
					}
				}

			protected:
				constexpr Area() : m_Content(nullptr), m_Length(0) {}

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Area &value)
				{
					copy(value.m_Content, value.m_Length);
				}

				constexpr void move(Area &value)
				{
					move(value.m_Content, value.m_Length);
				}

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

		template <typename ALLOCATION, typename TYPE=std::remove_cv_t<typename ALLOCATION::Type>>
		class Value;

		template <typename TYPE, size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
		using Array = Value<Allocation::Dynamic<TYPE, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>>;

		template <typename TYPE, size_t N>
		using Buffer = Value<Allocation::Fixed<TYPE, N>>;

		template <typename TYPE, size_t N>
		using Vector = Value<Allocation::Constant<TYPE, N>>;

		template <typename TYPE>
		using Area = Value<Allocation::Area<TYPE>>;

		template <typename TYPE>
		using View = Value<Allocation::Area<const TYPE>>;

		template <typename ALLOCATION, typename TYPE>
		class Value : public ALLOCATION
		{
		public:
			using Allocation = ALLOCATION;

			using Type = TYPE;

			constexpr Value() : Allocation() {}

			constexpr Value(std::initializer_list<Type> content) : Allocation() { Allocation::copy(content.begin(), content.size()); }

			template <typename T=typename Allocation::Type>
			constexpr Value(std::enable_if_t<!std::is_const<T>::value, T> *content, size_t length) : Allocation() { Allocation::copy(content, length); }

			constexpr Value(const Type *content, size_t length) : Allocation() { Allocation::copy(content, length); }

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

			constexpr operator Area<Type> () { return { &(*this)[0], Allocation::getLength() }; }

			constexpr operator View<Type> () const { return { &(*this)[0], Allocation::getLength() }; }

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

			constexpr Area<Type> getArea(size_t offset)
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

			constexpr View<Type> getView(size_t offset) const
			{
				if (offset > Allocation::getLength())
				{
					offset = Allocation::getLength();
				}

				return { &(*this)[offset], Allocation::getLength() - offset };
			}

			constexpr Type *at(size_t offset)
			{
				return (offset < Allocation::getLength()) ? &(*this)[offset] : nullptr;
			}

			constexpr const Type *at(size_t offset) const
			{
				return (offset < Allocation::getLength()) ? &(*this)[offset] : nullptr;
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

			template <typename VALUE_ALLOCATION>
			constexpr bool set(size_t offset, const Memory::Value<VALUE_ALLOCATION, Type> &value)
			{
				size_t valueLength = value.getLength();

				if ((offset <= Allocation::getLength()) && ((valueLength <= Allocation::getLength() - offset) || Allocation::increaseLength(valueLength - Allocation::getLength() + offset)))
				{
					copy(offset, value);
					return true;
				}
				else
				{
					return false;
				}
			}

			constexpr bool insert(size_t offset, Type &&value)
			{
				if ((offset < Allocation::getLength()) && Allocation::increaseLength(1))
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

			constexpr bool insert(size_t offset, Type &value)
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

			template <typename VALUE_ALLOCATION>
			constexpr bool insert(size_t offset, const Memory::Value<VALUE_ALLOCATION, Type> &value)
			{
				size_t valueLength = value.getLength();

				if (valueLength == 0)
				{
					return true;
				}
				else if ((offset <= Allocation::getLength()) && Allocation::increaseLength(valueLength))
				{
					shiftRight(offset, valueLength);
					copy(offset, value);
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

			template <typename VALUE_ALLOCATION>
			constexpr bool append(const Value<VALUE_ALLOCATION, Type> &value)
			{
				return set(Allocation::getLength(), value);
			}

			constexpr bool remove(size_t offset)
			{
				size_t length = Allocation::getLength();

				if (offset >= length)
				{
					return false;
				}

				shiftLeft(offset, 1);

				if (!Allocation::decreaseLength(1))
				{
					(*this)[length - 1] = Type();
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

			template <typename VALUE_ALLOCATION>
			constexpr size_t match(size_t offset, const Value<VALUE_ALLOCATION, Type> &value) const
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

			template <typename VALUE_ALLOCATION>
			constexpr size_t find(size_t offset, const Value<VALUE_ALLOCATION, Type> &value) const
			{
				if (value.getLength() <= Allocation::getLength())
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

			constexpr void shiftLeft(size_t offset, size_t count)
			{
				if ((count > 0) && (offset < Allocation::getLength()) && (count < Allocation::getLength() - offset))
				{
					for (size_t n = offset; n < Allocation::getLength() - count; n++)
					{
						(*this)[n] = std::move((*this)[n + count]);
					}
				}
			}

			constexpr void shiftRight(size_t offset, size_t count)
			{
				if ((count > 0) && (offset < Allocation::getLength()) && (count < Allocation::getLength() - offset))
				{
					for (size_t n = Allocation::getLength() - 1; n >= count + offset; n--)
					{
						(*this)[n] = std::move((*this)[n - count]);
					}
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

			constexpr Type *begin() { return (Allocation::getLength() != 0) ? &(*this)[0] : nullptr; }

			constexpr Type *end() { return (Allocation::getLength() != 0) ? &(*this)[Allocation::getLength()] : nullptr; }

			constexpr const Type *begin() const { return (Allocation::getLength() != 0) ? &(*this)[0] : nullptr; }

			constexpr const Type *end() const { return (Allocation::getLength() != 0) ? &(*this)[Allocation::getLength()] : nullptr; }

		protected:
			constexpr Type &operator [] (size_t offset)
			{
				return Allocation::getContent()[offset];
			}

			constexpr const Type &operator [] (size_t offset) const
			{
				return Allocation::getContent()[offset];
			}

			template <typename FROM_ALLOCATION>
			constexpr void copy(size_t offset, const Memory::Value<FROM_ALLOCATION, Type> &value)
			{
				std::copy(value.begin(), value.end(), &(*this)[offset]);
			}
		};
	}
}
