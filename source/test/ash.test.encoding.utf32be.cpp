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
					// valid code points
					for (Ash::Unicode::Character::Value code = 0; code < 0xD800; code++)
					{
						Ash::Encoding::Utf32be::Character character(code);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(*character.at(0), ((code >> 24) & 0xFF));
						TEST_IS_EQ(*character.at(1), ((code >> 16) & 0xFF));
						TEST_IS_EQ(*character.at(2), ((code >> 8) & 0xFF));
						TEST_IS_EQ(*character.at(3), (code & 0xFF));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0xE000; code < 0x110000; code++)
					{
						Ash::Encoding::Utf32be::Character character(code);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(*character.at(0), ((code >> 24) & 0xFF));
						TEST_IS_EQ(*character.at(1), ((code >> 16) & 0xFF));
						TEST_IS_EQ(*character.at(2), ((code >> 8) & 0xFF));
						TEST_IS_EQ(*character.at(3), (code & 0xFF));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					// invalid code points
					for (Ash::Unicode::Character::Value code = 0xD800; code < 0xE000; code++)
					{
						Ash::Encoding::Utf32be::Character character(code);

						TEST_IS_ZERO(character.getLength());

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

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf32be::Character(code)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Utf32be::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + length;
						if (expectedCharacter == 0xD7FF)
						{
							expectedCharacter = 0xE000;
						}
						else
						{
							expectedCharacter = expectedCharacter + 1;
						}
					}

					TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code < 0x100; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x100; code < 0x10000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code >> 8));
						TEST_IS_TRUE(invalidContent.set(1, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x1000000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code >> 16));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x110000; code != 0; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code >> 24));
						TEST_IS_TRUE(invalidContent.set(1, code >> 16));
						TEST_IS_TRUE(invalidContent.set(2, code >> 8));
						TEST_IS_TRUE(invalidContent.set(3, code));

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

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(character));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Utf32be::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - length;
						if (expectedCharacter == 0xE000)
						{
							expectedCharacter = 0xD7FF;
						}
						else
						{
							expectedCharacter = expectedCharacter - 1;
						}
					}

					TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code < 0x100; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x100; code < 0x10000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code >> 8));
						TEST_IS_TRUE(invalidContent.set(1, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x1000000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code >> 16));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32be::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x110000; code != 0; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf32be::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code >> 24));
						TEST_IS_TRUE(invalidContent.set(1, code >> 16));
						TEST_IS_TRUE(invalidContent.set(2, code >> 8));
						TEST_IS_TRUE(invalidContent.set(3, code));

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
