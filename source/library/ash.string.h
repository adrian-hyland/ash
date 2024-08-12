#pragma once

#include <initializer_list>
#include <algorithm>
#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace String
	{
		template
		<
			typename ALLOCATION,
			typename ENCODING,
			typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		class Value;

		template
		<
			typename ENCODING,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		using View = Value<Ash::Memory::Allocation::Reference<const typename ENCODING::Code>, ENCODING>;

		template
		<
			typename ENCODING,
			size_t   CAPACITY,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		using Buffer = Value<Ash::Memory::Allocation::Buffer<typename ENCODING::Code, CAPACITY>, ENCODING>;

		template
		<
			typename ENCODING,
			size_t   MINIMUM_CAPACITY    = 32,
			size_t   PERCENTAGE_INCREASE = 50,
			size_t   BLOCK_SIZE          = 32,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		using Array = Value<Ash::Memory::Allocation::Array<typename ENCODING::Code, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>, ENCODING>;

		template
		<
			typename ENCODING,
			size_t   CAPACITY            = 32,
			size_t   PERCENTAGE_INCREASE = 50,
			size_t   BLOCK_SIZE          = 32,
			typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
		>
		using ArrayBuffer = Value<Ash::Memory::Allocation::ArrayBuffer<typename ENCODING::Code, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>, ENCODING>;

		template
		<
			typename ALLOCATION,
			typename ENCODING,
			typename,
			typename
		>
		class Value : public Ash::Memory::Value<ALLOCATION, typename ENCODING::Code>
		{
		public:
			using Allocation = ALLOCATION;
			using Encoding = ENCODING;
			using Code = typename Encoding::Code;
			using Character = typename Encoding::Character;
			using Memory = Ash::Memory::Value<Allocation, Code>;

			constexpr Value() : Memory() {}

			constexpr Value(const Code *value) : Memory(value, getCodeLength(value)) {}

			constexpr Value(const Code *value, size_t length) : Memory(value, getCodeLength(value, length)) {}

			constexpr Value(const Value &value) : Memory(value) {}

			constexpr Value(Value &&value) noexcept : Memory(std::move(value)) {}

			constexpr Value &operator = (const Value &value)
			{
				Memory::operator = (value);

				return *this;
			}

			constexpr Value &operator = (Value &&value) noexcept
			{
				Memory::operator = (std::move(value));

				return *this;
			}

			constexpr size_t getNextCharacter(size_t offset, Character &character) const
			{
				return Encoding::decodeNext(*this, offset, character);
			}

			constexpr size_t getPreviousCharacter(size_t offset, Character &character) const
			{
				return Encoding::decodePrevious(*this, offset, character);
			}

			template
			<
				typename TO_ALLOCATION,
				typename TO_ENCODING,
				typename = Ash::Type::IsClass<TO_ALLOCATION, Ash::Memory::Generic::Allocation>,
				typename = Ash::Type::IsClass<TO_ENCODING, Ash::Generic::Encoding>
			>
			constexpr size_t convertTo(Value<TO_ALLOCATION, TO_ENCODING> &value, Ash::Unicode::Character replacementCharacter = TO_ENCODING::Character::replacement) const
			{
				return Ash::Encoding::convert<ENCODING, TO_ENCODING>(*this, value, replacementCharacter);
			}

			constexpr size_t find(size_t offset, Ash::Unicode::Character character) const
			{
				return Ash::Encoding::find<ENCODING>(*this, offset, character);
			}

			constexpr bool contains(Ash::Unicode::Character character) const
			{
				return Ash::Encoding::contains<ENCODING>(*this, character);
			}

			constexpr size_t skipAnyOf(size_t offset, std::initializer_list<Ash::Unicode::Character> characterList) const
			{
				return Ash::Encoding::skipAnyOf<ENCODING>(*this, offset, characterList);
			}

			constexpr size_t skipNoneOf(size_t offset, std::initializer_list<Ash::Unicode::Character> characterList) const
			{
				return Ash::Encoding::skipNoneOf<ENCODING>(*this, offset, characterList);
			}

			template
			<
				typename TOKEN_ALLOCATION,
				typename = Ash::Type::IsClass<TOKEN_ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			constexpr size_t token(size_t offset, std::initializer_list<Ash::Unicode::Character> delimiters, Ash::String::Value<TOKEN_ALLOCATION, ENCODING> &tokenString) const
			{
				return Ash::Encoding::token<ENCODING>(*this, offset, delimiters, tokenString);
			}

		protected:
			static constexpr bool isNull(const Code *value)
			{
				if constexpr (Encoding::minSize == 1)
				{
					return value[0] == 0;
				}
				else if constexpr (Encoding::minSize == 2)
				{
					return (value[0] == 0) && (value[1] == 0);
				}
				else if constexpr (Encoding::minSize == 4)
				{
					return (value[0] == 0) && (value[1] == 0) && (value[2] == 0) && (value[3] == 0);
				}
				else
				{
					bool isNull = (value[0] == 0);
					for (size_t n = 1; isNull && (n < Encoding::minSize); n++)
					{
						isNull = isNull && (value[n] == 0);
					}
					return isNull;
				}
			}

			static constexpr size_t getCodeLength(const Code *value, size_t length)
			{
				if constexpr (Memory::isFixedCapacity && !Memory::isReference)
				{
					if (length > Memory::maxCapacity)
					{
						for (size_t offset = Memory::maxCapacity - Memory::maxCapacity % Encoding::minSize; offset != 0; offset = offset - Encoding::minSize)
						{
							Character character;
							if (Encoding::decodePrevious(Ash::Memory::View<Code>(value, length), offset, character) != 0)
							{
								return offset;
							}
						}

						return 0;
					}
				}

				return length;
			}

			static constexpr size_t getCodeLength(const Code *value)
			{
				size_t length = 0;

				for (length = 0; !isNull(&value[length]); length = length + Encoding::minSize)
					;

				return getCodeLength(value, length);
			}
		};
	}
}
