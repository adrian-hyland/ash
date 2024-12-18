#include "ash.encoding.singlebyte.h"
#include "ash.test.encoding.singlebyte.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace SingleByte
			{
				static Ash::Test::Assertion lookup()
				{
					{
						using TestLookup = Ash::Encoding::SingleByte::Lookup<0x80, 0>;
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestLookup::isCodeValid(code));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), code);
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCode(character, ' '), ' ');
						}
					}

					{
						static constexpr Ash::Encoding::CodeUnit16 values[] =
						{
							'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
						};
						using TestLookup = Ash::Encoding::SingleByte::Lookup<0, sizeof(values) / sizeof(values[0]), values>;
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x09))
						{
							TEST_IS_TRUE(TestLookup::isCodeValid(code));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x0A, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between('0', '9'))
						{
							TEST_IS_TRUE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x09))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), Ash::Unicode::Character('0' + code));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x0A, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between('0', '9'))
						{
							TEST_IS_EQ(TestLookup::getCode(character), character - '0');
						}
						TEST_IS_FALSE(TestLookup::isCharacterValid('0' - 1));
						TEST_IS_FALSE(TestLookup::isCharacterValid('9' + 1));
						TEST_IS_EQ(TestLookup::getCode('0' - 1), '?');
						TEST_IS_EQ(TestLookup::getCode('9' + 1), '?');
					}

					{
						static constexpr Ash::Encoding::CodeUnit16 values[] = { 0x1000 };
						using TestLookup = Ash::Encoding::SingleByte::Lookup<0x80, sizeof(values) / sizeof(values[0]), values>;
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x80))
						{
							TEST_IS_TRUE(TestLookup::isCodeValid(code));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x81, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), code);
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x81, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCode(character, ' '), ' ');
						}
						TEST_IS_FALSE(TestLookup::isCharacterValid(0x0FFF));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1000));
						TEST_IS_FALSE(TestLookup::isCharacterValid(0x1001));
						TEST_IS_EQ(TestLookup::getCharacter(0x80), 0x1000);
						TEST_IS_EQ(TestLookup::getCode(0x0FFF), '?');
						TEST_IS_EQ(TestLookup::getCode(0x1000), 0x80);
						TEST_IS_EQ(TestLookup::getCode(0x1001), '?');
					}

					{
						static constexpr Ash::Encoding::CodeUnit16 values[] = { 0x1001, 0x1000 };
						using TestLookup = Ash::Encoding::SingleByte::Lookup<0x80, sizeof(values) / sizeof(values[0]), values>;
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x81))
						{
							TEST_IS_TRUE(TestLookup::isCodeValid(code));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x82, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), code);
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x82, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCode(character, ' '), ' ');
						}
						TEST_IS_FALSE(TestLookup::isCharacterValid(0x0FFF));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1000));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1001));
						TEST_IS_FALSE(TestLookup::isCharacterValid(0x1002));
						TEST_IS_EQ(TestLookup::getCharacter(0x80), 0x1001);
						TEST_IS_EQ(TestLookup::getCharacter(0x81), 0x1000);
						TEST_IS_EQ(TestLookup::getCode(0x0FFF), '?');
						TEST_IS_EQ(TestLookup::getCode(0x1000), 0x81);
						TEST_IS_EQ(TestLookup::getCode(0x1001), 0x80);
						TEST_IS_EQ(TestLookup::getCode(0x1002), '?');
					}

					{
						static constexpr Ash::Encoding::CodeUnit16 values[] = { 0x1008, 0x1001, 0x1009, 0x1000, 0x1004, 0x1002, 0x1006, 0x1003, 0x1007, 0x1005 };
						using TestLookup = Ash::Encoding::SingleByte::Lookup<0x80, sizeof(values) / sizeof(values[0]), values>;
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x89))
						{
							TEST_IS_TRUE(TestLookup::isCodeValid(code));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x8A, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCodeValid(code));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_TRUE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x8A, 0xFF))
						{
							TEST_IS_FALSE(TestLookup::isCharacterValid(character));
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), code);
						}
						for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x8A, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCharacter(code), Ash::Unicode::Character::replacement);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
						{
							TEST_IS_EQ(TestLookup::getCode(character), character);
						}
						for (Ash::Unicode::Character character : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
						{
							TEST_IS_EQ(TestLookup::getCode(character, ' '), ' ');
						}
						TEST_IS_FALSE(TestLookup::isCharacterValid(0x0FFF));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1000));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1001));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1002));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1003));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1004));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1005));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1006));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1007));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1008));
						TEST_IS_TRUE(TestLookup::isCharacterValid(0x1009));
						TEST_IS_FALSE(TestLookup::isCharacterValid(0x100A));
						TEST_IS_EQ(TestLookup::getCharacter(0x80), 0x1008);
						TEST_IS_EQ(TestLookup::getCharacter(0x81), 0x1001);
						TEST_IS_EQ(TestLookup::getCharacter(0x82), 0x1009);
						TEST_IS_EQ(TestLookup::getCharacter(0x83), 0x1000);
						TEST_IS_EQ(TestLookup::getCharacter(0x84), 0x1004);
						TEST_IS_EQ(TestLookup::getCharacter(0x85), 0x1002);
						TEST_IS_EQ(TestLookup::getCharacter(0x86), 0x1006);
						TEST_IS_EQ(TestLookup::getCharacter(0x87), 0x1003);
						TEST_IS_EQ(TestLookup::getCharacter(0x88), 0x1007);
						TEST_IS_EQ(TestLookup::getCharacter(0x89), 0x1005);
						TEST_IS_EQ(TestLookup::getCode(0x0FFF), '?');
						TEST_IS_EQ(TestLookup::getCode(0x1000), 0x83);
						TEST_IS_EQ(TestLookup::getCode(0x1001), 0x81);
						TEST_IS_EQ(TestLookup::getCode(0x1002), 0x85);
						TEST_IS_EQ(TestLookup::getCode(0x1003), 0x87);
						TEST_IS_EQ(TestLookup::getCode(0x1004), 0x84);
						TEST_IS_EQ(TestLookup::getCode(0x1005), 0x89);
						TEST_IS_EQ(TestLookup::getCode(0x1006), 0x86);
						TEST_IS_EQ(TestLookup::getCode(0x1007), 0x88);
						TEST_IS_EQ(TestLookup::getCode(0x1008), 0x80);
						TEST_IS_EQ(TestLookup::getCode(0x1009), 0x82);
						TEST_IS_EQ(TestLookup::getCode(0x100A), '?');
					}

					return {};
				}

				static Ash::Test::Assertion character()
				{
					static constexpr Ash::Encoding::CodeUnit16 decimalValues[] =
					{
						'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
					};
					using DecimalLookup = Ash::Encoding::SingleByte::Lookup<0, sizeof(decimalValues) / sizeof(decimalValues[0]), decimalValues>;
					using DecimalTable = Ash::Encoding::SingleByte::Table<DecimalLookup>;

					for (Ash::Unicode::Character::Value value : decimalValues)
					{
						DecimalTable::Character character(value);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(Ash::Unicode::Character(*character.at(0)), value - '0');

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, '0' - 1))
					{
						DecimalTable::Character character(value);

						TEST_IS_ZERO(character.getLength());

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between('9' + 1, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
					{
						DecimalTable::Character character(value);

						TEST_IS_ZERO(character.getLength());

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					return {};
				}

				static Ash::Test::Assertion decodeNext()
				{
					static constexpr Ash::Encoding::CodeUnit16 decimalValues[] =
					{
						'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
					};
					using DecimalLookup = Ash::Encoding::SingleByte::Lookup<0, sizeof(decimalValues) / sizeof(decimalValues[0]), decimalValues>;
					using DecimalTable = Ash::Encoding::SingleByte::Table<DecimalLookup>;
					Ash::Memory::Array<DecimalTable::Code> content;
					DecimalTable::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(DecimalTable::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : decimalValues)
					{
						TEST_IS_TRUE(content.append(DecimalTable::Character(value)));
					}

					expectedCharacter = decimalValues[0];
					while (offset < content.getLength())
					{
						size_t length = DecimalTable::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + length;
						expectedCharacter = expectedCharacter + 1;
					}

					TEST_IS_ZERO(DecimalTable::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x0A, 0xFF))
					{
						Ash::Memory::Sequence<DecimalTable::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(DecimalTable::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious()
				{
					static constexpr Ash::Encoding::CodeUnit16 decimalValues[] =
					{
						'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
					};
					using DecimalLookup = Ash::Encoding::SingleByte::Lookup<0, sizeof(decimalValues) / sizeof(decimalValues[0]), decimalValues>;
					using DecimalTable = Ash::Encoding::SingleByte::Table<DecimalLookup>;
					Ash::Memory::Array<DecimalTable::Code> content;
					DecimalTable::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(DecimalTable::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : decimalValues)
					{
						TEST_IS_TRUE(content.append(DecimalTable::Character(value)));
					}

					expectedCharacter = decimalValues[sizeof(decimalValues) / sizeof(decimalValues[0]) - 1];
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = DecimalTable::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - length;
						expectedCharacter = expectedCharacter - 1;
					}

					TEST_IS_ZERO(DecimalTable::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x0A, 0xFF))
					{
						Ash::Memory::Sequence<DecimalTable::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(DecimalTable::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testSingleByte,
			TEST_CASE(Ash::Test::Encoding::SingleByte::lookup),
			TEST_CASE(Ash::Test::Encoding::SingleByte::character),
			TEST_CASE(Ash::Test::Encoding::SingleByte::decodeNext),
			TEST_CASE(Ash::Test::Encoding::SingleByte::decodePrevious),
		);
	}
}
