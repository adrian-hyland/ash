#pragma once

#include "ash.encoding.h"


namespace Ash
{
	namespace Encoding
	{
		namespace SingleByte
		{
			using Code = uint8_t;

			namespace Generic
			{
				class Lookup {};
			}

			template
			<
				Ash::Encoding::CodeUnit16        START_CODE,
				size_t                           SIZE = 0x100 - START_CODE,
				const Ash::Encoding::CodeUnit16 *VALUES = nullptr
			>
			class Lookup : Ash::Encoding::SingleByte::Generic::Lookup
			{
			public:
				static constexpr Ash::Encoding::CodeUnit16 startCode = START_CODE;

				static constexpr size_t size = SIZE;

				static constexpr bool isCodeValid(Code code) { return toUnicode.isValid(code); }

				static constexpr bool isCharacterValid(Ash::Unicode::Character character) { return fromUnicode.isValid(character); }

				static constexpr Ash::Unicode::Character getCharacter(Code code) { return toUnicode.convert(code); }

				static constexpr Code getCode(Ash::Unicode::Character character, Code replacement) { return fromUnicode.convert(character, replacement); }

			protected:
				class ToUnicode : public Ash::Memory::Sequence<Ash::Encoding::CodeUnit16, size>
				{
				public:
					using Content = Ash::Memory::Sequence<Ash::Encoding::CodeUnit16, size>;

					constexpr ToUnicode(const Ash::Encoding::CodeUnit16 values[size]) : Content(values, size) {}

					constexpr bool isValid(Code code) const
					{
						return (code < startCode) || (Content::getOr(code - startCode, Ash::Unicode::Character::replacement) != Ash::Unicode::Character::replacement);
					}

					constexpr Ash::Unicode::Character convert(Code code) const
					{
						return (code < startCode) ? code : Content::getOr(code - startCode, Ash::Unicode::Character::replacement);
					}
				};

				class FromUnicode
				{
				public:
					constexpr FromUnicode(const Ash::Encoding::CodeUnit16 values[size]) : m_Mappings()
					{
						Code code = startCode;
						for (size_t n = 0; n < size; n++)
						{
							if (values[n] != Ash::Unicode::Character::replacement)
							{
								insert(Map(values[n], code));
							}

							code++;
						}
					}

					constexpr bool isValid(Ash::Unicode::Character character) const
					{
						return (character < startCode) || (find(character) != nullptr);
					}

					constexpr Code convert(Ash::Unicode::Character character, Code replacementCode) const
					{
						if (character < startCode)
						{
							return character;
						}

						const Code *code = find(character);
						return (code != nullptr) ? *code : replacementCode;
					}

				protected:
					struct Map
					{
						constexpr Map() : from(0), to(0) {}

						constexpr Map(Ash::Encoding::CodeUnit16 fromValue, Code toValue) : from(fromValue), to(toValue) {}

						Ash::Encoding::CodeUnit16 from;
						Code                      to;
					};

					constexpr const Code *find(Ash::Unicode::Character character) const
					{
						size_t from = 0;
						size_t to = m_Mappings.getLength();

						if ((character < m_Mappings.at(from)->from) || (character > m_Mappings.at(to - 1)->from))
						{
							return nullptr;
						}

						while (to > from)
						{
							size_t middle = from + (to - from) / 2;
							if (m_Mappings.at(middle)->from == character)
							{
								return &m_Mappings.at(middle)->to;
							}
							else if (m_Mappings.at(middle)->from > character)
							{
								to = middle;
							}
							else
							{
								from = middle + 1;
							}
						}

						return nullptr;
					}

					constexpr void insert(Map map)
					{
						size_t offset = m_Mappings.getLength();
						if (offset > 0)
						{
							if (map.from < m_Mappings.at(0)->from)
							{
								offset = 0;
							}
							else if (map.from < m_Mappings.at(offset - 1)->from)
							{
								size_t from = 0;
								size_t to = offset;
								for (;;)
								{
									offset = from + (to - from) / 2;
									if (map.from < m_Mappings.at(offset)->from)
									{
										to = offset;
									}
									else
									{
										offset++;
										if (map.from > m_Mappings.at(offset)->from)
										{
											from = offset;
										}
										else
										{
											break;
										}
									}
								}
							}
						}

						m_Mappings.insert(offset, map).assertErrorNotSet();
					}

				private:
					Ash::Memory::Buffer<Map, size> m_Mappings;
				};

				static constexpr ToUnicode toUnicode = ToUnicode(VALUES);

				static constexpr FromUnicode fromUnicode = FromUnicode(VALUES);

			private:
				Lookup() = delete;
			};

			template
			<
				Ash::Encoding::CodeUnit16 START_CODE
			>
			class Lookup<START_CODE, 0, nullptr> : Ash::Encoding::SingleByte::Generic::Lookup
			{
			public:
				static constexpr Ash::Encoding::CodeUnit16 startCode = START_CODE;

				static constexpr size_t size = 0;

				static constexpr bool isCodeValid(Code code) { return code < startCode; }

				static constexpr bool isCharacterValid(Ash::Unicode::Character character) { return character < startCode; }

				static constexpr Ash::Unicode::Character getCharacter(Code code) { return (code < startCode) ? code : Ash::Unicode::Character::replacement; }

				static constexpr Code getCode(Ash::Unicode::Character character, Code replacement) { return (character < startCode) ? Code(character) : replacement; }
			};

			namespace Generic
			{
				class Table : Ash::Generic::Encoding {};
			}

			template
			<
				typename LOOKUP,
				Code     REPLACEMENT_CODE = '?',
				typename = Ash::Type::IsClass<LOOKUP, Ash::Encoding::SingleByte::Generic::Lookup>
			>
			class Table : Ash::Encoding::SingleByte::Generic::Table
			{
			public:
				using Lookup = LOOKUP;

				using Code = Ash::Encoding::SingleByte::Code;

				static constexpr size_t minSize = 1;

				static constexpr size_t maxSize = 1;

				static constexpr bool isLittleEndian = false;

				static constexpr bool isBigEndian = false;

				class Character : public Ash::Memory::Buffer<Code, maxSize>
				{
				public:
					static constexpr Code replacementCode = REPLACEMENT_CODE;

					constexpr Character() : Ash::Memory::Buffer<Code, maxSize>() {}

					constexpr Character(Ash::Unicode::Character character, bool replaceInvalidCharacter = true) : Ash::Memory::Buffer<Code, maxSize>()
					{
						set(character, replaceInvalidCharacter).throwOnError();
					}

					constexpr Ash::Unicode::Character operator = (Ash::Unicode::Character character)
					{
						set(character).assertErrorNotSet();
						return character;
					}

					[[nodiscard]]
					constexpr Ash::Error::Value set(Ash::Unicode::Character character, bool replaceInvalidCharacter = true)
					{
						setLength(1).assertErrorNotSet();
						if (!isValid(character) && !replaceInvalidCharacter)
						{
							return Ash::Encoding::Error::invalidCodePoint;
						}

						(*this)[0] = Lookup::getCode(character, replacementCode);
						return Ash::Error::none;
					}

					constexpr operator Ash::Unicode::Character () const
					{
						return (getLength() > 0) ? Lookup::getCharacter((*this)[0]) : Ash::Unicode::Character('\0');
					}

					static constexpr bool isValid(Ash::Unicode::Character character)
					{
						return Lookup::isCharacterValid(character);
					}

				protected:
					[[nodiscard]]
					constexpr Ash::Error::Value set(Code code)
					{
						setLength(1).assertErrorNotSet();
						(*this)[0] = code;
						return Ash::Error::none;
					}

					friend Table;
				};

				[[nodiscard]]
				static constexpr Ash::Error::Value decodeNext(Ash::Memory::View<Code> value, size_t offset, Character &character)
				{
					Code code = 0;
					Ash::Error::Value error = getNextCode(value, offset, code);
					if (!error)
					{
						if (Lookup::isCodeValid(code))
						{
							return character.set(code);
						}
						else
						{
							error = Ash::Encoding::Error::invalidCodeUnit;
						}
					}

					character.clear();
					return error;
				}

				[[nodiscard]]
				static constexpr Ash::Error::Value decodePrevious(Ash::Memory::View<Code> value, size_t offset, Character &character)
				{
					Code code = 0;
					Ash::Error::Value error = getPreviousCode(value, offset, code);
					if (!error)
					{
						if (Lookup::isCodeValid(code))
						{
							return character.set(code);
						}
						else
						{
							error = Ash::Encoding::Error::invalidCodeUnit;
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
				Table() = delete;
			};
		}
	}
}
