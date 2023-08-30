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
					// valid code points
					for (Ash::Unicode::Character::Value code = 0; code < 0x80; code++)
					{
						Ash::Encoding::Utf8::Character character(code);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(*character.at(0), code);

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0x80; code < 0x800; code++)
					{
						Ash::Encoding::Utf8::Character character(code);

						TEST_IS_EQ(character.getLength(), 2);

						TEST_IS_EQ(*character.at(0), 0xC0 + (code >> 6));
						TEST_IS_EQ(*character.at(1), 0x80 + (code & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0x800; code < 0xD800; code++)
					{
						Ash::Encoding::Utf8::Character character(code);

						TEST_IS_EQ(character.getLength(), 3);

						TEST_IS_EQ(*character.at(0), 0xE0 + (code >> 12));
						TEST_IS_EQ(*character.at(1), 0x80 + ((code >> 6) & 0x3F));
						TEST_IS_EQ(*character.at(2), 0x80 + (code & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0xE000; code < 0x10000; code++)
					{
						Ash::Encoding::Utf8::Character character(code);

						TEST_IS_EQ(character.getLength(), 3);

						TEST_IS_EQ(*character.at(0), 0xE0 + (code >> 12));
						TEST_IS_EQ(*character.at(1), 0x80 + ((code >> 6) & 0x3F));
						TEST_IS_EQ(*character.at(2), 0x80 + (code & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x110000; code++)
					{
						Ash::Encoding::Utf8::Character character(code);

						TEST_IS_EQ(character.getLength(), 4);

						TEST_IS_EQ(*character.at(0), 0xF0 + (code >> 18));
						TEST_IS_EQ(*character.at(1), 0x80 + ((code >> 12) & 0x3F));
						TEST_IS_EQ(*character.at(2), 0x80 + ((code >> 6) & 0x3F));
						TEST_IS_EQ(*character.at(3), 0x80 + (code & 0x3F));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					// invalid code points
					for (Ash::Unicode::Character::Value code = 0xD800; code < 0xE000; code++)
					{
						Ash::Encoding::Utf8::Character character(code);

						TEST_IS_ZERO(character.getLength());

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

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf8::Character(code)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Utf8::decodeNext(content, offset, character);

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

					TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0x80; code < 0x100; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x8000; code < 0x10000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						if ((code & 0xE0C0) == 0xC080)
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 8));
						TEST_IS_TRUE(invalidContent.set(1, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x800000; code < 0x1000000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						if (((code & 0xE0C000) == 0xC08000) || ((code & 0xF0C0C0) == 0xE08080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 16));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x80000000; code != 0; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						if (((code & 0xE0C00000) == 0xC0800000) || ((code & 0xF0C0C000) == 0xE0808000) || ((code & 0xF8C0C0C0) == 0xF0808080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 24));
						TEST_IS_TRUE(invalidContent.set(1, code >> 16));
						TEST_IS_TRUE(invalidContent.set(2, code >> 8));
						TEST_IS_TRUE(invalidContent.set(3, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0; code < 0x80; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xC0 + (code >> 6)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0; code < 0x800; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (code >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0; code < 0x10000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (code >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0xD800; code < 0xE000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (code >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodeNext(invalidContent, 0, character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x110000; code < 0x200000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (code >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (code & 0x3F)));

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

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Utf8::Character(code)));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Utf8::decodePrevious(content, offset, character);

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

					TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0x80; code < 0x100; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 1> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x100; code < 0x10000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						if (((code & 0x80) == 0) || ((code & 0xE0C0) == 0xC080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 8));
						TEST_IS_TRUE(invalidContent.set(1, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x1000000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						if (((code & 0x80) == 0) || ((code & 0xE0C0) == 0xC080) || ((code & 0xF0C0C0) == 0xE08080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 16));
						TEST_IS_TRUE(invalidContent.set(1, code >> 8));
						TEST_IS_TRUE(invalidContent.set(2, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x01000000; code != 0; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						if (((code & 0x80) == 0) || ((code & 0xE0C0) == 0xC080) || ((code & 0xF0C0C0) == 0xE08080) || ((code & 0xF8C0C0C0) == 0xF0808080))
						{
							continue;
						}

						TEST_IS_TRUE(invalidContent.set(0, code >> 24));
						TEST_IS_TRUE(invalidContent.set(1, code >> 16));
						TEST_IS_TRUE(invalidContent.set(2, code >> 8));
						TEST_IS_TRUE(invalidContent.set(3, code));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0; code < 0x80; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 2> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xC0 + (code >> 6)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0; code < 0x800; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (code >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0xD800; code < 0xE000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 3> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xE0 + (code >> 12)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0; code < 0x10000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (code >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (code & 0x3F)));

						TEST_IS_ZERO(Ash::Encoding::Utf8::decodePrevious(invalidContent, invalidContent.getLength(), character));

						TEST_IS_ZERO(character.getLength());
					}

					for (Ash::Unicode::Character::Value code = 0x110000; code < 0x200000; code++)
					{
						Ash::Memory::Sequence<Ash::Encoding::Utf8::Code, 4> invalidContent;

						TEST_IS_TRUE(invalidContent.set(0, 0xF0 + (code >> 18)));
						TEST_IS_TRUE(invalidContent.set(1, 0x80 + ((code >> 12) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(2, 0x80 + ((code >> 6) & 0x3F)));
						TEST_IS_TRUE(invalidContent.set(3, 0x80 + (code & 0x3F)));

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
