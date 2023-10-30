#pragma once

#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		class Wide : Ash::Generic::Encoding
		{
		public:
			using Code = Ash::Encoding::CodeUnitWide;

			static constexpr size_t minSize = 1;

			static constexpr size_t maxSize = 4 / sizeof(Code);

			static constexpr bool isLittleEndian = false;

			static constexpr bool isBigEndian = false;

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
						return getCode(0);
					}
					else if (getLength() == 2)
					{
						return 0x10000 + ((getCode(0) & 0x3FF) << 10) + (getCode(1) & 0x3FF);
					}
					else
					{
						return Ash::Unicode::Character::replacement;
					}
				}

			protected:
				constexpr Ash::Unicode::Character::Value getCode(size_t offset) const { return Ash::Unicode::Character::Value((*this)[offset]); }

				constexpr bool set(Ash::Unicode::Character value)
				{
					if ((value < 0xD800) || (value > 0xDFFF))
					{
						if constexpr (maxSize == 1)
						{
							setLength(1);
							(*this)[0] = value;
							return true;
						}
						else if constexpr (maxSize == 2)
						{
							if (value < 0x10000)
							{
								setLength(1);
								(*this)[0] = value;
							}
							else
							{
								value = value - 0x10000;
								setLength(2);
								(*this)[0] = (value >> 10) | 0xD800;
								(*this)[1] = (value & 0x3FF) | 0xDC00;
							}
							return true;
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

			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			static constexpr size_t decodeNext(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;

				if constexpr (maxSize == 1)
				{
					if (value.get(offset++, code1) && (code1 >= 0) && (code1 < 0x110000))
					{
						return character.set(code1);
					}
				}
				else if constexpr (maxSize == 2)
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

			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			static constexpr size_t decodePrevious(const Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;

				if constexpr (maxSize == 1)
				{
					if ((offset > 0) && value.get(--offset, code1) && (code1 >= 0) && (code1 < 0x110000))
					{
						return character.set(code1);
					}
				}
				else if constexpr (maxSize == 2)
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

		private:	
			Wide();
		};
	}
}
