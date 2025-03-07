#include "ash.string.h"
#include "ash.ascii.h"
#include "ash.utf8.h"
#include "ash.wide.h"
#include "ash.test.string.h"


namespace Ash
{
	namespace Test
	{
		namespace String
		{
			namespace Literal
			{
				static Ash::Test::Assertion ascii()
				{
					TEST_IS_ZERO(Ash::Ascii::Literal::getLength(nullptr));
					TEST_IS_ZERO(Ash::Ascii::Literal::getLength(""));
					TEST_IS_EQ(Ash::Ascii::Literal::getLength("abc"), 3);

					TEST_IS_ZERO(Ash::Ascii::Literal::getLength(nullptr, 0, 4));
					TEST_IS_ZERO(Ash::Ascii::Literal::getLength("", 0, 4));
					TEST_IS_EQ(Ash::Ascii::Literal::getLength("abc", 3, 4), 3);
					TEST_IS_EQ(Ash::Ascii::Literal::getLength("abcde", 5, 4), 4);

					TEST_IS_ZERO(Ash::Ascii::Literal::getLength(nullptr, 4));
					TEST_IS_ZERO(Ash::Ascii::Literal::getLength("", 4));
					TEST_IS_EQ(Ash::Ascii::Literal::getLength("abc", 4), 3);
					TEST_IS_EQ(Ash::Ascii::Literal::getLength("abcde", 4), 4);

					Ash::Ascii::View view;

					view = Ash::Ascii::Literal::getView(nullptr, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Ascii::Literal::getView("", 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Ascii::Literal::getView("abc", 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), "abc", view.getLength()));

					view = Ash::Ascii::Literal::getView("abcde", 4);
					TEST_IS_EQ(view.getLength(), 4);
					TEST_IS_ZERO(::memcmp(view.at(0), "abcde", view.getLength()));

					view = Ash::Ascii::Literal::getView(nullptr, 0, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Ascii::Literal::getView("", 0, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Ascii::Literal::getView("abc", 3, 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), "abc", view.getLength()));

					view = Ash::Ascii::Literal::getView("abcde", 5, 4);
					TEST_IS_EQ(view.getLength(), 4);
					TEST_IS_ZERO(::memcmp(view.at(0), "abcde", view.getLength()));

					return {};
				}

				static Ash::Test::Assertion utf8()
				{
					TEST_IS_ZERO(Ash::Utf8::Literal::getLength(nullptr));
					TEST_IS_ZERO(Ash::Utf8::Literal::getLength(u8""));
					TEST_IS_EQ(Ash::Utf8::Literal::getLength(u8"abc"), 3);

					TEST_IS_ZERO(Ash::Utf8::Literal::getLength(nullptr, 0, 4));
					TEST_IS_ZERO(Ash::Utf8::Literal::getLength(u8"", 0, 4));
					TEST_IS_EQ(Ash::Utf8::Literal::getLength(u8"abc", 3, 4), 3);
					TEST_IS_EQ(Ash::Utf8::Literal::getLength(u8"abcde", 5, 4), 4);

					TEST_IS_ZERO(Ash::Utf8::Literal::getLength(nullptr, 4));
					TEST_IS_ZERO(Ash::Utf8::Literal::getLength(u8"", 4));
					TEST_IS_EQ(Ash::Utf8::Literal::getLength(u8"abc", 4), 3);
					TEST_IS_EQ(Ash::Utf8::Literal::getLength(u8"abcde", 4), 4);

					Ash::Utf8::View view;

					view = Ash::Utf8::Literal::getView(nullptr, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Utf8::Literal::getView(u8"", 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Utf8::Literal::getView(u8"abc", 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));

					view = Ash::Utf8::Literal::getView(u8"abc🤔", 3);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 5);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 6);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 7);
					TEST_IS_EQ(view.getLength(), 7);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc🤔", view.getLength()));

					view = Ash::Utf8::Literal::getView(u8"abcde", 4);
					TEST_IS_EQ(view.getLength(), 4);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abcde", view.getLength()));

					view = Ash::Utf8::Literal::getView(nullptr, 0, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Utf8::Literal::getView(u8"", 0, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Utf8::Literal::getView(u8"abc", 3, 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));

					view = Ash::Utf8::Literal::getView(u8"abc🤔", 7, 3);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 7, 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 7, 5);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 7, 6);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc", view.getLength()));
					view = Ash::Utf8::Literal::getView(u8"abc🤔", 7, 7);
					TEST_IS_EQ(view.getLength(), 7);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abc🤔", view.getLength()));

					view = Ash::Utf8::Literal::getView(u8"abcde", 5, 4);
					TEST_IS_EQ(view.getLength(), 4);
					TEST_IS_ZERO(::memcmp(view.at(0), u8"abcde", view.getLength()));

					return {};
				}

				static Ash::Test::Assertion wide()
				{
					TEST_IS_ZERO(Ash::Wide::Literal::getLength(nullptr));
					TEST_IS_ZERO(Ash::Wide::Literal::getLength(L""));
					TEST_IS_EQ(Ash::Wide::Literal::getLength(L"abc"), 3);

					TEST_IS_ZERO(Ash::Wide::Literal::getLength(nullptr, 0, 4));
					TEST_IS_ZERO(Ash::Wide::Literal::getLength(L"", 0, 4));
					TEST_IS_EQ(Ash::Wide::Literal::getLength(L"abc", 3, 4), 3);
					TEST_IS_EQ(Ash::Wide::Literal::getLength(L"abcde", 5, 4), 4);

					TEST_IS_ZERO(Ash::Wide::Literal::getLength(nullptr, 4));
					TEST_IS_ZERO(Ash::Wide::Literal::getLength(L"", 4));
					TEST_IS_EQ(Ash::Wide::Literal::getLength(L"abc", 4), 3);
					TEST_IS_EQ(Ash::Wide::Literal::getLength(L"abcde", 4), 4);

					Ash::Wide::View view;

					view = Ash::Wide::Literal::getView(nullptr, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Wide::Literal::getView(L"", 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Wide::Literal::getView(L"abc", 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), L"abc", view.getLength()));

					if constexpr (Ash::Encoding::Wide::minSize != Ash::Encoding::Wide::maxSize)
					{
						view = Ash::Wide::Literal::getView(L"abc🤔", 3);
						TEST_IS_EQ(view.getLength(), 3);
						TEST_IS_ZERO(::memcmp(view.at(0), L"abc", view.getLength()));
						view = Ash::Wide::Literal::getView(L"abc🤔", 4);
						TEST_IS_EQ(view.getLength(), 3);
						TEST_IS_ZERO(::memcmp(view.at(0), L"abc", view.getLength()));
						view = Ash::Wide::Literal::getView(L"abc🤔", 5);
						TEST_IS_EQ(view.getLength(), 5);
						TEST_IS_ZERO(::memcmp(view.at(0), L"abc🤔", view.getLength()));
					}

					view = Ash::Wide::Literal::getView(L"abcde", 4);
					TEST_IS_EQ(view.getLength(), 4);
					TEST_IS_ZERO(::memcmp(view.at(0), L"abcde", view.getLength()));

					view = Ash::Wide::Literal::getView(nullptr, 0, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Wide::Literal::getView(L"", 0, 4);
					TEST_IS_NULL(view.at(0));
					TEST_IS_ZERO(view.getLength());

					view = Ash::Wide::Literal::getView(L"abc", 3, 4);
					TEST_IS_EQ(view.getLength(), 3);
					TEST_IS_ZERO(::memcmp(view.at(0), L"abc", view.getLength()));

					if constexpr (Ash::Encoding::Wide::minSize != Ash::Encoding::Wide::maxSize)
					{
						view = Ash::Wide::Literal::getView(L"abc🤔", 5, 3);
						TEST_IS_EQ(view.getLength(), 3);
						TEST_IS_ZERO(::memcmp(view.at(0), L"abc", view.getLength()));
						view = Ash::Wide::Literal::getView(L"abc🤔", 5, 4);
						TEST_IS_EQ(view.getLength(), 3);
						TEST_IS_ZERO(::memcmp(view.at(0), L"abc", view.getLength()));
						view = Ash::Wide::Literal::getView(L"abc🤔", 5, 5);
						TEST_IS_EQ(view.getLength(), 5);
						TEST_IS_ZERO(::memcmp(view.at(0), L"abc🤔", view.getLength()));
					}

					view = Ash::Wide::Literal::getView(L"abcde", 5, 4);
					TEST_IS_EQ(view.getLength(), 4);
					TEST_IS_ZERO(::memcmp(view.at(0), L"abcde", view.getLength()));

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testString,
			TEST_CASE(Ash::Test::String::Literal::ascii),
			TEST_CASE(Ash::Test::String::Literal::utf8),
			TEST_CASE(Ash::Test::String::Literal::wide),
		);
	}
}
