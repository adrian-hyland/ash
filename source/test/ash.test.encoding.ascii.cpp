#include "ash.encoding.ascii.h"
#include "ash.test.encoding.ascii.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Ascii
			{
				static Ash::Test::Assertion isValid()
				{
					TEST_IS_TRUE(Ash::Encoding::Ascii::Character::isValid(0x00));
					TEST_IS_TRUE(Ash::Encoding::Ascii::Character::isValid(0x7F));

					TEST_IS_FALSE(Ash::Encoding::Ascii::Character::isValid(0x80));
					TEST_IS_FALSE(Ash::Encoding::Ascii::Character::isValid(Ash::Unicode::Character::maximum));

					return {};
				}

				static Ash::Test::Assertion character()
				{
					Ash::Encoding::Ascii::Character character;
					TEST_IS_ZERO(character.getLength());
					TEST_IS_EQ(Ash::Unicode::Character(character), '\0');

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
					{
						Ash::Encoding::Ascii::Character character(value);

						TEST_IS_EQ(character.getLength(), 1);
						TEST_IS_EQ(*character.at(0), Ash::Encoding::Ascii::Code(value));
						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, Ash::Unicode::Character::maximum))
					{
						Ash::Encoding::Ascii::Character character(value);

						TEST_IS_EQ(character.getLength(), 1);
						TEST_IS_EQ(*character.at(0), Ash::Encoding::Ascii::Character::replacementCode);
						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character(Ash::Encoding::Ascii::Character::replacementCode));
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, Ash::Unicode::Character::maximum))
					{
						try
						{
							Ash::Encoding::Ascii::Character character(value, false);

							TEST_FAIL("Ash::Encoding::Error::invalidCodePoint exception");
						}
						catch (const Ash::Error::Exception &exception)
						{
							TEST_IS_EQ(Ash::Error::Value(*exception.getCategory(), exception.getCode()), Ash::Encoding::Error::invalidCodePoint);
						}
					}

					return {};
				}

				static Ash::Test::Assertion set()
				{
					for (Ash::Unicode::Character value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
					{
						Ash::Encoding::Ascii::Character character;

						TEST_IS_EQ(character.set(value), Ash::Error::none);
						TEST_IS_EQ(Ash::Unicode::Character(character), value);

						TEST_IS_EQ(character.set(value, false), Ash::Error::none);
						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, Ash::Unicode::Character::maximum))
					{
						Ash::Encoding::Ascii::Character character;

						TEST_IS_EQ(character.set(value), Ash::Error::none);
						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character(Ash::Encoding::Ascii::Character::replacementCode));

						TEST_IS_EQ(character.set(value, false), Ash::Encoding::Error::invalidCodePoint);
					}

					return {};
				}

				static Ash::Test::Assertion decodeNext()
				{
					Ash::Memory::Array<Ash::Encoding::Ascii::Code> content;
					Ash::Encoding::Ascii::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_EQ(Ash::Encoding::Ascii::decodeNext(content, offset, character), Ash::Memory::Error::readAccessOutOfBound);
					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
					{
						TEST_IS_EQ(content.append(Ash::Encoding::Ascii::Character(value)), Ash::Error::none);
					}

					expectedCharacter = 0;
					while (offset < content.getLength())
					{
						TEST_IS_EQ(Ash::Encoding::Ascii::decodeNext(content, offset, character), Ash::Error::none);
						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + character.getLength();
						expectedCharacter = expectedCharacter + 1;
					}

					TEST_IS_EQ(Ash::Encoding::Ascii::decodeNext(content, offset, character), Ash::Memory::Error::readAccessOutOfBound);
					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Ascii::Code, 1> invalidContent;

						TEST_IS_EQ(invalidContent.set(0, Ash::Encoding::Ascii::Code(value)), Ash::Error::none);
						TEST_IS_EQ(Ash::Encoding::Ascii::decodeNext(invalidContent, 0, character), Ash::Encoding::Error::invalidCodeUnit);
						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious()
				{
					Ash::Memory::Array<Ash::Encoding::Ascii::Code> content;
					Ash::Encoding::Ascii::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_EQ(Ash::Encoding::Ascii::decodePrevious(content, offset, character), Ash::Memory::Error::readAccessOutOfBound);
					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
					{
						TEST_IS_EQ(content.append(Ash::Encoding::Ascii::Character(value)), Ash::Error::none);
					}

					expectedCharacter = 127;
					offset = content.getLength();
					while (offset > 0)
					{
						TEST_IS_EQ(Ash::Encoding::Ascii::decodePrevious(content, offset, character), Ash::Error::none);
						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - character.getLength();
						expectedCharacter = expectedCharacter - 1;
					}

					TEST_IS_EQ(Ash::Encoding::Ascii::decodePrevious(content, offset, character), Ash::Memory::Error::readAccessOutOfBound);
					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Ascii::Code, 1> invalidContent;

						TEST_IS_EQ(invalidContent.set(0, Ash::Encoding::Ascii::Code(value)), Ash::Error::none);
						TEST_IS_EQ(Ash::Encoding::Ascii::decodePrevious(invalidContent, invalidContent.getLength(), character), Ash::Encoding::Error::invalidCodeUnit);
						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testAscii,
			TEST_CASE(Ash::Test::Encoding::Ascii::isValid),
			TEST_CASE(Ash::Test::Encoding::Ascii::character),
			TEST_CASE(Ash::Test::Encoding::Ascii::set),
			TEST_CASE(Ash::Test::Encoding::Ascii::decodeNext),
			TEST_CASE(Ash::Test::Encoding::Ascii::decodePrevious)
		);
	}
}
