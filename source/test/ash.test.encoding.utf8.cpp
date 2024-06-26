#include "ash.encoding.utf8.h"
#include "ash.test.encoding.utf8.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Utf8
			{
				static Ash::Test::Assertion character()
				{
					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
					{
						Ash::Encoding::Utf8::Character character(value);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(uint8_t(*character.at(0)), value);

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0x7FF))
					{
						Ash::Encoding::Utf8::Character character(value);

						TEST_IS_EQ(character.getLength(), 2);

						TEST_IS_EQ(uint8_t(*character.at(0)), 0xC0 + (value >> 6));
						TEST_IS_EQ(uint8_t(*character.at(1)), 0x80 + (value & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x800, 0xD7FF))
					{
						Ash::Encoding::Utf8::Character character(value);

						TEST_IS_EQ(character.getLength(), 3);

						TEST_IS_EQ(uint8_t(*character.at(0)), 0xE0 + (value >> 12));
						TEST_IS_EQ(uint8_t(*character.at(1)), 0x80 + ((value >> 6) & 0x3F));
						TEST_IS_EQ(uint8_t(*character.at(2)), 0x80 + (value & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xE000, 0xFFFF))
					{
						Ash::Encoding::Utf8::Character character(value);

						TEST_IS_EQ(character.getLength(), 3);

						TEST_IS_EQ(uint8_t(*character.at(0)), 0xE0 + (value >> 12));
						TEST_IS_EQ(uint8_t(*character.at(1)), 0x80 + ((value >> 6) & 0x3F));
						TEST_IS_EQ(uint8_t(*character.at(2)), 0x80 + (value & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x10000, Ash::Unicode::Character::maximum))
					{
						Ash::Encoding::Utf8::Character character(value);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(uint8_t(*character.at(0)), 0xF0 + (value >> 18));
						TEST_IS_EQ(uint8_t(*character.at(1)), 0x80 + ((value >> 12) & 0x3F));
						TEST_IS_EQ(uint8_t(*character.at(2)), 0x80 + ((value >> 6) & 0x3F));
						TEST_IS_EQ(uint8_t(*character.at(3)), 0x80 + (value & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateStart, Ash::Unicode::Character::surrogateEnd))
					{
						Ash::Encoding::Utf8::Character character(value);

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					return {};
				}

				static Ash::Test::Assertion decodeNext()
				{
					Ash::Memory::Array<Ash::Encoding::Utf8::Code> content;
					Ash::Encoding::Utf8::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value unicodeCharacter : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf8::Character(unicodeCharacter)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Utf8::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + length;
						if (offset < content.getLength())
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateStart - 1) ? Ash::Unicode::Character::surrogateEnd + 1 : expectedCharacter + 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, Ash::Unicode::Character::maximum);

					TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x8000, 0xFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						if ((value & 0xE0C0) == 0xC080)
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 8));
						TEST_IS_TRUE(invalidContent.set(1, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x800000, 0xFFFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						if (((value & 0xE0C000) == 0xC08000) || ((value & 0xF0C0C0) == 0xE08080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 16));
						TEST_IS_TRUE(invalidContent.set(1, value >> 8));
						TEST_IS_TRUE(invalidContent.set(2, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80000000, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						if (((value & 0xE0C00000) == 0xC0800000) || ((value & 0xF0C0C000) == 0xE0808000) || ((value & 0xF8C0C0C0) == 0xF0808080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 24));
						TEST_IS_TRUE(invalidContent.set(1, value >> 16));
						TEST_IS_TRUE(invalidContent.set(2, value >> 8));
						TEST_IS_TRUE(invalidContent.set(3, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xC0 + (value >> 6)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7FF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (value >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (value >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xD800, 0xDFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (value >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x110000, 0x1FFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (value >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious()
				{
					Ash::Memory::Array<Ash::Encoding::Utf8::Code> content;
					Ash::Encoding::Utf8::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value unicodeCharacter : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf8::Character(unicodeCharacter)));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Utf8::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - length;
						if (offset > 0)
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateEnd + 1) ? Ash::Unicode::Character::surrogateStart - 1 : expectedCharacter - 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, 0);

					TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x80, 0xFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x100, 0xFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						if (((value & 0x80) == 0) || ((value & 0xE0C0) == 0xC080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 8));
						TEST_IS_TRUE(invalidContent.set(1, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x10000, 0xFFFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						if (((value & 0x80) == 0) || ((value & 0xE0C0) == 0xC080) || ((value & 0xF0C0C0) == 0xE08080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 16));
						TEST_IS_TRUE(invalidContent.set(1, value >> 8));
						TEST_IS_TRUE(invalidContent.set(2, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x1000000, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						if (((value & 0x80) == 0) || ((value & 0xE0C0) == 0xC080) || ((value & 0xF0C0C0) == 0xE08080) || ((value & 0xF8C0C0C0) == 0xF0808080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, value >> 24));
						TEST_IS_TRUE(invalidContent.set(1, value >> 16));
						TEST_IS_TRUE(invalidContent.set(2, value >> 8));
						TEST_IS_TRUE(invalidContent.set(3, value));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7F))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xC0 + (value >> 6)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0x7FF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (value >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xD800, 0xDFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (value >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (value >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x110000, 0x1FFFFF))
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (value >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((value >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((value >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (value & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testUtf8,
			TEST_CASE(Ash::Test::Encoding::Utf8::character),
			TEST_CASE(Ash::Test::Encoding::Utf8::decodeNext),
			TEST_CASE(Ash::Test::Encoding::Utf8::decodePrevious)
		);
	}
}
