#include "ash.memory.h"
#include "ash.encoding.utf32be.h"
#include "ash.test.encoding.utf32be.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Utf32be
			{
				static Ash::Test::Assertion character(void)
				{
					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xD7FF))
					{
						Ash::Encoding::Utf32be::Character character(value);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(uint8_t(*character.at(0)), (value >> 24) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(1)), (value >> 16) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(2)), (value >> 8) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(3)), value & 0xFF);

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xE000, Ash::Unicode::Character::maximum))
					{
						Ash::Encoding::Utf32be::Character character(value);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(uint8_t(*character.at(0)), (value >> 24) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(1)), (value >> 16) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(2)), (value >> 8) & 0xFF);
						TEST_IS_EQ(uint8_t(*character.at(3)), value & 0xFF);

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateStart, Ash::Unicode::Character::surrogateEnd))
					{
						Ash::Encoding::Utf32be::Character character(value);

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					return {};
				}

				static Ash::Test::Assertion decodeNext()
				{
					Ash::Memory::Array<Ash::Encoding::Utf32be::Code> content;
					Ash::Encoding::Utf32be::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf32be::Character(value)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Utf32be::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + length;
						if (offset < content.getLength())
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateStart - 1) ? Ash::Unicode::Character::surrogateEnd + 1 : expectedCharacter + 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, Ash::Unicode::Character::maximum);

					TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x100, 0xFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value >> 8));
						TEST_IS_TRUE(invalidContent.set(1, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x10000, 0xFFFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value >> 16));
						TEST_IS_TRUE(invalidContent.set(1, value >> 8));
						TEST_IS_TRUE(invalidContent.set(2, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateStart, Ash::Unicode::Character::surrogateEnd))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0x00));
						TEST_IS_TRUE(invalidContent.set(1, 0x00));
						TEST_IS_TRUE(invalidContent.set(2, value >> 8));
						TEST_IS_TRUE(invalidContent.set(3, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x110000, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value >> 24));
						TEST_IS_TRUE(invalidContent.set(1, value >> 16));
						TEST_IS_TRUE(invalidContent.set(2, value >> 8));
						TEST_IS_TRUE(invalidContent.set(3, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious()
				{
					Ash::Memory::Array<Ash::Encoding::Utf32be::Code> content;
					Ash::Encoding::Utf32be::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf32be::Character(value)));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Utf32be::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - length;
						if (offset > 0)
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateEnd + 1) ? Ash::Unicode::Character::surrogateStart - 1 : expectedCharacter - 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, 0);

					TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x100, 0xFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value >> 8));
						TEST_IS_TRUE(invalidContent.set(1, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x10000, 0xFFFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value >> 16));
						TEST_IS_TRUE(invalidContent.set(1, value >> 8));
						TEST_IS_TRUE(invalidContent.set(2, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateStart, Ash::Unicode::Character::surrogateEnd))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0x00));
						TEST_IS_TRUE(invalidContent.set(1, 0x00));
						TEST_IS_TRUE(invalidContent.set(2, value >> 8));
						TEST_IS_TRUE(invalidContent.set(3, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x110000, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value >> 24));
						TEST_IS_TRUE(invalidContent.set(1, value >> 16));
						TEST_IS_TRUE(invalidContent.set(2, value >> 8));
						TEST_IS_TRUE(invalidContent.set(3, value));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testUtf32be,
			TEST_CASE(Ash::Test::Encoding::Utf32be::character),
			TEST_CASE(Ash::Test::Encoding::Utf32be::decodeNext),
			TEST_CASE(Ash::Test::Encoding::Utf32be::decodePrevious)
		);
	}
}
