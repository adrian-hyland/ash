#include "ash.memory.h"
#include "ash.encoding.wide.h"
#include "ash.test.encoding.wide.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			namespace Wide
			{
				static Ash::Test::Assertion character()
				{
					// valid code points
					for (Ash::Unicode::Character::Value code = 0; code < 0xD800; code++)
					{
						Ash::Encoding::Wide::Character character(code);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(code));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0xE000; code < 0x10000; code++)
					{
						Ash::Encoding::Wide::Character character(code);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(code));

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					for (Ash::Unicode::Character::Value code = 0x10000; code < 0x110000; code++)
					{
						Ash::Encoding::Wide::Character character(code);

						if (Ash::Encoding::Wide::maxSize == 1)
						{
							TEST_IS_EQ(character.getLength(), 1);

							TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(code));
						}
						else
						{
							TEST_IS_EQ(character.getLength(), 2);

							TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(0xD800 + ((code - 0x10000) >> 10)));
							TEST_IS_EQ(*character.at(1), Ash::Encoding::Wide::Code(0xDC00 + ((code - 0x10000) & 0x3FF)));
						}

						TEST_IS_EQ(Ash::Unicode::Character(character), code);
					}

					// invalid code points
					for (Ash::Unicode::Character::Value code = 0xD800; code < 0xE000; code++)
					{
						Ash::Encoding::Wide::Character character(code);

						TEST_IS_ZERO(character.getLength());

						TEST_IS_EQ(Ash::Unicode::Character(character), Ash::Unicode::Character::replacement);
					}

					return {};
				}

				static Ash::Test::Assertion decodeNext()
				{
					Ash::Memory::Array<Ash::Encoding::Wide::Code> content;
					Ash::Encoding::Wide::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Wide::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(character));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Wide::decodeNext(content, offset, character);

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

					TEST_IS_ZERO(Ash::Encoding::Wide::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					if (Ash::Encoding::Wide::maxSize == 2)
					{
						for (Ash::Unicode::Character::Value code = 0xD8000000; code < 0xE0000000; code++)
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 2> invalidContent;

							if ((code & 0xFC00FC00) == 0xD800DC00)
							{
								continue;
							}

							TEST_IS_TRUE(invalidContent.set(0, code >> 16));
							TEST_IS_TRUE(invalidContent.set(1, code));

							TEST_IS_ZERO(Ash::Encoding::Wide::decodeNext(invalidContent, 0, character));

							TEST_IS_ZERO(character.getLength());
						}
					}
					else
					{
						for (Ash::Unicode::Character::Value code = 0x110000; code != 0; code++)
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, code));

							TEST_IS_ZERO(Ash::Encoding::Wide::decodeNext(invalidContent, 0, character));

							TEST_IS_ZERO(character.getLength());
						}
					}

					return {};
				}

				static Ash::Test::Assertion decodePrevious()
				{
					Ash::Memory::Array<Ash::Encoding::Wide::Code> content;
					Ash::Encoding::Wide::Character character;
					Ash::Unicode::Character expectedCharacter = 0;
					size_t offset = 0;

					TEST_IS_ZERO(Ash::Encoding::Wide::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					for (Ash::Unicode::Character::Value code = 0; code <= Ash::Unicode::Character::maximum; code++)
					{
						TEST_IS_TRUE(content.append(character));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Wide::decodePrevious(content, offset, character);

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

					TEST_IS_ZERO(Ash::Encoding::Wide::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					if (Ash::Encoding::Wide::maxSize == 2)
					{
						for (Ash::Unicode::Character::Value code = 0xD8000000; code < 0xE0000000; code++)
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 2> invalidContent;

							if (((code & 0xFFFF) < 0xD800) || ((code & 0xFFFF) > 0xDFFF) || ((code & 0xFC00FC00) == 0xD800DC00))
							{
								continue;
							}

							TEST_IS_TRUE(invalidContent.set(0, code >> 16));
							TEST_IS_TRUE(invalidContent.set(1, code));

							TEST_IS_ZERO(Ash::Encoding::Wide::decodePrevious(invalidContent, invalidContent.getLength(), character));

							TEST_IS_ZERO(character.getLength());
						}
					}
					else
					{
						for (Ash::Unicode::Character::Value code = 0x110000; code != 0; code++)
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, code));

							TEST_IS_ZERO(Ash::Encoding::Wide::decodePrevious(invalidContent, invalidContent.getLength(), character));

							TEST_IS_ZERO(character.getLength());
						}
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testWide,
			TEST_CASE(Ash::Test::Encoding::Wide::character),
			TEST_CASE(Ash::Test::Encoding::Wide::decodeNext),
			TEST_CASE(Ash::Test::Encoding::Wide::decodePrevious)
		);
	}
};
