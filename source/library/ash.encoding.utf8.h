#pragma once

#include "ash.string.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf8
		{
		public:
			using Code = uint8_t;

			static constexpr size_t minSize = 1;

			static constexpr size_t maxSize = 4;

			class Character : public Memory::Buffer<Code, maxSize>
			{
			public:
				static constexpr Ash::Unicode::Character replacement = Ash::Unicode::Character::replacement;

				constexpr Character() : Memory::Buffer<Code, maxSize>() {}

				constexpr Character(Ash::Unicode::Character value) : Memory::Buffer<Code, maxSize>()
				{
					set(value);
				}

				constexpr Ash::Unicode::Character operator = (Ash::Unicode::Character value)
				{
					set(value);
					return value;
				}

				constexpr operator Ash::Unicode::Character () const
				{
					if (getLength() == 1)
					{
						return (*this)[0];
					}
					else if (getLength() == 2)
					{
						return (((*this)[0] & 0x1F) << 6) | ((*this)[1] & 0x3F);
					}
					else if (getLength() == 3)
					{
						return (((*this)[0] & 0x0F) << 12) | (((*this)[1] & 0x3F) << 6) | ((*this)[2] & 0x3F);
					}
					else if (getLength() == 4)
					{
						return (((*this)[0] & 0x0F) << 18) | (((*this)[1] & 0x3F) << 12) | (((*this)[2] & 0x3F) << 6) | ((*this)[3] & 0x3F);
					}
					else
					{
						return Ash::Unicode::Character::replacement;
					}
				}

			protected:
				constexpr bool set(Ash::Unicode::Character value)
				{
					if (value < 0x80)
					{
						setLength(1);
						(*this)[0] = value;
					}
					else if (value < 0x0800)
					{
						setLength(2);
						(*this)[0] = (value >> 6) | 0xC0;
						(*this)[1] = (value & 0x3F) | 0x80;
					}
					else if (value < 0x10000)
					{
						if ((value >= 0xD800) && (value <= 0xDFFF))
						{
							clear();
							return false;
						}
							
						setLength(3);
						(*this)[0] = (value >> 12) | 0xE0;
						(*this)[1] = ((value >> 6) & 0x3F) | 0x80;
						(*this)[2] = (value & 0x3F) | 0x80;
					}
					else
					{
						setLength(4);
						(*this)[0] = (value >> 18) | 0xF0;
						(*this)[1] = ((value >> 12) & 0x3F) | 0x80;
						(*this)[2] = ((value >> 6) & 0x3F) | 0x80;
						(*this)[3] = (value & 0x3F) | 0x80;
					}

					return true;
				}

				constexpr size_t set(Code code1)
				{
					setLength(1);
					(*this)[0] = code1;
					return 1;
				}

				constexpr size_t set(Code code1, Code code2)
				{
					if (code1 > 0xC1)
					{
						setLength(2);
						(*this)[0] = code1;
						(*this)[1] = code2;
						return 2;
					}

					clear();
					return 0;
				}

				constexpr size_t set(Code code1, Code code2, Code code3)
				{
					if (((code1 != 0xE0) || (code2 > 0x9F)) && ((code1 != 0xED) || (code2 < 0xA0)))
					{
						setLength(3);
						(*this)[0] = code1;
						(*this)[1] = code2;
						(*this)[2] = code3;
						return 3;
					}

					clear();
					return 0;
				}

				constexpr size_t set(Code code1, Code code2, Code code3, Code code4)
				{
					if (((code1 != 0xF0) || (code2 > 0x8F)) && ((code1 != 0xF4) || (code2 < 0x90)) && (code1 < 0xF5))
					{
						setLength(4);
						(*this)[0] = code1;
						(*this)[1] = code2;
						(*this)[2] = code3;
						(*this)[3] = code4;
						return 4;
					}

					clear();
					return 0;
				}

				friend Utf8;
			};

			template <typename ALLOCATION>
			static constexpr size_t decodeNext(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				if (value.get(offset++, code1))
				{
					if ((code1 & 0x80) == 0x00)
					{
						return character.set(code1);
					}
					else if (value.get(offset++, code2) && ((code2 & 0xC0) == 0x80))
					{
						if ((code1 & 0xE0) == 0xC0)
						{
							return character.set(code1, code2);
						}
						else if (value.get(offset++, code3) && ((code3 & 0xC0) == 0x80))
						{
							if ((code1 & 0xF0) == 0xE0)
							{
								return character.set(code1, code2, code3);
							}
							else if (value.get(offset, code4) && ((code4 & 0xC0) == 0x80))
							{
								if ((code1 & 0xF8) == 0xF0)
								{
									return character.set(code1, code2, code3, code4);
								}
							}
						}
					}
				}

				character.clear();
				return 0;
			}

			template <typename ALLOCATION>
			static constexpr size_t decodePrevious(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				if ((offset > 0) && value.get(--offset, code1))
				{
					if ((code1 & 0x80) == 0x00)
					{
						return character.set(code1);
					}
					else if (((code1 & 0xC0) == 0x80) && (offset > 0) && value.get(--offset, code2))
					{
						if ((code2 & 0xE0) == 0xC0)
						{
							return character.set(code2, code1);
						}
						else if (((code2 & 0xC0) == 0x80) && (offset > 0) && value.get(--offset, code3))
						{
							if ((code3 & 0xF0) == 0xE0)
							{
								return character.set(code3, code2, code1);
							}
							else if (((code3 & 0xC0) == 0x80) && (offset > 0) && value.get(--offset, code4))
							{
								if ((code4 & 0xF8) == 0xF0)
								{
									return character.set(code4, code3, code2, code1);
								}
							}
						}
					}
				}

				character.clear();
				return 0;
			}

			template <typename ALLOCATION>
			using Value = Ash::String::Value<ALLOCATION, Utf8>;

			template <size_t CAPACITY>
			using Buffer = Ash::String::Buffer<Utf8, CAPACITY>;

			using View = Ash::String::View<Utf8>;

			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			using String = Ash::String::Array<Utf8, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

		private:
			Utf8();
		};
	}
}
