#include "ash.memory.h"
#include "ash.encoding.utf16le.h"
#include "ash.test.encoding.utf16le.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Utf16le
			{
				static Ash::Test::Assertion character()
				{
					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xD7FF))
					{
						Ash::Encoding::Utf16le::Character character(value);

						TEST_IS_EQ(character.getLength(), 2);

						TEST_IS_EQ(uint8_t(*character.at(0)), value & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(1)), value >> 8);

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xE000, 0xFFFF))
					{
						Ash::Encoding::Utf16le::Character character(value);

						TEST_IS_EQ(character.getLength(), 2);

						TEST_IS_EQ(uint8_t(*character.at(0)), value & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(1)), value >> 8);

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x10000, Ash::Unicode::Character::maximum))
					{
						Ash::Encoding::Utf16le::Character character(value);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(uint8_t(*character.at(0)), ((value - 0x10000) >> 10) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(1)), 0xD8 + (((value - 0x10000) >> 18) & 0x03));
						TEST_IS_EQ(uint8_t(*character.at(2)), (value - 0x10000) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(3)), 0xDC + (((value - 0x10000) >> 8) & 0x03));

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateStart, Ash::Unicode::Character::surrogateEnd))
					{
						Ash::Encoding::Utf16le::Character character(value);

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					return {};
				}

				static Ash::Test::Assertion decodeNext()
				{
					Ash::Memory::Array<Ash::Encoding::Utf16le::Code> content;
					Ash::Encoding::Utf16le::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf16le::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf16le::Character(value)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Utf16le::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + length;
						if (offset < content.getLength())
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateStart - 1) ? Ash::Unicode::Character::surrogateEnd + 1 : expectedCharacter + 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, Ash::Unicode::Character::maximum);

					TEST_IS_ZERO(Ash::Encoding::Utf16le::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16le::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(Ash::Encoding::Utf16le::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xD80000, 0xDBFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16le::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value >> 8));
						TEST_IS_TRUE(invalidContent.set(1, value >> 16));
						TEST_IS_TRUE(invalidContent.set(2, value));

						TEST_IS_ZERO(Ash::Encoding::Utf16le::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xD8000000, 0xDFFFFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16le::Code, 4> invalidContent;

						if ((value & 0xFC00FC00) == 0xD800DC00)
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 16));
						TEST_IS_TRUE(invalidContent.set(1, value >> 24));
						TEST_IS_TRUE(invalidContent.set(2, value));
						TEST_IS_TRUE(invalidContent.set(3, value >> 8));

						TEST_IS_ZERO(Ash::Encoding::Utf16le::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious()
				{
					Ash::Memory::Array<Ash::Encoding::Utf16le::Code> content;
					Ash::Encoding::Utf16le::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf16le::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf16le::Character(value)));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Utf16le::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - length;
						if (offset > 0)
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateEnd + 1) ? Ash::Unicode::Character::surrogateStart - 1 : expectedCharacter - 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, 0);

					TEST_IS_ZERO(Ash::Encoding::Utf16le::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16le::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(Ash::Encoding::Utf16le::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xDC0000, 0xDFFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16le::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));
						TEST_IS_TRUE(invalidContent.set(1, value >> 8));
						TEST_IS_TRUE(invalidContent.set(2, value >> 16));

						TEST_IS_ZERO(Ash::Encoding::Utf16le::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xD8000000, 0xDFFFFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf16le::Code, 4> invalidContent;

						if (((value & 0xFFFF) < 0xD800) || ((value & 0xFFFF) > 0xDFFF) || ((value & 0xFC00FC00) == 0xD800DC00))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 16));
						TEST_IS_TRUE(invalidContent.set(1, value >> 24));
						TEST_IS_TRUE(invalidContent.set(2, value));
						TEST_IS_TRUE(invalidContent.set(3, value >> 8));

						TEST_IS_ZERO(Ash::Encoding::Utf16le::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testUtf16le,
			TEST_CASE(Ash::Test::Encoding::Utf16le::character),
			TEST_CASE(Ash::Test::Encoding::Utf16le::decodeNext),
			TEST_CASE(Ash::Test::Encoding::Utf16le::decodePrevious)
		);
	}
}
