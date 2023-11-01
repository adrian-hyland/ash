#include "ash.memory.h"
#include "ash.encoding.utf16be.h"
#include "ash.test.encoding.utf32be.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Utf16be
			{
				static Ash::Test::Assertion character()
				{
					// valid code points
					for (Ash::Unicode::Character::Value code = 0; code < 0xD800; code++)
					{
						Ash::Encoding::Utf16be::Character character(code);

						TEST_IS_EQ(character.getLength(), 2);

						TEST_IS_EQ(uint8_t(*character.at(0)), (code >> 8));
						TEST_IS_EQ(uint8_t(*character.at(1)), (code & 0xFF));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0xE000; code < 0x10000; code++)
					{
						Ash::Encoding::Utf16be::Character character(code);

						TEST_IS_EQ(character.getLength(), 2);

						TEST_IS_EQ(uint8_t(*character.at(0)), (code >> 8));
						TEST_IS_EQ(uint8_t(*character.at(1)), (code & 0xFF));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x110000; code++)
					{
						Ash::Encoding::Utf16be::Character character(code);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(uint8_t(*character.at(0)), 0xD8 + (((code - 0x10000) >> 18) & 0x03));
						TEST_IS_EQ(uint8_t(*character.at(1)), (((code - 0x10000) >> 10) & 0xFF));
						TEST_IS_EQ(uint8_t(*character.at(2)), 0xDC + (((code - 0x10000) >> 8) & 0x03));
						TEST_IS_EQ(uint8_t(*character.at(3)), ((code - 0x10000) & 0xFF));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					// invalid code points
					for (Ash::Unicode::Character::Value code = 0xD800; code < 0xE000; code++)
					{
						Ash::Encoding::Utf16be::Character character(code);

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					return {};
				}

				static Ash::Test::Assertion decodeNext()
				{
					Ash::Memory::Array<Ash::Encoding::Utf16be::Code> content;
					Ash::Encoding::Utf16be::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf16be::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character unicodeCharacter : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf16be::Character(unicodeCharacter)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Utf16be::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + length;
						if (offset < content.getLength())
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateStart - 1) ? Ash::Unicode::Character::surrogateEnd + 1 : expectedCharacter + 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, Ash::Unicode::Character::maximum);

					TEST_IS_ZERO(Ash::Encoding::Utf16be::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code < 0x100; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16be::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf16be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0xD80000; code < 0xDC0000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16be::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code >> 16));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code));

						TEST_IS_ZERO(Ash::Encoding::Utf16be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0xD8000000; code < 0xE0000000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16be::Code, 4> invalidContent;

						if ((code & 0xFC00FC00) == 0xD800DC00)
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 24));
						TEST_IS_TRUE(invalidContent.set(1, code >> 16));
						TEST_IS_TRUE(invalidContent.set(2, code >> 8));
						TEST_IS_TRUE(invalidContent.set(3, code));

						TEST_IS_ZERO(Ash::Encoding::Utf16be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious(void)
				{
					Ash::Memory::Array<Ash::Encoding::Utf16be::Code> content;
					Ash::Encoding::Utf16be::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf16be::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character unicodeCharacter : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf16be::Character(unicodeCharacter)));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Utf16be::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - length;
						if (offset > 0)
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateEnd + 1) ? Ash::Unicode::Character::surrogateStart - 1 : expectedCharacter - 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, 0);

					TEST_IS_ZERO(Ash::Encoding::Utf16be::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code < 0x100; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16be::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf16be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0xDC0000; code < 0xE00000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16be::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));
						TEST_IS_TRUE(invalidContent.set(1, code >> 16));
						TEST_IS_TRUE(invalidContent.set(2, code >> 8));

						TEST_IS_ZERO(Ash::Encoding::Utf16be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0xD8000000; code < 0xE0000000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16be::Code, 4> invalidContent;

						if (((code & 0xFFFF) < 0xD800) || ((code & 0xFFFF) > 0xDFFF) || ((code & 0xFC00FC00) == 0xD800DC00))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 24));
						TEST_IS_TRUE(invalidContent.set(1, code >> 16));
						TEST_IS_TRUE(invalidContent.set(2, code >> 8));
						TEST_IS_TRUE(invalidContent.set(3, code));

						TEST_IS_ZERO(Ash::Encoding::Utf16be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testUtf16be,
			TEST_CASE(Ash::Test::Encoding::Utf16be::character),
			TEST_CASE(Ash::Test::Encoding::Utf16be::decodeNext),
			TEST_CASE(Ash::Test::Encoding::Utf16be::decodePrevious)
		);
	}
};
