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
					typename PART,
					typename = Ash::Type::IsClass<PART, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion lookup()
				{
					using Part = PART;

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0xFF))
					{
						if (Part::Lookup::isCodeValid(code))
						{
							TEST_IS_TRUE(Part::Lookup::isCodeValid(code));
							Ash::Unicode::Character character = Part::Lookup::getCharacter(code);
							TEST_IS_NOT_EQ(character, Ash::Unicode::Character::replacement);
							TEST_IS_EQ(Part::Lookup::getCode(character), code);
						}
						else
						{
							TEST_IS_FALSE(Part::Lookup::isCodeValid(code));
							TEST_IS_EQ(Part::Lookup::getCharacter(code), Ash::Unicode::Character::replacement);
						}
					}

					return {};
				}

				template
				<
					typename PART,
					typename = Ash::Type::IsClass<PART, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion character()
				{
					using Part = PART;
					using Character = Part::Character;

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

						if (Part::Lookup::isCharacterValid(value))
						{
							TEST_IS_EQ(character.getLength(), 1);

							TEST_IS_EQ(Ash::Unicode::Character(*character.at(0)), Part::Lookup::getCode(character));

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
					typename PART,
					typename = Ash::Type::IsClass<PART, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion decodeNext()
				{
					using Part = PART;
					using Character = Part::Character;

					Ash::Memory::Array<Ash::Encoding::SingleByte::Code> content;
					Character character;
					Ash::Encoding::SingleByte::Code expectedCode = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Part::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0xFF))
					{
						if (Part::Lookup::isCodeValid(code))
						{
							TEST_IS_TRUE(content.append(Character(Part::Lookup::getCharacter(code))));
						}
					}

					expectedCode = 0;
					while (offset < content.getLength())
					{
						size_t length = Part::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), Part::Lookup::getCharacter(expectedCode));

						offset = offset + length;
						do
						{
							expectedCode++;
						}
						while (!Part::Lookup::isCodeValid(expectedCode));
					}

					TEST_IS_ZERO(Part::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0xA0, 0xFF))
					{
						if (!Part::Lookup::isCodeValid(code))
						{
							Ash::Memory::Sequence<Ash::Encoding::SingleByte::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, code));

							TEST_IS_ZERO(Part::decodeNext(invalidContent, 0, character));

							TEST_IS_ZERO(character.getLength());
						}
					}

					return {};
				}

				template
				<
					typename PART,
					typename = Ash::Type::IsClass<PART, Ash::Encoding::SingleByte::Generic::Table>
				>
				static Ash::Test::Assertion decodePrevious()
				{
					using Part = PART;
					using Character = Part::Character;

					Ash::Memory::Array<Ash::Encoding::SingleByte::Code> content;
					Character character;
					Ash::Encoding::SingleByte::Code expectedCode = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Part::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0x00, 0xFF))
					{
						if (Part::Lookup::isCodeValid(code))
						{
							TEST_IS_TRUE(content.append(Character(Part::Lookup::getCharacter(code))));
						}
					}

					expectedCode = 0xFF;
					offset = content.getLength();
					while (offset > 0)
					{
						while (!Part::Lookup::isCodeValid(expectedCode))
						{
							expectedCode--;
						}

						size_t length = Part::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), Part::Lookup::getCharacter(expectedCode));

						offset = offset - length;
						expectedCode--;
					}

					TEST_IS_ZERO(Part::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Encoding::SingleByte::Code code : Ash::Iterate<Ash::Encoding::SingleByte::Code>::between(0xA0, 0xFF))
					{
						if (!Part::Lookup::isCodeValid(code))
						{
							Ash::Memory::Sequence<Ash::Encoding::SingleByte::Code, 1> invalidContent;

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
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::lookup, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::character, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodeNext, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::Iso8859::decodePrevious, Ash::Encoding::Iso8859::Part16),
		);
	}
}
