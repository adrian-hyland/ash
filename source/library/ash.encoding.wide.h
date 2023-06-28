#pragma once

#include "ash.string.h"


namespace Ash
{
	namespace Encoding
	{
		class Wide
		{
		public:
			using Code = wchar_t;

			static constexpr size_t minSize = 1;

			static constexpr size_t maxSize = 4 / sizeof(wchar_t);

			class Character : public Memory::Buffer<Code, maxSize>
			{
			public:
				static constexpr Ash::Unicode::Character::Value replacement = Ash::Unicode::Character::replacement;

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
						return 0x10000 + (((*this)[0] & 0x3FF) << 10) + ((*this)[1] & 0x3FF);
					}
					else
					{
						return Ash::Unicode::Character::replacement;
					}
				}

			protected:
				constexpr bool set(Ash::Unicode::Character value)
				{
					if ((value < 0xD800) || (value > 0xDFFF))
					{
						if (value < 0x10000)
						{
							setLength(1);
							(*this)[0] = value;
							return true;
						}
						else if (value < 0x110000)
						{
							if (maxSize == 1)
							{
								setLength(1);
								(*this)[0] = value;
								return true;
							}
							else if (maxSize == 2)
							{
								value = value - 0x10000;
								setLength(2);
								(*this)[0] = (value >> 10) | 0xD800;
								(*this)[1] = (value & 0x3FF) | 0xDC00;
								return true;
							}
						}
					}
					
					clear();
					return false;
				}

				constexpr size_t set(Code code1)
				{
					setLength(1);
					(*this)[0] = code1;
					return 1;
				}

				constexpr size_t set(Code code1, Code code2)
				{
					setLength(2);
					(*this)[0] = code1;
					(*this)[1] = code2;
					return 2;
				}

				friend Wide;
			};

			template <typename ALLOCATION>
			static constexpr size_t decodeNext(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;

				if (maxSize == 1)
				{
					if (value.get(offset++, code1) && (code1 >= 0) && (code1 < 0x110000))
					{
						return character.set(code1);
					}
				}
				else if (maxSize == 2)
				{
					if (value.get(offset++, code1))
					{
						if ((code1 & 0xFC00) == 0xD800)
						{
							if (value.get(offset++, code2))
							{
								if ((code2 & 0xFC00) == 0xDC00)
								{
									return character.set(code1, code2);
								}
							}
						}
						else if ((code1 & 0xFC00) != 0xDC00)
						{
							return character.set(code1);
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

				if (maxSize == 1)
				{
					if ((offset > 0) && value.get(--offset, code1) && (code1 >= 0) && (code1 < 0x110000))
					{
						return character.set(code1);
					}
				}
				else if (maxSize == 2)
				{
					if ((offset > 0) && value.get(--offset, code1))
					{
						if ((code1 & 0xFC00) == 0xDC00)
						{
							if ((offset > 0) && value.get(--offset, code2))
							{
								if ((code2 & 0xFC00) == 0xD800)
								{
									return character.set(code2, code1);
								}
							}
						}
						else if ((code1 & 0xFC00) != 0xD800)
						{
							return character.set(code1);
						}
					}
				}

				character.clear();
				return 0;
			}

			template <typename ALLOCATION>
			using Value = String::Value<ALLOCATION, Wide>;
		
			template <size_t N>
			using Buffer = String::Buffer<Wide, N>;

			using View = String::View<Wide>;

			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			using String = String::Array<Wide, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

		private:	
			Wide();
		};
	}
}
