#pragma once

#include <initializer_list>
#include <algorithm>
#include "ash.encoding.h"


namespace Ash
{
	namespace String
	{
		template <typename ALLOCATION, typename ENCODING>
		class Value;

		template <typename ENCODING>
		using View = Value<Memory::Allocation::Reference<const typename ENCODING::Code>, ENCODING>;

		template <typename ENCODING, size_t CAPACITY>
		using Buffer = Value<Memory::Allocation::VariableLength<typename ENCODING::Code, CAPACITY>, ENCODING>;

		template <typename ENCODING, size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
		using Array = Value<Memory::Allocation::Dynamic<typename ENCODING::Code, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>, ENCODING>;

		template <typename ALLOCATION, typename ENCODING>
		class Value : public Memory::Value<ALLOCATION, typename ENCODING::Code>
		{
		public:
			using Allocation = ALLOCATION;
			using Encoding = ENCODING;
			using Code = typename Encoding::Code;
			using Character = typename Encoding::Character;
			using Memory = Memory::Value<Allocation, Code>;

			constexpr Value() : Memory() {}

			// TODO: this could truncate the value (for fixed capacity values)
			//       what should happen if the truncation is in the middle of a code point? should we allow truncation but truncate at appropriate code point?
			constexpr Value(const Code *value) : Memory(value, getStringLength(value)) {}

			constexpr Value(const Code *value, size_t length) : Memory(value, length) {}

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

			template <typename TO_ALLOCATION, typename TO_ENCODING>
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

			template <typename TOKEN_ALLOCATION>
			constexpr size_t token(size_t offset, std::initializer_list<Ash::Unicode::Character> delimiters, Ash::String::Value<TOKEN_ALLOCATION, ENCODING> &tokenString) const
			{
				return Ash::Encoding::token<ENCODING>(*this, offset, delimiters, tokenString);
			}

		protected:
			static constexpr size_t getStringLength(const Code *value)
			{
				size_t length = 0;

				while (value[length] != '\0')
				{
					length++;
				}

				return length;
			}
		};
	}
}
