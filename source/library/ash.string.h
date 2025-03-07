#pragma once

#include <initializer_list>
#include <algorithm>
#include "ash.type.h"
#include "ash.encoding.h"
#include "ash.encoding.ascii.h"
#include "ash.encoding.utf8.h"
#include "ash.encoding.wide.h"


namespace Ash
{
	namespace String
	{
		template
		<
			typename LITERAL,
			typename = Ash::Type::IsStringLiteral<LITERAL>
		>
		struct Literal {};

		template
		<
		>
		struct Literal<const Ash::Encoding::Ascii::Code *>
		{
			using Encoding = Ash::Encoding::Ascii;

			using Code = Encoding::Code;

			using Value = const Code *;

			static constexpr size_t getLength(Value value)
			{
				size_t length = 0;

				if (value != nullptr)
				{
					if (!std::is_constant_evaluated())
					{
						length = ::strlen(value);
					}
					else
					{
						for (length = 0; value[length] != '\0'; length++) 
							;
					}
				}

				return length;
			}

			static constexpr size_t getLength(Value value, size_t length, size_t maxLength)
			{
				return length < maxLength ? length : maxLength;
			}

			static constexpr size_t getLength(Value value, size_t maxLength)
			{
				return getLength(value, getLength(value), maxLength);
			}

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t maxLength) { return { value, getLength(value, maxLength) }; }

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t length, size_t maxLength) { return { value, getLength(value, length, maxLength) }; }
		};

		template
		<
		>
		struct Literal<const Ash::Encoding::Utf8::Code *>
		{
			using Encoding = Ash::Encoding::Utf8;

			using Code = Encoding::Code;

			using Value = const Code *;

			static constexpr size_t getLength(Value value)
			{
				size_t length = 0;

				if (value != nullptr)
				{
					if (!std::is_constant_evaluated())
					{
						return ::strlen((const char *)value);
					}
					else
					{
						for (length = 0; value[length] != '\0'; length++) 
							;
					}
				}

				return length;
			}

			static constexpr size_t getLength(Value value, size_t length, size_t maxLength)
			{
				if (length < maxLength)
				{
					return length;
				}
				else
				{
					for (size_t offset = maxLength; offset != 0; offset = offset - Encoding::minSize)
					{
						Encoding::Character character;
						if (Encoding::decodePrevious(Ash::Memory::View<Code>(value, length), offset, character) != 0)
						{
							return offset;
						}
					}

					return 0;
				}
			}

			static constexpr size_t getLength(Value value, size_t maxLength)
			{
				return getLength(value, getLength(value), maxLength);
			}

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t maxLength) { return { value, getLength(value, maxLength) }; }

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t length, size_t maxLength) { return { value, getLength(value, length, maxLength) }; }
		};

		template
		<
		>
		struct Literal<const Ash::Encoding::Wide::Code *>
		{
			using Encoding = Ash::Encoding::Wide;

			using Code = Encoding::Code;

			using Value = const Code *;

			static constexpr size_t getLength(Value value)
			{
				size_t length = 0;

				if (value != nullptr)
				{
					if (!std::is_constant_evaluated())
					{
						return ::wcslen(value);
					}
					else
					{
						for (length = 0; value[length] != '\0'; length++) 
							;
					}
				}

				return length;
			}

			static constexpr size_t getLength(Value value, size_t length, size_t maxLength)
			{
				if (length < maxLength)
				{
					return length;
				}
				else if constexpr (Encoding::minSize == Encoding::maxSize)
				{
					return maxLength;
				}
				else
				{
					for (size_t offset = maxLength; offset != 0; offset = offset - Encoding::minSize)
					{
						Encoding::Character character;
						if (Encoding::decodePrevious(Ash::Memory::View<Code>(value, length), offset, character) != 0)
						{
							return offset;
						}
					}

					return 0;
				}
			}

			static constexpr size_t getLength(Value value, size_t maxLength)
			{
				return getLength(value, getLength(value), maxLength);
			}

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t maxLength) { return { value, getLength(value, maxLength) }; }

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t length, size_t maxLength) { return { value, getLength(value, length, maxLength) }; }
		};

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

			template
			<
				typename VALUE,
				typename STRING_ENCODING = ENCODING,
				typename = Ash::Type::IsStringLiteral<VALUE>,
				typename = Ash::Type::IsSame<typename Ash::String::Literal<VALUE>::Encoding, STRING_ENCODING>
			>
			constexpr Value(VALUE value) : Memory(value, Ash::String::Literal<VALUE>::getLength(value, Memory::maxCapacity)) {}

			template
			<
				typename VALUE,
				typename STRING_ENCODING = ENCODING,
				typename = Ash::Type::IsStringLiteral<VALUE>,
				typename = Ash::Type::IsSame<typename Ash::String::Literal<VALUE>::Encoding, STRING_ENCODING>
			>
			constexpr Value(VALUE value, size_t length) : Memory(value, Ash::String::Literal<VALUE>::getLength(value, length, Memory::maxCapacity)) {}

			template
			<
				typename VALUE,
				typename STRING_ALLOCATION = ALLOCATION,
				typename STRING_ENCODING = ENCODING,
				typename = Ash::Type::IsStringLiteral<VALUE>,
				typename = Ash::Type::IsNotSame<typename Ash::String::Literal<VALUE>::Encoding, STRING_ENCODING>,
				typename = Ash::Type::IsNotConstant<typename STRING_ALLOCATION::Type>
			>
			constexpr Value(VALUE value, Ash::Unicode::Character replacementCharacter = Character::replacement) : Memory()
			{
				Ash::Encoding::convert<typename Ash::String::Literal<VALUE>::Encoding, ENCODING>(Literal<VALUE>::getView(value, Memory::maxCapacity), *this, replacementCharacter);
			}

			template
			<
				typename VALUE,
				typename STRING_ALLOCATION = ALLOCATION,
				typename STRING_ENCODING = ENCODING,
				typename = Ash::Type::IsStringLiteral<VALUE>,
				typename = Ash::Type::IsNotSame<typename Ash::String::Literal<VALUE>::Encoding, STRING_ENCODING>,
				typename = Ash::Type::IsNotConstant<typename STRING_ALLOCATION::Type>
			>
			constexpr Value(VALUE value, size_t length, Ash::Unicode::Character replacementCharacter = Character::replacement) : Memory()
			{
				Ash::Encoding::convert<typename Ash::String::Literal<VALUE>::Encoding, ENCODING>(Literal<VALUE>::getView(value, length, Memory::maxCapacity), *this, replacementCharacter);
			}

			template
			<
				typename VALUE_ALLOCATION,
				typename VALUE_ENCODING,
				typename STRING_ALLOCATION = ALLOCATION,
				typename STRING_ENCODING = ENCODING,
				typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>,
				typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>,
				typename = Ash::Type::IsNotSame<VALUE_ENCODING, STRING_ENCODING>,
				typename = Ash::Type::IsNotConstant<typename STRING_ALLOCATION::Type>
			>
			constexpr Value(const Value<VALUE_ALLOCATION, VALUE_ENCODING> &value, Ash::Unicode::Character replacementCharacter = Character::replacement) : Memory()
			{
				Ash::Encoding::convert<VALUE_ENCODING, ENCODING>(value, *this, replacementCharacter);
			}

			template
			<
				typename VALUE_ALLOCATION,
				typename STRING_ALLOCATION = ALLOCATION,
				typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>,
				typename = Ash::Type::IsNotSame<VALUE_ALLOCATION, STRING_ALLOCATION>
			>
			constexpr Value(const Value<VALUE_ALLOCATION, Encoding> &value) : Memory(value.at(0), getCodeLength(value.at(0), value.getLength())) {}

			constexpr Value(Ash::Memory::View<Code> value) : Memory(value.at(0), getCodeLength(value.at(0), value.getLength())) {}

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

			constexpr View<Encoding> getView(size_t offset, size_t length) const
			{
				return Memory::getView(offset, length);
			}

			constexpr View<Encoding> getView(size_t offset = 0) const
			{
				return Memory::getView(offset);
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

			constexpr size_t reverseFind(size_t offset, Ash::Unicode::Character character) const
			{
				return Ash::Encoding::reverseFind<ENCODING>(*this, offset, character);
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
		};
	}
}
