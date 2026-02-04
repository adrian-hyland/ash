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

			static constexpr size_t getLength(Value value, size_t length)
			{
				return (value != nullptr) ? length : 0;
			}

			static constexpr Ash::Memory::View<Code> getView(Value value) { return { value, getLength(value) }; }

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t length) { return { value, getLength(value, length) }; }
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

			static constexpr size_t getLength(Value value, size_t length)
			{
				if (value != nullptr)
				{
					for (size_t offset = length; offset != 0; offset = offset - Encoding::minSize)
					{
						Encoding::Character character;
						if (!Ash::Error::isSet(Encoding::decodePrevious(Ash::Memory::View<Code>(value, length), offset, character)))
						{
							return offset;
						}
					}
				}

				return 0;
			}

			static constexpr Ash::Memory::View<Code> getView(Value value) { return { value, getLength(value) }; }

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t length) { return { value, getLength(value, length) }; }
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

			static constexpr size_t getLength(Value value, size_t length)
			{
				if (value != nullptr)
				{
					for (size_t offset = length; offset != 0; offset = offset - Encoding::minSize)
					{
						Encoding::Character character;

						if (!Ash::Error::isSet(Encoding::decodePrevious(Ash::Memory::View<Code>(value, length), offset, character)))
						{
							return offset;
						}
					}
				}

				return 0;
			}

			static constexpr Ash::Memory::View<Code> getView(Value value) { return { value, getLength(value) }; }

			static constexpr Ash::Memory::View<Code> getView(Value value, size_t length) { return { value, getLength(value, length) }; }
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
			constexpr Value(VALUE value) : Memory(Ash::String::Literal<VALUE>::getView(value)) {}

			template
			<
				typename VALUE,
				typename STRING_ENCODING = ENCODING,
				typename = Ash::Type::IsStringLiteral<VALUE>,
				typename = Ash::Type::IsSame<typename Ash::String::Literal<VALUE>::Encoding, STRING_ENCODING>
			>
			constexpr Value(VALUE value, size_t length) : Memory(Ash::String::Literal<VALUE>::getView(value, length)) {}

			template
			<
				typename VALUE,
				typename STRING_ALLOCATION = ALLOCATION,
				typename STRING_ENCODING = ENCODING,
				typename = Ash::Type::IsStringLiteral<VALUE>,
				typename = Ash::Type::IsNotSame<typename Ash::String::Literal<VALUE>::Encoding, STRING_ENCODING>,
				typename = Ash::Type::IsNotConstant<typename STRING_ALLOCATION::Type>
			>
			constexpr Value(VALUE value, bool replaceInvalidCharacter = true) : Memory()
			{
				Ash::Encoding::convert<typename Ash::String::Literal<VALUE>::Encoding, Encoding>(Ash::String::Literal<VALUE>::getView(value), *this, replaceInvalidCharacter).throwOnError();
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
			constexpr Value(VALUE value, size_t length, bool replaceInvalidCharacter = true) : Memory()
			{
				Ash::Encoding::convert<typename Ash::String::Literal<VALUE>::Encoding, Encoding>(Ash::String::Literal<VALUE>::getView(value, length), *this, replaceInvalidCharacter).throwOnError();
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
			constexpr Value(const Value<VALUE_ALLOCATION, VALUE_ENCODING> &value, bool replaceInvalidCharacter = true) : Memory()
			{
				Ash::Encoding::convert<VALUE_ENCODING, Encoding>(value, *this, replaceInvalidCharacter).throwOnError();
			}

			template
			<
				typename VALUE_ALLOCATION,
				typename STRING_ALLOCATION = ALLOCATION,
				typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>,
				typename = Ash::Type::IsNotSame<VALUE_ALLOCATION, STRING_ALLOCATION>
			>
			constexpr Value(const Value<VALUE_ALLOCATION, Encoding> &value) : Memory(value) {}

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

			constexpr Ash::String::View<Encoding> getView(size_t offset, size_t length) const
			{
				return Memory::getView(offset, length);
			}

			constexpr Ash::String::View<Encoding> getView(size_t offset = 0) const
			{
				return Memory::getView(offset);
			}

			[[nodiscard]]
			constexpr Ash::Error::Value insert(size_t offset, Ash::Unicode::Character character, bool replaceInvalidCharacter = true)
			{
				Character encodedCharacter;
				Ash::Error::Value error = encodedCharacter.set(character, replaceInvalidCharacter);
				if (!error)
				{
					error = Memory::insert(offset, encodedCharacter);
				}

				return error;
			}

			[[nodiscard]]
			constexpr Ash::Error::Value insert(size_t offset, Ash::String::View<Encoding> value)
			{
				return Memory::insert(offset, value);
			}

			[[nodiscard]]
			constexpr Ash::Error::Value append(Ash::Unicode::Character character, bool replaceInvalidCharacter = true)
			{
				Character encodedCharacter;
				Ash::Error::Value error = encodedCharacter.set(character, replaceInvalidCharacter);
				if (!error)
				{
					error = Memory::append(encodedCharacter);
				}

				return error;
			}

			[[nodiscard]]
			constexpr Ash::Error::Value append(Ash::String::View<Encoding> value)
			{
				return Memory::append(value);
			}

			template
			<
				typename VALUE_ENCODING,
				typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>,
				typename = Ash::Type::IsNotSame<VALUE_ENCODING, ENCODING>
			>
			[[nodiscard]]
			constexpr Ash::Error::Value append(Ash::String::View<VALUE_ENCODING> value, bool replaceInvalidCharacter = true)
			{
				size_t offset = Memory::getLength();
				return Ash::Encoding::convert<VALUE_ENCODING, Encoding>(value, *this, offset, replaceInvalidCharacter);
			}

			[[nodiscard]]
			constexpr Ash::Error::Value getNextCharacter(size_t offset, Character &character) const
			{
				return Encoding::decodeNext(*this, offset, character);
			}

			[[nodiscard]]
			constexpr Ash::Error::Value getPreviousCharacter(size_t offset, Character &character) const
			{
				return Encoding::decodePrevious(*this, offset, character);
			}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsStringLiteral<VALUE>
			>
			[[nodiscard]]
			constexpr Ash::Error::Value convertFrom(VALUE value, bool replaceInvalidCharacter = true)
			{
				if constexpr (Ash::Type::isSame<typename Ash::String::Literal<VALUE>::Encoding, Encoding>)
				{
					return Memory::copyFrom(Ash::String::Literal<VALUE>::getView(value));
				}
				else
				{
					return Ash::Encoding::convert<typename Ash::String::Literal<VALUE>::Encoding, Encoding>(Ash::String::Literal<VALUE>::getView(value), *this, replaceInvalidCharacter);
				}
			}

			template
			<
				typename FROM_ENCODING,
				typename = Ash::Type::IsClass<FROM_ENCODING, Ash::Generic::Encoding>
			>
			[[nodiscard]]
			constexpr Ash::Error::Value convertFrom(Ash::String::View<FROM_ENCODING> value, bool replaceInvalidCharacter = true)
			{
				if constexpr (Ash::Type::isSame<FROM_ENCODING, Encoding>)
				{
					return Memory::copyFrom(value);
				}
				else
				{
					return Ash::Encoding::convert<FROM_ENCODING, Encoding>(value, *this, replaceInvalidCharacter);
				}
			}

			constexpr size_t find(size_t offset, Ash::Unicode::Character character) const
			{
				return Ash::Encoding::find<Encoding>(*this, offset, character);
			}

			constexpr size_t reverseFind(size_t offset, Ash::Unicode::Character character) const
			{
				return Ash::Encoding::reverseFind<Encoding>(*this, offset, character);
			}

			constexpr bool contains(Ash::Unicode::Character character) const
			{
				return Ash::Encoding::contains<Encoding>(*this, character);
			}

			[[nodiscard]]
			constexpr Ash::Error::Value skipAnyOf(size_t &offset, std::initializer_list<Ash::Unicode::Character> characterList) const
			{
				return Ash::Encoding::skipAnyOf<Encoding>(*this, offset, characterList, false);
			}

			[[nodiscard]]
			constexpr Ash::Error::Value skipNoneOf(size_t &offset, std::initializer_list<Ash::Unicode::Character> characterList) const
			{
				return Ash::Encoding::skipNoneOf<Encoding>(*this, offset, characterList, false);
			}

			template
			<
				typename TOKEN_ALLOCATION,
				typename = Ash::Type::IsClass<TOKEN_ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			[[nodiscard]]
			constexpr Ash::Error::Value token(size_t &offset, std::initializer_list<Ash::Unicode::Character> delimiters, Ash::String::Value<TOKEN_ALLOCATION, Encoding> &tokenString) const
			{
				return Ash::Encoding::token<Encoding>(*this, offset, delimiters, tokenString, false);
			}

		protected:
			constexpr Value(Ash::Memory::View<Code> value) : Memory(value) {}

		private:
			template
			<
				typename FRIEND_ALLOCATION,
				typename FRIEND_ENCODING,
				typename,
				typename
			>
			friend class Value;
		};

		template
		<
			typename VALUE,
			typename = Ash::Type::IsStringLiteral<VALUE>
		>
		constexpr Ash::String::View<typename Ash::String::Literal<VALUE>::Encoding> view(VALUE value)
		{
			return value;
		}
	}
}
