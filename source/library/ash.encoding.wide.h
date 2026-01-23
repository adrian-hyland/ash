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

			class Character : public Ash::Memory::Buffer<Code, maxSize>
			{
			public:
				constexpr Character() : Ash::Memory::Buffer<Code, maxSize>() {}

				constexpr Character(Ash::Unicode::Character character, bool replaceInvalidCharacter = true) : Ash::Memory::Buffer<Code, maxSize>()
				{
					set(character, replaceInvalidCharacter).assertErrorNotSet();
				}

				constexpr Ash::Unicode::Character operator = (Ash::Unicode::Character character)
				{
					set(character).assertErrorNotSet();
					return character;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Ash::Unicode::Character character, bool replaceInvalidCharacter = true)
				{
					Ash::Unicode::Character::Value value = character;

					if constexpr (maxSize == 1)
					{
						setLength(maxSize).assertErrorNotSet();
						(*this)[0] = value;
					}
					else if constexpr (maxSize == 2)
					{
						if (value < 0x10000)
						{
							setLength(minSize).assertErrorNotSet();
							(*this)[0] = value;
						}
						else
						{
							value = value - 0x10000;
							setLength(maxSize).assertErrorNotSet();
							(*this)[0] = (value >> 10) | 0xD800;
							(*this)[1] = (value & 0x3FF) | 0xDC00;
						}
					}

					return Ash::Error::none;
				}

				constexpr operator Ash::Unicode::Character () const
				{
					switch (getLength())
					{
						case 0:
							return '\0';
						break;

						case 1:
							return getCode(0);
						break;

						case 2:
							if constexpr (maxSize == 2)
							{
								return 0x10000 + ((getCode(0) & 0x3FF) << 10) + (getCode(1) & 0x3FF);
							}
							else
							{
								return Ash::Unicode::Character::replacement;
							}
						break;

						default:
							return Ash::Unicode::Character::replacement;
						break;
					}
				}

				static constexpr bool isValid(Ash::Unicode::Character character)
				{
					return true;
				}

			protected:
				constexpr Ash::Unicode::Character::Value getCode(size_t offset) const { return Ash::Unicode::Character::Value((*this)[offset]); }

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1)
				{
					if constexpr (maxSize == 1)
					{
						if ((code1 < 0) || (code1 >= 0x110000) || ((code1 >= 0xD800) && (code1 < 0xE000)))
						{
							clear();
							return Ash::Encoding::Error::invalidCodePoint;
						}
					}

					setLength(minSize).assertErrorNotSet();
					(*this)[0] = code1;
					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1, Code code2)
				{
					setLength(maxSize).assertErrorNotSet();
					(*this)[0] = code1;
					(*this)[1] = code2;
					return Ash::Error::none;
				}

				friend Wide;
			};

			[[nodiscard]]
			static constexpr Ash::Error::Value decodeNext(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Ash::Error::Value error = Ash::Error::none;

				if constexpr (maxSize == 1)
				{
					error = getNextCode(value, offset, code1);
					if (!error)
					{
						return character.set(code1);
					}
				}
				else if constexpr (maxSize == 2)
				{
					error = getNextCode(value, offset, code1);
					if (!error)
					{
						if ((code1 & 0xFC00) == 0xD800)
						{
							error = getNextCode(value, offset, code2);
							if (!error)
							{
								if ((code2 & 0xFC00) == 0xDC00)
								{
									return character.set(code1, code2);
								}
								else
								{
									error = Ash::Encoding::Error::invalidCodeUnit;
								}
							}
						}
						else if ((code1 & 0xFC00) != 0xDC00)
						{
							return character.set(code1);
						}
						else
						{
							error = Ash::Encoding::Error::invalidCodeUnit;
						}
					}
				}

				character.clear();
				return error;
			}

			[[nodiscard]]
			static constexpr Ash::Error::Value decodePrevious(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Ash::Error::Value error = Ash::Error::none;

				if constexpr (maxSize == 1)
				{
					error = getPreviousCode(value, offset, code1);
					if (!error)
					{
						return character.set(code1);
					}
				}
				else if constexpr (maxSize == 2)
				{
					error = getPreviousCode(value, offset, code1);
					if (!error)
					{
						if ((code1 & 0xFC00) == 0xDC00)
						{
							error = getPreviousCode(value, offset, code2);
							if (!error)
							{
								if ((code2 & 0xFC00) == 0xD800)
								{
									return character.set(code2, code1);
								}
								else
								{
									error = Ash::Encoding::Error::invalidCodeUnit;
								}
							}
						}
						else if ((code1 & 0xFC00) != 0xD800)
						{
							return character.set(code1);
						}
						else
						{
							error = Ash::Encoding::Error::invalidCodeUnit;
						}
					}
				}

				character.clear();
				return error;
			}

		protected:
			[[nodiscard]]
			static constexpr Ash::Error::Value getNextCode(Ash::Memory::View<Code> value, size_t &offset, Code &code)
			{
				return value.get(offset++, code);
			}

			[[nodiscard]]
			static constexpr Ash::Error::Value getPreviousCode(Ash::Memory::View<Code> value, size_t &offset, Code &code)
			{
				return (offset != 0) ? value.get(--offset, code) : Ash::Memory::Error::readAccessOutOfBound;
			}

		private:
			Wide();
		};
	}
}
