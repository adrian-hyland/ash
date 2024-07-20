#include "ash.encoding.h"
#include "ash.ascii.h"
#include "ash.iso8859.h"
#include "ash.utf8.h"
#include "ash.utf16le.h"
#include "ash.utf16be.h"
#include "ash.utf32le.h"
#include "ash.utf32be.h"
#include "ash.wide.h"
#include "ash.test.encoding.h"


namespace Ash
{
	namespace Test
	{
		namespace Encoding
		{
			static Ash::Test::Assertion findAscii()
			{
				Ash::Ascii::String<> asciiString;
				size_t offset;

				offset = Ash::Encoding::find<Ash::Encoding::Ascii>(asciiString, 0, '1');
				TEST_IS_EQ(offset, asciiString.getLength());

				for (Ash::Encoding::Ascii::Code code : Ash::Iterate<Ash::Encoding::Ascii::Code>::between(1, 127))
				{
					TEST_IS_TRUE(asciiString.append(code));
				}

				offset = Ash::Encoding::find<Ash::Encoding::Ascii>(asciiString, 0, '\x01');
				TEST_IS_EQ(offset, 0);

				offset = Ash::Encoding::find<Ash::Encoding::Ascii>(asciiString, 0, '\x7F');
				TEST_IS_EQ(offset, 0x7E);

				offset = Ash::Encoding::find<Ash::Encoding::Ascii>(asciiString, 0, '\x00');
				TEST_IS_EQ(offset, asciiString.getLength());

				offset = Ash::Encoding::find<Ash::Encoding::Ascii>(asciiString, 1, '\x01');
				TEST_IS_EQ(offset, asciiString.getLength());

				offset = Ash::Encoding::find<Ash::Encoding::Ascii>(asciiString, asciiString.getLength(), '\x01');
				TEST_IS_EQ(offset, asciiString.getLength());
				
				return {};
			}

			template
			<
				typename ENCODING,
				typename = Ash::Type::IsClass<ENCODING, Ash::Encoding::Iso8859::Generic::Part>
			>
			static Ash::Test::Assertion findIso8859()
			{
				using Table = typename ENCODING::Table;
				Ash::String::Array<ENCODING> string;
				size_t offset;

				offset = Ash::Encoding::find<ENCODING>(string, 0, '1');
				TEST_IS_EQ(offset, string.getLength());

				for (typename ENCODING::Code code : Ash::Iterate<typename ENCODING::Code>::between(1, 0xFF))
				{
					if (Table::isCodeValid(code))
					{
						TEST_IS_TRUE(string.append(code));
					}
				}

				offset = Ash::Encoding::find<ENCODING>(string, 0, '\x01');
				TEST_IS_EQ(offset, 0);

				offset = Ash::Encoding::find<ENCODING>(string, 0, Table::getCharacter(Table::startCode + Table::size - 1));
				TEST_IS_EQ(offset, string.getLength() - 1);

				offset = Ash::Encoding::find<ENCODING>(string, 0, '\x00');
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, 1, '\x01');
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, string.getLength(), '\x01');
				TEST_IS_EQ(offset, string.getLength());
				
				return {};
			}

			template
			<
				typename ENCODING,
				typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
			>
			static Ash::Test::Assertion find()
			{
				Ash::String::Array<ENCODING> string;
				size_t offset;

				offset = Ash::Encoding::find<ENCODING>(string, 0, '1');
				TEST_IS_EQ(offset, string.getLength());

				for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(1, 0xD7FF) + Ash::Iterate<Ash::Unicode::Character::Value>::between(0xE000, Ash::Unicode::Character::maximum))
				{
					TEST_IS_TRUE(string.append(typename ENCODING::Character(value)));
				}

				offset = Ash::Encoding::find<ENCODING>(string, 0, 0x00);
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, 0, 0x01);
				TEST_IS_EQ(offset, 0);
				offset = Ash::Encoding::find<ENCODING>(string, offset + ENCODING::minSize, 0x01);
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, 0, 0x80);
				TEST_IS_EQ(offset, 0x7F * ENCODING::minSize);
				offset = Ash::Encoding::find<ENCODING>(string, offset + ENCODING::minSize, 0x80);
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, 0, 0x102);
				if constexpr (Ash::Type::isSame<ENCODING, Ash::Encoding::Utf8>)
				{
					TEST_IS_EQ(offset, 0x7F + 2 * (0x102 - 0x80));
				}
				else
				{
					TEST_IS_EQ(offset, 0x101 * ENCODING::minSize);
				}
				offset = Ash::Encoding::find<ENCODING>(string, offset + ENCODING::minSize, 0x01);
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, 0, 0x800);
				if constexpr (Ash::Type::isSame<ENCODING, Ash::Encoding::Utf8>)
				{
					TEST_IS_EQ(offset, 0x7F + 2 * (0x800 - 0x80));
				}
				else
				{
					TEST_IS_EQ(offset, 0x7FF * ENCODING::minSize);
				}
				offset = Ash::Encoding::find<ENCODING>(string, offset + ENCODING::minSize, 0x800);
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, 0, 0x10000);
				if constexpr (Ash::Type::isSame<ENCODING, Ash::Encoding::Utf8>)
				{
					TEST_IS_EQ(offset, 0x7F + 2 * (0x800 - 0x80) + 3 * (0x10000 - 0x800 - (0xE000 - 0xD800)));
				}
				else
				{
					TEST_IS_EQ(offset, (0xFFFF - (0xE000 - 0xD800)) * ENCODING::minSize);
				}
				offset = Ash::Encoding::find<ENCODING>(string, offset + ENCODING::minSize, 0x800);
				TEST_IS_EQ(offset, string.getLength());

				offset = Ash::Encoding::find<ENCODING>(string, 0, Ash::Unicode::Character::maximum);
				if constexpr (Ash::Type::isSame<ENCODING, Ash::Encoding::Utf8>)
				{
					TEST_IS_EQ(offset, 0x7F + 2 * (0x800 - 0x80) + 3 * (0x10000 - 0x800 - (0xE000 - 0xD800)) + 4 * (Ash::Unicode::Character::maximum - 0x10000));
				}
				else
				{
					TEST_IS_EQ(offset, (0xFFFF - (0xE000 - 0xD800)) * ENCODING::minSize + (Ash::Unicode::Character::maximum - 0x10000) * ENCODING::maxSize);
				}
				offset = Ash::Encoding::find<ENCODING>(string, offset + ENCODING::minSize, Ash::Unicode::Character::maximum);
				TEST_IS_EQ(offset, string.getLength());
				
				return {};
			}

			template
			<
				typename ENCODING,
				typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
			>
			static Ash::Test::Assertion contains()
			{
				Ash::String::Array<ENCODING> string;

				TEST_IS_FALSE(string.contains(' '));

				TEST_IS_TRUE(string.append(typename ENCODING::Character(' ')));
				TEST_IS_TRUE(string.contains(' '));
				TEST_IS_FALSE(string.contains('!'));

				return {};
			}

			template
			<
				typename ENCODING,
				typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
			>
			static Ash::Test::Assertion skipAnyOf()
			{
				Ash::String::Array<ENCODING> string;

				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'a' }), 0);

				TEST_IS_TRUE(string.append(typename ENCODING::Character('a')));
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'a' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'c', 'a' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'b', 'c' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'b' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, ENCODING::minSize, { 'a' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, ENCODING::minSize, { 'c', 'a' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, ENCODING::minSize, { 'b', 'c' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, ENCODING::minSize, { 'b' }), ENCODING::minSize);

				TEST_IS_TRUE(string.append(typename ENCODING::Character('a')));
				TEST_IS_TRUE(string.append(typename ENCODING::Character('a')));
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'a' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'c', 'a' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'b', 'c' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'b' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 2 * ENCODING::minSize, { 'a' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 2 * ENCODING::minSize, { 'c', 'a' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 2 * ENCODING::minSize, { 'b', 'c' }), 2 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 2 * ENCODING::minSize, { 'b' }), 2 * ENCODING::minSize);

				TEST_IS_TRUE(string.append(typename ENCODING::Character('b')));
				TEST_IS_TRUE(string.append(typename ENCODING::Character('b')));
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'a', 'b' }), 5 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'c', 'a' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'b', 'c' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'a' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 0, { 'b' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 4 * ENCODING::minSize, { 'a', 'b' }), 5 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 4 * ENCODING::minSize, { 'c', 'a' }), 4 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 4 * ENCODING::minSize, { 'b', 'c' }), 5 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 4 * ENCODING::minSize, { 'a' }), 4 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipAnyOf<ENCODING>(string, 4 * ENCODING::minSize, { 'b' }), 5 * ENCODING::minSize);

				return {};
			}

			template
			<
				typename ENCODING,
				typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
			>
			static Ash::Test::Assertion skipNoneOf()
			{
				Ash::String::Array<ENCODING> string;

				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'a' }), 0);

				TEST_IS_TRUE(string.append(typename ENCODING::Character('a')));
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'a' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'c', 'a' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'b', 'c' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'b' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, ENCODING::minSize, { 'a' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, ENCODING::minSize, { 'c', 'a' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, ENCODING::minSize, { 'b', 'c' }), ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, ENCODING::minSize, { 'b' }), ENCODING::minSize);

				TEST_IS_TRUE(string.append(typename ENCODING::Character('a')));
				TEST_IS_TRUE(string.append(typename ENCODING::Character('a')));
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'a' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'c', 'a' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'b', 'c' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'b' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 2 * ENCODING::minSize, { 'a' }), 2 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 2 * ENCODING::minSize, { 'c', 'a' }), 2 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 2 * ENCODING::minSize, { 'b', 'c' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 2 * ENCODING::minSize, { 'b' }), 3 * ENCODING::minSize);

				TEST_IS_TRUE(string.append(typename ENCODING::Character('b')));
				TEST_IS_TRUE(string.append(typename ENCODING::Character('b')));
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'a', 'b' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'c', 'a' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'b', 'c' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'c', 'd' }), 5 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'a' }), 0);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'b' }), 3 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 0, { 'c' }), 5 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 4 * ENCODING::minSize, { 'a', 'b' }), 4 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 4 * ENCODING::minSize, { 'c', 'a' }), 5 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 4 * ENCODING::minSize, { 'b', 'c' }), 4 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 4 * ENCODING::minSize, { 'a' }), 5 * ENCODING::minSize);
				TEST_IS_EQ(Ash::Encoding::skipNoneOf<ENCODING>(string, 4 * ENCODING::minSize, { 'b' }), 4 * ENCODING::minSize);

				return {};
			}

			template
			<
				typename ENCODING,
				typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
			>
			static Ash::Test::Assertion token()
			{
				Ash::String::Array<ENCODING> string;
				Ash::String::View<ENCODING> token;
				Ash::String::Array<ENCODING> filename;
				Ash::String::Buffer<ENCODING, 8> extension;

				Ash::Ascii::View("/test/folder/source.c").convertTo(string);

				size_t offset = Ash::Encoding::token<ENCODING>(string, 0, { '/' }, token);
				TEST_IS_EQ(offset, 5 * ENCODING::minSize);
				TEST_IS_EQ(string.match(ENCODING::minSize, token), token.getLength());
				TEST_IS_EQ(token.getLength(), 4 * ENCODING::minSize);

				offset = Ash::Encoding::token<ENCODING>(string, offset, { '/' }, token);
				TEST_IS_EQ(offset, 12 * ENCODING::minSize);
				TEST_IS_EQ(string.match(6 * ENCODING::minSize, token), token.getLength());
				TEST_IS_EQ(token.getLength(), 6 * ENCODING::minSize);

				offset = Ash::Encoding::token<ENCODING>(string, offset, { '/' }, token);
				TEST_IS_EQ(offset, 21 * ENCODING::minSize);
				TEST_IS_EQ(string.match(13 * ENCODING::minSize, token), token.getLength());
				TEST_IS_EQ(token.getLength(), 8 * ENCODING::minSize);

				offset = Ash::Encoding::token<ENCODING>(token, 0, { '.' }, filename);
				TEST_IS_EQ(offset, 6 * ENCODING::minSize);
				TEST_IS_EQ(token.match(0, filename), filename.getLength());
				TEST_IS_EQ(filename.getLength(), 6 * ENCODING::minSize);

				offset = Ash::Encoding::token<ENCODING>(token, offset, { '.' }, extension);
				TEST_IS_EQ(offset, 8 * ENCODING::minSize);
				TEST_IS_EQ(token.match(7 * ENCODING::minSize, extension), extension.getLength());
				TEST_IS_EQ(extension.getLength(), 1 * ENCODING::minSize);

				return {};
			}

			template
			<
				typename ENCODING,
				typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
			>
			static Ash::Test::Assertion convert()
			{
				using Character = typename ENCODING::Character;
				Ash::String::Array<ENCODING> from;
				Ash::String::Array<ENCODING> to;
				Ash::Utf8::String<> utf8;
				Ash::Utf16be::String<> utf16be;
				Ash::Utf16le::String<> utf16le;
				Ash::Utf32be::String<> utf32be;
				Ash::Utf32le::String<> utf32le;
				Ash::Wide::String<> wide;
				size_t length;

				if constexpr (Ash::Type::isSame<ENCODING, Ash::Encoding::Ascii>)
				{
					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(1, 0x7F))
					{
						TEST_IS_TRUE(from.append(Character(value)));
					}
				}
				else if constexpr (Ash::Type::isClass<ENCODING, Ash::Encoding::Iso8859::Generic::Part>)
				{
					using Table = typename ENCODING::Table;

					for (typename ENCODING::Code value : Ash::Iterate<typename ENCODING::Code>::between(1, 0xFF))
					{
						if (Table::isCodeValid(value))
						{
							TEST_IS_TRUE(from.append(Character(Table::getCharacter(value))));
						}
					}
				}
				else
				{
					for (Ash::Unicode::Character::Value value : Ash::Iterate<Ash::Unicode::Character::Value>::between(1, 0xD7FF) + Ash::Iterate<Ash::Unicode::Character::Value>::between(0xE000, Ash::Unicode::Character::maximum))
					{
						TEST_IS_TRUE(from.append(Character(value)));
					}
				}

				length = Ash::Encoding::convert<ENCODING, Ash::Encoding::Utf8>(from, utf8);
				TEST_IS_EQ(length, from.getLength());
				length = Ash::Encoding::convert<Ash::Encoding::Utf8, ENCODING>(utf8, to);
				TEST_IS_EQ(length, utf8.getLength());
				TEST_IS_EQ(from.getLength(), to.getLength());
				TEST_IS_EQ(from.match(0, to), from.getLength());

				length = Ash::Encoding::convert<ENCODING, Ash::Encoding::Utf16be>(from, utf16be);
				TEST_IS_EQ(length, from.getLength());
				length = Ash::Encoding::convert<Ash::Encoding::Utf16be, ENCODING>(utf16be, to);
				TEST_IS_EQ(length, utf16be.getLength());
				TEST_IS_EQ(from.getLength(), to.getLength());
				TEST_IS_EQ(from.match(0, to), from.getLength());

				length = Ash::Encoding::convert<ENCODING, Ash::Encoding::Utf16le>(from, utf16le);
				TEST_IS_EQ(length, from.getLength());
				length = Ash::Encoding::convert<Ash::Encoding::Utf16le, ENCODING>(utf16le, to);
				TEST_IS_EQ(length, utf16le.getLength());
				TEST_IS_EQ(from.getLength(), to.getLength());
				TEST_IS_EQ(from.match(0, to), from.getLength());

				length = Ash::Encoding::convert<ENCODING, Ash::Encoding::Utf32be>(from, utf32be);
				TEST_IS_EQ(length, from.getLength());
				length = Ash::Encoding::convert<Ash::Encoding::Utf32be, ENCODING>(utf32be, to);
				TEST_IS_EQ(length, utf32be.getLength());
				TEST_IS_EQ(from.getLength(), to.getLength());
				TEST_IS_EQ(from.match(0, to), from.getLength());

				length = Ash::Encoding::convert<ENCODING, Ash::Encoding::Utf32le>(from, utf32le);
				TEST_IS_EQ(length, from.getLength());
				length = Ash::Encoding::convert<Ash::Encoding::Utf32le, ENCODING>(utf32le, to);
				TEST_IS_EQ(length, utf32le.getLength());
				TEST_IS_EQ(from.getLength(), to.getLength());
				TEST_IS_EQ(from.match(0, to), from.getLength());

				length = Ash::Encoding::convert<ENCODING, Ash::Encoding::Wide>(from, wide);
				TEST_IS_EQ(length, from.getLength());
				length = Ash::Encoding::convert<Ash::Encoding::Wide, ENCODING>(wide, to);
				TEST_IS_EQ(length, wide.getLength());
				TEST_IS_EQ(from.getLength(), to.getLength());
				TEST_IS_EQ(from.match(0, to), from.getLength());

				return {};
			}
		}

		TEST_UNIT
		(
			testEncodingCore,

			TEST_CASE(Ash::Test::Encoding::findAscii),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::findIso8859, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::find, Ash::Encoding::Utf8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::find, Ash::Encoding::Utf16le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::find, Ash::Encoding::Utf16be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::find, Ash::Encoding::Utf32le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::find, Ash::Encoding::Utf32be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::find, Ash::Encoding::Wide),

			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Ascii),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Utf8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Utf16le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Utf16be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Utf32le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Utf32be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::contains, Ash::Encoding::Wide),

			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Ascii),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Utf8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Utf16le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Utf16be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Utf32le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Utf32be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipAnyOf, Ash::Encoding::Wide),

			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Ascii),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Utf8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Utf16le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Utf16be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Utf32le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Utf32be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::skipNoneOf, Ash::Encoding::Wide),

			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Ascii),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Utf8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Utf16le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Utf16be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Utf32le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Utf32be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::token, Ash::Encoding::Wide),

			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Ascii),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part1),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part2),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part3),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part4),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part5),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part6),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part7),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part9),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part10),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part11),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part13),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part14),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part15),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Iso8859::Part16),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Utf8),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Utf16le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Utf16be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Utf32le),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Utf32be),
			TEST_CASE_GENERIC(Ash::Test::Encoding::convert, Ash::Encoding::Wide),
		);
	}
}
