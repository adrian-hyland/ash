#pragma once

#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf16be : Ash::Generic::Encoding
		{
		public:
			using Code = Ash::Encoding::CodeUnit8;

			static constexpr size_t minSize = 2;

			static constexpr size_t maxSize = 4;

			static constexpr bool isLittleEndian = false;

			static constexpr bool isBigEndian = true;

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

					if (value < 0x10000)
					{
						setLength(minSize).assertErrorNotSet();
						(*this)[0] = value >> 8;
						(*this)[1] = value & 0xFF;
					}
					else
					{
						value = value - 0x10000;
						setLength(maxSize).assertErrorNotSet();
						(*this)[0] = ((value >> 18) & 0x03) | 0xD8;
						(*this)[1] = value >> 10;
						(*this)[2] = ((value >> 8) & 0x03) | 0xDC;
						(*this)[3] = value;
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

						case 2:
							return (getCode(0) << 8) + getCode(1);
						break;

						case 4:
							return 0x10000 + ((getCode(0) & 0x03) << 18) + (getCode(1) << 10) + ((getCode(2) & 0x03) << 8) + getCode(3);
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
				constexpr Ash::Unicode::Character::Value getCode(size_t offset) const { return Ash::Unicode::Character::Value(uint8_t((*this)[offset])); }

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1, Code code2)
				{
					setLength(minSize).assertErrorNotSet();
					(*this)[0] = code1;
					(*this)[1] = code2;
					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1, Code code2, Code code3, Code code4)
				{
					setLength(maxSize).assertErrorNotSet();
					(*this)[0] = code1;
					(*this)[1] = code2;
					(*this)[2] = code3;
					(*this)[3] = code4;
					return Ash::Error::none;
				}

				friend Utf16be;
			};

			[[nodiscard]]
			static constexpr Ash::Error::Value decodeNext(Ash::Memory::View<Code> value, size_t offset, Character &character)
			{
				Code code1 = 0;
				Code code2 = 0;
				Code code3 = 0;
				Code code4 = 0;

				Ash::Error::Value error = getNextCode(value, offset, code1);
				if (!error)
				{
					error = getNextCode(value, offset, code2);
					if (!error)
					{
						if ((code1 & 0xFC) == 0xD8)
						{
							error = getNextCode(value, offset, code3);
							if (!error)
							{
								error = getNextCode(value, offset, code4);
								if (!error)
								{
									if ((code3 & 0xFC) == 0xDC)
									{
										return character.set(code1, code2, code3, code4);
									}
									else
									{
										error = Ash::Encoding::Error::invalidCodeUnit;
									}
								}
							}
						}
						else if ((code1 & 0xFC) != 0xDC)
						{
							return character.set(code1, code2);
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
				Code code3 = 0;
				Code code4 = 0;

				Ash::Error::Value error = getPreviousCode(value, offset, code1);
				if (!error)
				{
					error = getPreviousCode(value, offset, code2);
					if (!error)
					{
						if ((code2 & 0xFC) == 0xDC)
						{
							error = getPreviousCode(value, offset, code3);
							if (!error)
							{
								error = getPreviousCode(value, offset, code4);
								if (!error)
								{
									if ((code4 & 0xFC) == 0xD8)
									{
										return character.set(code4, code3, code2, code1);
									}
									else
									{
										error = Ash::Encoding::Error::invalidCodeUnit;
									}
								}
							}
						}
						else if ((code2 & 0xFC) != 0xD8)
						{
							return character.set(code2, code1);
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
			Utf16be();
		};
	}
}
