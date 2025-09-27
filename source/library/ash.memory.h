#pragma once

#include <utility>
#include <algorithm>
#include <cwchar>
#include "ash.iterate.h"
#include "ash.memory.allocation.array.h"
#include "ash.memory.allocation.arraybuffer.h"
#include "ash.memory.allocation.buffer.h"
#include "ash.memory.allocation.reference.h"
#include "ash.memory.allocation.sequence.h"


namespace Ash
{
	namespace Memory
	{
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
				if (Allocation::getLength() > 1)
				{
					count = count % Allocation::getLength();
					if (count == 1)
					{
						rotateLeft();
					}
					else if (count > 1)
					{
						Type value;
						size_t start = 0;
						size_t end = Allocation::getLength() - count;
						size_t offset = start;
						size_t nextOffset = start + count;

						value = std::move((*this)[offset]);
						for (size_t n = 1; n < Allocation::getLength(); n++)
						{
							if (nextOffset == start)
							{
								(*this)[offset] = std::move(value);
								nextOffset++;
								start = nextOffset;
								value = std::move((*this)[nextOffset]);
							}
							else
							{
								(*this)[offset] = std::move((*this)[nextOffset]);
							}
							offset = nextOffset;
							nextOffset = offset + ((offset < end) ? count : count - Allocation::getLength());
						}
						(*this)[offset] = std::move(value);
					}
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

			constexpr void rotateRight(size_t count)
			{
				if (Allocation::getLength() > 1)
				{
					count = count % Allocation::getLength();
					if (count == 1)
					{
						rotateRight();
					}
					else if (count > 1)
					{
						rotateLeft(Allocation::getLength() - count);
					}
				}
			}

			constexpr void reverse()
			{
				if (Allocation::getLength() > 1)
				{
					for (size_t leftOffset = 0, rightOffset = Allocation::getLength() - 1; leftOffset < rightOffset; leftOffset++, rightOffset--)
					{
						Type value = std::move((*this)[leftOffset]);
						(*this)[leftOffset] = std::move((*this)[rightOffset]);
						(*this)[rightOffset] = std::move(value);
					}
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
