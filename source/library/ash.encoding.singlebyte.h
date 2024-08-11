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

				static constexpr bool isCodeValid(Code code) { return getInstance().m_ToUnicode.isValid(code); }

				static constexpr bool isCharacterValid(Ash::Unicode::Character character) { return getInstance().m_FromUnicode.isValid(character); }

				static constexpr Ash::Unicode::Character getCharacter(Code code) { return getInstance().m_ToUnicode.convert(code); }

				static constexpr Code getCode(Ash::Unicode::Character character, Code replacement = '?') { return getInstance().m_FromUnicode.convert(character, replacement); }

			protected:
				constexpr Lookup() : m_ToUnicode(VALUES), m_FromUnicode(VALUES) {}

				static Lookup &getInstance()
				{
					static Lookup lookup;

					return lookup;
				}

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
						return convert(character, nullptr);
					}

					constexpr Code convert(Ash::Unicode::Character character, Code replacement) const
					{
						Code code;
						if (!convert(character, &code))
						{
							code = replacement;
						}
						return code;
					}

					constexpr bool convert(Ash::Unicode::Character character, Code *code) const
					{
						if (character < startCode)
						{
							if (code != nullptr)
							{
								*code = character;
							}
							return true;
						}

						size_t from = 0;
						size_t to = m_Mappings.getLength();
						
						if ((character < m_Mappings.at(from)->from) || (character > m_Mappings.at(to - 1)->from))
						{
							return false;
						}

						while (to > from)
						{
							size_t middle = from + (to - from) / 2;
							if (m_Mappings.at(middle)->from == character)
							{
								if (code != nullptr)
								{
									*code = m_Mappings.at(middle)->to;
								}
								return true;
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
						return false;
					}

				protected:
					struct Map
					{
						constexpr Map() : from(0), to(0) {}

						constexpr Map(Ash::Encoding::CodeUnit16 fromValue, Code toValue) : from(fromValue), to(toValue) {}

						Ash::Encoding::CodeUnit16 from;
						Code                      to;
					};

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
						m_Mappings.insert(offset, map);
					}

				private:
					Ash::Memory::Buffer<Map, size> m_Mappings;
				};

			private:
				ToUnicode   m_ToUnicode;
				FromUnicode m_FromUnicode;

				Lookup(const Lookup &lookup) = delete;
				Lookup &operator = (const Lookup &lookup) = delete;
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

				static constexpr Code getCode(Ash::Unicode::Character character, Code replacementValue = '?') { return (character < startCode) ? Code(character) : replacementValue; }
			};

			namespace Generic
			{
				class Table : Ash::Generic::Encoding {};
			}

			template
			<
				typename LOOKUP,
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
					static constexpr Ash::Unicode::Character replacement = '?';

					constexpr Character() : Ash::Memory::Buffer<Code, maxSize>() {}

					constexpr Character(Ash::Unicode::Character character) : Ash::Memory::Buffer<Code, maxSize>()
					{
						set(character);
					}

					constexpr Ash::Unicode::Character operator = (Ash::Unicode::Character character)
					{
						set(character);
						return character;
					}

					constexpr operator Ash::Unicode::Character () const { return (getLength() > 0) ? Lookup::getCharacter((*this)[0]) : Ash::Unicode::Character(Ash::Unicode::Character::replacement); }

				protected:
					constexpr void set(Ash::Unicode::Character character)
					{
						if (Lookup::isCharacterValid(character))
						{
							setLength(1);
							(*this)[0] = Lookup::getCode(character);
						}
						else
						{
							setLength(0);
						}
					}

					constexpr size_t set(Code code)
					{
						setLength(1);
						(*this)[0] = code;
						return 1;
					}

					friend Table;
				};

				template
				<
					typename ALLOCATION,
					typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
				>
				static constexpr size_t decodeNext(const Ash::Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
				{
					Code code = 0;

					if (value.get(offset, code) && Lookup::isCodeValid(code))
					{
						return character.set(code);
					}
					else
					{
						character.clear();
						return 0;
					}
				}

				template
				<
					typename ALLOCATION,
					typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
				>
				static constexpr size_t decodePrevious(const Ash::Memory::Value<ALLOCATION, Code> &value, size_t offset, Character &character)
				{
					Code code = 0;

					if ((offset > 0) && value.get(--offset, code) && Lookup::isCodeValid(code))
					{
						return character.set(code);
					}
					else
					{
						character.clear();
						return 0;
					}
				}

			private:
				Table() = delete;
			};
		}
	}
}
