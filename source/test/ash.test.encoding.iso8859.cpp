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
				static Ash::Test::Assertion tableCore()
				{
					{
						using TestTable = Ash::Encoding::Iso8859::Table<0x80, 0>;
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestTable::isCodeValid(code));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestTable::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCharacterValid(character));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), code);
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCode(character, ' '), ' ');
						}
					}

					{
						static constexpr Ash::Encoding::CodeUnit16 values[] = { 0x1000 };
						using TestTable = Ash::Encoding::Iso8859::Table<0x80, sizeof(values) / sizeof(values[0]), values>;
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x80))
						{
							TEST_IS_TRUE(TestTable::isCodeValid(code));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x81, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestTable::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCharacterValid(character));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), code);
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x81, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCode(character, ' '), ' ');
						}
						TEST_IS_FALSE(TestTable::isCharacterValid(0x0FFF));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1000));
						TEST_IS_FALSE(TestTable::isCharacterValid(0x1001));
						TEST_IS_EQ(TestTable::getCharacter(0x80), 0x1000);
						TEST_IS_EQ(TestTable::getCode(0x0FFF), '?');
						TEST_IS_EQ(TestTable::getCode(0x1000), 0x80);
						TEST_IS_EQ(TestTable::getCode(0x1001), '?');
					}

					{
						static constexpr Ash::Encoding::CodeUnit16 values[] = { 0x1001, 0x1000 };
						using TestTable = Ash::Encoding::Iso8859::Table<0x80, sizeof(values) / sizeof(values[0]), values>;
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x81))
						{
							TEST_IS_TRUE(TestTable::isCodeValid(code));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x82, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestTable::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCharacterValid(character));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), code);
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x82, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCode(character, ' '), ' ');
						}
						TEST_IS_FALSE(TestTable::isCharacterValid(0x0FFF));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1000));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1001));
						TEST_IS_FALSE(TestTable::isCharacterValid(0x1002));
						TEST_IS_EQ(TestTable::getCharacter(0x80), 0x1001);
						TEST_IS_EQ(TestTable::getCharacter(0x81), 0x1000);
						TEST_IS_EQ(TestTable::getCode(0x0FFF), '?');
						TEST_IS_EQ(TestTable::getCode(0x1000), 0x81);
						TEST_IS_EQ(TestTable::getCode(0x1001), 0x80);
						TEST_IS_EQ(TestTable::getCode(0x1002), '?');
					}

					{
						static constexpr Ash::Encoding::CodeUnit16 values[] = { 0x1008, 0x1001, 0x1009, 0x1000, 0x1004, 0x1002, 0x1006, 0x1003, 0x1007, 0x1005 };
						using TestTable = Ash::Encoding::Iso8859::Table<0x80, sizeof(values) / sizeof(values[0]), values>;
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x89))
						{
							TEST_IS_TRUE(TestTable::isCodeValid(code));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x8A, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestTable::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x8A, 0xFF))
						{
							TEST_IS_FALSE(TestTable::isCharacterValid(character));
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), code);
						}
						for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x8A, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestTable::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestTable::getCode(character, ' '), ' ');
						}
						TEST_IS_FALSE(TestTable::isCharacterValid(0x0FFF));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1000));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1001));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1002));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1003));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1004));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1005));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1006));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1007));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1008));
						TEST_IS_TRUE(TestTable::isCharacterValid(0x1009));
						TEST_IS_FALSE(TestTable::isCharacterValid(0x100A));
						TEST_IS_EQ(TestTable::getCharacter(0x80), 0x1008);
						TEST_IS_EQ(TestTable::getCharacter(0x81), 0x1001);
						TEST_IS_EQ(TestTable::getCharacter(0x82), 0x1009);
						TEST_IS_EQ(TestTable::getCharacter(0x83), 0x1000);
						TEST_IS_EQ(TestTable::getCharacter(0x84), 0x1004);
						TEST_IS_EQ(TestTable::getCharacter(0x85), 0x1002);
						TEST_IS_EQ(TestTable::getCharacter(0x86), 0x1006);
						TEST_IS_EQ(TestTable::getCharacter(0x87), 0x1003);
						TEST_IS_EQ(TestTable::getCharacter(0x88), 0x1007);
						TEST_IS_EQ(TestTable::getCharacter(0x89), 0x1005);
						TEST_IS_EQ(TestTable::getCode(0x0FFF), '?');
						TEST_IS_EQ(TestTable::getCode(0x1000), 0x83);
						TEST_IS_EQ(TestTable::getCode(0x1001), 0x81);
						TEST_IS_EQ(TestTable::getCode(0x1002), 0x85);
						TEST_IS_EQ(TestTable::getCode(0x1003), 0x87);
						TEST_IS_EQ(TestTable::getCode(0x1004), 0x84);
						TEST_IS_EQ(TestTable::getCode(0x1005), 0x89);
						TEST_IS_EQ(TestTable::getCode(0x1006), 0x86);
						TEST_IS_EQ(TestTable::getCode(0x1007), 0x88);
						TEST_IS_EQ(TestTable::getCode(0x1008), 0x80);
						TEST_IS_EQ(TestTable::getCode(0x1009), 0x82);
						TEST_IS_EQ(TestTable::getCode(0x100A), '?');
					}

					return {};
				}

				template
				<
					typename TABLE,
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
				>
				static Ash::Test::Assertion table()
				{
					using Table = TABLE;
					for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0xFF))
					{
						if (Table::isCodeValid(code))
						{
							TEST_IS_TRUE(Table::isCodeValid(code));
							Ash::Unicode::Character character = Table::getCharacter(code);
							TEST_IS_NOT_EQ(character, Ash::Unicode::Character::replacement);
							TEST_IS_EQ(Table::getCode(character), code);
						}
						else
						{
							TEST_IS_FALSE(Table::isCodeValid(code));
							TEST_IS_EQ(Table::getCharacter(code), Ash::Unicode::Character::replacement);
						}
					}

					return {};
				}

				template
				<
					typename TABLE,
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
				>
				static Ash::Test::Assertion character()
				{
					using Table = TABLE;
					using Character = typename Ash::Encoding::Iso8859::Part<Table>::Character;

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

						if (Table::isCharacterValid(value))
						{
							TEST_IS_EQ(character.getLength(), 1);

							TEST_IS_EQ(Ash::Unicode::Character(*character.at(0)), Table::getCode(character));

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
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
				>
				static Ash::Test::Assertion decodeNext()
				{
					using Table = TABLE;
					using Part = typename Ash::Encoding::Iso8859::Part<Table>;
					using Character = Part::Character;

					Ash::Memory::Array<Ash::Encoding::Iso8859::Code> content;
					Character character;
					Ash::Encoding::Iso8859::Code expectedCode = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Part::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0xFF))
					{
						if (Table::isCodeValid(code))
						{
							TEST_IS_TRUE(content.append(Character(Table::getCharacter(code))));
						}
					}

					expectedCode = 0;
					while (offset < content.getLength())
					{
						size_t length = Part::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), Table::getCharacter(expectedCode));

						offset = offset + length;
						do
						{
							expectedCode++;
						}
						while (!Table::isCodeValid(expectedCode));
					}

					TEST_IS_ZERO(Part::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xA0, 0xFF))
					{
						if (!Table::isCodeValid(code))
						{
							Ash::Memory::Sequence<Ash::Encoding::Iso8859::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, code));

							TEST_IS_ZERO(Part::decodeNext(invalidContent, 0, character));

							TEST_IS_ZERO(character.getLength());
						}
					}

					return {};
				}

				template
				<
					typename TABLE,
					typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
				>
				static Ash::Test::Assertion decodePrevious()
				{
					using Table = TABLE;
					using Part = typename Ash::Encoding::Iso8859::Part<Table>;
					using Character = Part::Character;

					Ash::Memory::Array<Ash::Encoding::Iso8859::Code> content;
					Character character;
					Ash::Encoding::Iso8859::Code expectedCode = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Part::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Encoding::Iso8859::Code>::between(0x00, 0xFF))
					{
						if (Table::isCodeValid(code))
						{
							TEST_IS_TRUE(content.append(Character(Table::getCharacter(code))));
						}
					}

					expectedCode = 0xFF;
					offset = content.getLength();
					while (offset > 0)
					{
						while (!Table::isCodeValid(expectedCode))
						{
							expectedCode--;
						}

						size_t length = Part::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), Table::getCharacter(expectedCode));

						offset = offset - length;
						expectedCode--;
					}

					TEST_IS_ZERO(Part::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::Iso8859::Code code : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xA0, 0xFF))
					{
						if (!Table::isCodeValid(code))
						{
							Ash::Memory::Sequence<Ash::Encoding::Iso8859::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, code));

							TEST_IS_ZERO(Part::decodePrevious(invalidContent, invalidContent.getLength(), character));

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
			TEST_CASE(Ash::Test::Encoding::Iso8859::tableCore),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::table, Ash::Encoding::Iso8859::Table16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Table16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Table16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Table16),
		);
	}
}
