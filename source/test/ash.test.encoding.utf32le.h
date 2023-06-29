#include "ash.test.h"
#include "ash.memory.h"
#include "ash.encoding.utf32le.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Utf32le
			{
				constexpr Ash::Test::Assertion character()
				{
					// valid code points
					for (Ash::Unicode::Character::Value code = 0; code < 0xD800; code++)
					{
						Ash::Encoding::Utf32le::Character character(code);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(*character.at(0), (code & 0xFF));
						TEST_IS_EQ(*character.at(1), ((code >> 8) & 0xFF));
						TEST_IS_EQ(*character.at(2), ((code >> 16) & 0xFF));
						TEST_IS_EQ(*character.at(3), ((code >> 24) & 0xFF));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0xE000; code < 0x110000; code++)
					{
						Ash::Encoding::Utf32le::Character character(code);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(*character.at(0), (code & 0xFF));
						TEST_IS_EQ(*character.at(1), ((code >> 8) & 0xFF));
						TEST_IS_EQ(*character.at(2), ((code >> 16) & 0xFF));
						TEST_IS_EQ(*character.at(3), ((code >> 24) & 0xFF));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					// invalid code points
					for (Ash::Unicode::Character::Value code = 0xD800; code < 0xE000; code++)
					{
						Ash::Encoding::Utf32le::Character character(code);

						TEST_IS_ZERO(character.getLength());

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					return {};
				}


				static Ash::Test::Assertion decodeNext(void)
				{
					Ash::Memory::Array<Ash::Encoding::Utf32le::Code> content;
					Ash::Encoding::Utf32le::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf32le::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf32le::Character(code)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						Ash::Encoding::Utf32le::Character character;
						
						size_t length = Ash::Encoding::Utf32le::decodeNext(content, offset, character);

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

					TEST_IS_ZERO(Ash::Encoding::Utf32le::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code < 0x100; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x100; code < 0x10000; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x1000000; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code >> 16));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x110000; code != 0; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code >> 16));
						TEST_IS_TRUE(invalidContent.set(3, code >> 24));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious()
				{
					Ash::Memory::Array<Ash::Encoding::Utf32le::Code> content;
					Ash::Encoding::Utf32le::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Utf32le::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf32le::Character(code)));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Utf32le::decodePrevious(content, offset, character);

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

					TEST_IS_ZERO(Ash::Encoding::Utf32le::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code < 0x100; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x100; code < 0x10000; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x1000000; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code >> 16));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x110000; code != 0; code++)
					{
						Ash::Memory::Vector<Ash::Encoding::Utf32le::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code >> 16));
						TEST_IS_TRUE(invalidContent.set(3, code >> 24));

						TEST_IS_ZERO(Ash::Encoding::Utf32le::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testUtf32le,
			TEST_CASE(Ash::Test::Encoding::Utf32le::character),
			TEST_CASE(Ash::Test::Encoding::Utf32le::decodeNext),
			TEST_CASE(Ash::Test::Encoding::Utf32le::decodePrevious)
		);
	}
}
