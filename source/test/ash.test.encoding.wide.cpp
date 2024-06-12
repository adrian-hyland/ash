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
					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x00, 0xD7FF))
					{
						Ash::Encoding::Wide::Character character(value);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(value));

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xE000, 0xFFFF))
					{
						Ash::Encoding::Wide::Character character(value);

						TEST_IS_EQ(character.getLength(), 1);

						TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(value));

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x10000, Ash::Unicode::Character::maximum))
					{
						Ash::Encoding::Wide::Character character(value);

						if constexpr (Ash::Encoding::Wide::maxSize == 1)
						{
							TEST_IS_EQ(character.getLength(), 1);

							TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(value));
						}
						else
						{
							TEST_IS_EQ(character.getLength(), 2);

							TEST_IS_EQ(*character.at(0), Ash::Encoding::Wide::Code(0xD800 + ((value - 0x10000) >> 10)));
							TEST_IS_EQ(*character.at(1), Ash::Encoding::Wide::Code(0xDC00 + ((value - 0x10000) & 0x3FF)));
						}

						TEST_IS_EQ(Ash::Unicode::Character(character), value);
					}

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateStart, Ash::Unicode::Character::surrogateEnd))
					{
						Ash::Encoding::Wide::Character character(value);

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

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Wide::Character(value)));
					}

					expectedCharacter = 0;
					offset = 0;
					while (offset < content.getLength())
					{
						size_t length = Ash::Encoding::Wide::decodeNext(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset + length;
						if (offset < content.getLength())
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateStart - 1) ? Ash::Unicode::Character::surrogateEnd + 1 : expectedCharacter + 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, Ash::Unicode::Character::maximum);

					TEST_IS_ZERO(Ash::Encoding::Wide::decodeNext(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					if constexpr (Ash::Encoding::Wide::maxSize == 2)
					{
						for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xD8000000, 0xDFFFFFFF))
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 2> invalidContent;

							if ((value & 0xFC00FC00) == 0xD800DC00)
							{
								continue;
							}

							TEST_IS_TRUE(invalidContent.set(0, value >> 16));
							TEST_IS_TRUE(invalidContent.set(1, value));

							TEST_IS_ZERO(Ash::Encoding::Wide::decodeNext(invalidContent, 0, character));

							TEST_IS_ZERO(character.getLength());
						}
					}
					else
					{
						for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x110000, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, value));

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

					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0, Ash::Unicode::Character::surrogateStart - 1) + Ash::Iterate<Ash::Unicode::Character::Value>::between(Ash::Unicode::Character::surrogateEnd + 1, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(content.append(Ash::Encoding::Wide::Character(value)));
					}

					expectedCharacter = Ash::Unicode::Character::maximum;
					offset = content.getLength();
					while (offset > 0)
					{
						size_t length = Ash::Encoding::Wide::decodePrevious(content, offset, character);

						TEST_IS_NOT_ZERO(length);

						TEST_IS_EQ(Ash::Unicode::Character(character), expectedCharacter);

						offset = offset - length;
						if (offset > 0)
						{
							expectedCharacter = (expectedCharacter == Ash::Unicode::Character::surrogateEnd + 1) ? Ash::Unicode::Character::surrogateStart - 1 : expectedCharacter - 1;
						}
					}
					TEST_IS_EQ(expectedCharacter, 0);

					TEST_IS_ZERO(Ash::Encoding::Wide::decodePrevious(content, offset, character));

					TEST_IS_ZERO(character.getLength());

					if constexpr (Ash::Encoding::Wide::maxSize == 2)
					{
						for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0xD8000000, 0xDFFFFFFF))
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 2> invalidContent;

							if (((value & 0xFFFF) < 0xD800) || ((value & 0xFFFF) > 0xDFFF) || ((value & 0xFC00FC00) == 0xD800DC00))
							{
								continue;
							}

							TEST_IS_TRUE(invalidContent.set(0, value >> 16));
							TEST_IS_TRUE(invalidContent.set(1, value));

							TEST_IS_ZERO(Ash::Encoding::Wide::decodePrevious(invalidContent, invalidContent.getLength(), character));

							TEST_IS_ZERO(character.getLength());
						}
					}
					else
					{
						for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(0x110000, std::numeric_limits<Ash::Unicode::Character::Value>::max()))
						{
							Ash::Memory::Sequence<Ash::Encoding::Wide::Code, 1> invalidContent;

							TEST_IS_TRUE(invalidContent.set(0, value));

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
