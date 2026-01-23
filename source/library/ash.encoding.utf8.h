#pragma once

#include "ash.type.h"
#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		class Utf8 : Ash::Generic::Encoding
		{
		public:
			using Code = Ash::Encoding::CodeUnit8;

			static constexpr size_t minSize = 1;

			static constexpr size_t maxSize = 4;

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

					if (value < 0x80)
					{
						setLength(1).assertErrorNotSet();
						(*this)[0] = value;
					}
					else if (value < 0x0800)
					{
						setLength(2).assertErrorNotSet();
						(*this)[0] = (value >> 6) | 0xC0;
						(*this)[1] = (value & 0x3F) | 0x80;
					}
					else if (value < 0x10000)
					{
						setLength(3).assertErrorNotSet();
						(*this)[0] = (value >> 12) | 0xE0;
						(*this)[1] = ((value >> 6) & 0x3F) | 0x80;
						(*this)[2] = (value & 0x3F) | 0x80;
					}
					else
					{
						setLength(4).assertErrorNotSet();
						(*this)[0] = (value >> 18) | 0xF0;
						(*this)[1] = ((value >> 12) & 0x3F) | 0x80;
						(*this)[2] = ((value >> 6) & 0x3F) | 0x80;
						(*this)[3] = (value & 0x3F) | 0x80;
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

						case 2:
							return ((getCode(0) & 0x1F) << 6) | (getCode(1) & 0x3F);
						break;

						case 3:
							return ((getCode(0) & 0x0F) << 12) | ((getCode(1) & 0x3F) << 6) | (getCode(2) & 0x3F);
						break;

						case 4:
							return ((getCode(0) & 0x0F) << 18) | ((getCode(1) & 0x3F) << 12) | ((getCode(2) & 0x3F) << 6) | (getCode(3) & 0x3F);
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
				constexpr Ash::Error::Value set(Code code1)
				{
					setLength(1).assertErrorNotSet();
					(*this)[0] = code1;
					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1, Code code2)
				{
					if (uint8_t(code1) <= 0xC1)
					{
						clear();
						return Ash::Encoding::Error::overlongEncoding;
					}

					setLength(2).assertErrorNotSet();
					(*this)[0] = code1;
					(*this)[1] = code2;
					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1, Code code2, Code code3)
				{
					if ((uint8_t(code1) == 0xE0) && (uint8_t(code2) < 0xA0))
					{
						clear();
						return Ash::Encoding::Error::overlongEncoding;
					}

					if ((uint8_t(code1) == 0xED) && (uint8_t(code2) > 0x9F))
					{
						clear();
						return Ash::Encoding::Error::invalidCodePoint;
					}

					setLength(3).assertErrorNotSet();
					(*this)[0] = code1;
					(*this)[1] = code2;
					(*this)[2] = code3;
					return Ash::Error::none;
				}

				[[nodiscard]]
				constexpr Ash::Error::Value set(Code code1, Code code2, Code code3, Code code4)
				{
					if ((uint8_t(code1) == 0xF0) && (uint8_t(code2) < 0x90))
					{
						clear();
						return Ash::Encoding::Error::overlongEncoding;
					}

					if (((uint8_t(code1) == 0xF4) && (uint8_t(code2) > 0x8F)) || (uint8_t(code1) > 0xF4))
					{
						clear();
						return Ash::Encoding::Error::invalidCodePoint;
					}

					setLength(4).assertErrorNotSet();
					(*this)[0] = code1;
					(*this)[1] = code2;
					(*this)[2] = code3;
					(*this)[3] = code4;
					return Ash::Error::none;
				}

				friend Utf8;
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
					if ((code1 & 0x80) == 0x00)
					{
						return character.set(code1);
					}
					else if ((code1 < 0xC0) || (code1 > 0xF7))
					{
						error = Ash::Encoding::Error::invalidCodeUnit;
					}
					else
					{
						error = getNextCode(value, offset, code2);
						if (!error)
						{
							if ((code2 & 0xC0) != 0x80)
							{
								error = Ash::Encoding::Error::invalidCodeUnit;
							}
							else if ((code1 & 0xE0) == 0xC0)
							{
								return character.set(code1, code2);
							}
							else
							{
								error = getNextCode(value, offset, code3);
								if (!error)
								{
									if ((code3 & 0xC0) != 0x80)
									{
										error = Ash::Encoding::Error::invalidCodeUnit;
									}
									else if ((code1 & 0xF0) == 0xE0)
									{
										return character.set(code1, code2, code3);
									}
									else
									{
										error = getNextCode(value, offset, code4);
										if (!error)
										{
											if (((code1 & 0xF8) == 0xF0) && ((code4 & 0xC0) == 0x80))
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
							}
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
					if ((code1 & 0x80) == 0x00)
					{
						return character.set(code1);
					}
					else if ((code1 & 0xC0) != 0x80)
					{
						error = Ash::Encoding::Error::invalidCodeUnit;
					}
					else
					{
						error = getPreviousCode(value, offset, code2);
						if (!error)
						{
							if ((code2 & 0xE0) == 0xC0)
							{
								return character.set(code2, code1);
							}
							else if ((code2 & 0xC0) != 0x80)
							{
								error = Ash::Encoding::Error::invalidCodeUnit;
							}
							else
							{
								error = getPreviousCode(value, offset, code3);
								if (!error)
								{
									if ((code3 & 0xF0) == 0xE0)
									{
										return character.set(code3, code2, code1);
									}
									else if ((code3 & 0xC0) != 0x80)
									{
										error = Ash::Encoding::Error::invalidCodeUnit;
									}
									else
									{
										error = getPreviousCode(value, offset, code4);
										if (!error)
										{
											if ((code4 & 0xF8) == 0xF0)
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
							}
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
			Utf8();
		};
	}
}
