#include "ash.encoding.iso8859.h"
#include "ash.test.encoding.iso8859.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Iso8859
			{
				template
				<
					typename TABLE,
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion lookup()
				{
					using Table = TABLE;

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0xFF))
					{
						if (Table::Lookup::isCodeValid(code))
						{
							TEST_IS_TRUE(Table::Lookup::isCodeValid(code));
							Ash::Unicode::Character character = Table::Lookup::getCharacter(code);
							TEST_IS_NOT_EQ(character, Ash::Unicode::Character::replacement);
							TEST_IS_EQ(Table::Lookup::getCode(character), code);
						}
						else
						{
							TEST_IS_FALSE(Table::Lookup::isCodeValid(code));
							TEST_IS_EQ(Table::Lookup::getCharacter(code), Ash::Unicode::Character::replacement);
						}
					}

					return {};
				}

				template
				<
					typename TABLE,
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion character()
				{
					using Table = TABLE;
					using Character = Table::Character;

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x9F))
					{
						Character character(value);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(Ash::Unicode::Character(*character.at(0)), value);

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xA0, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
					{
						Character character(value);

						if (Table::Lookup::isCharacterValid(value))
						{
							TEST_IS_EQ(character.getLength(), 1);

							TEST_IS_EQ(Ash::Unicode::Character(*character.at(0)), Table::Lookup::getCode(character));

							TEST_IS_EQ(Ash::Unicode::Character(character), value);
						}
						else
						{
							TEST_IS_EQ(character.getLength(), 0);
						}
					}

					return {};
				}

				template
				<
					typename TABLE,
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion decodeNext()
				{
					using Table = TABLE;
					using Character = Table::Character;

					Ash::Memory::Array<Ash::Encoding::SingleByte::Code> content;
					Character character;
					Ash::Encoding::SingleByte::Code expectedCode = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Table::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0xFF))
					{
						if (Table::Lookup::isCodeValid(code))
						{
							TEST_IS_TRUE(content.append(Character(Table::Lookup::getCharacter(code))));
						}
					}

					expectedCode = 0;
					while (offset < content.getLength())
					{
						size_t length = Table::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), Table::Lookup::getCharacter(expectedCode));

						offset = offset + length;
						do
						{
							expectedCode++;
						}
						while (!Table::Lookup::isCodeValid(expectedCode));
					}

					TEST_IS_ZERO(Table::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0xA0, 0xFF))
					{
						if (!Table::Lookup::isCodeValid(code))
						{
							Ash::Memory::Sequence<Ash::Encoding::SingleByte::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, code));

							TEST_IS_ZERO(Table::decodeNext(invalidContent, 0, character));

							TEST_IS_ZERO(character.getLength());
						}
					}

					return {};
				}

				template
				<
					typename TABLE,
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion decodePrevious()
				{
					using Table = TABLE;
					using Character = Table::Character;

					Ash::Memory::Array<Ash::Encoding::SingleByte::Code> content;
					Character character;
					Ash::Encoding::SingleByte::Code expectedCode = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Table::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0xFF))
					{
						if (Table::Lookup::isCodeValid(code))
						{
							TEST_IS_TRUE(content.append(Character(Table::Lookup::getCharacter(code))));
						}
					}

					expectedCode = 0xFF;
					offset = content.getLength();
					while (offset > 0)
					{
						while (!Table::Lookup::isCodeValid(expectedCode))
						{
							expectedCode--;
						}

						size_t length = Table::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), Table::Lookup::getCharacter(expectedCode));

						offset = offset - length;
						expectedCode--;
					}

					TEST_IS_ZERO(Table::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0xA0, 0xFF))
					{
						if (!Table::Lookup::isCodeValid(code))
						{
							Ash::Memory::Sequence<Ash::Encoding::SingleByte::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, code));

							TEST_IS_ZERO(Table::decodePrevious(invalidContent, invalidContent.getLength(), character));

							TEST_IS_ZERO(character.getLength());
						}
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testIso8859,
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part1::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part2::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part3::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part4::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part5::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part6::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part7::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part8::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part9::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part10::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part11::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part13::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part14::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part15::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part16::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part1::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part2::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part3::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part4::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part5::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part6::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part7::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part8::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part9::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part10::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part11::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part13::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part14::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part15::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part16::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part1::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part2::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part3::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part4::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part5::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part6::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part7::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part8::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part9::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part10::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part11::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part13::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part14::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part15::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part16::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part1::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part2::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part3::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part4::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part5::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part6::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part7::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part8::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part9::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part10::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part11::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part13::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part14::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part15::Table),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part16::Table),
		);
	}
}
