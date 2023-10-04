#include "ash.type.h"
#include "ash.test.type.h"


namespace Ash
{
	namespace Test
	{
		namespace Type
		{
			class TestBaseClass {};

			class TestDerivedClass : TestBaseClass {};

			class TestNonDerivedClass {};

			struct TestBaseStruct {};

			struct TestDerivedStruct : TestBaseStruct {};

			struct TestNonDerivedStruct {};

			static Ash::Test::Assertion isClass()
			{
				bool isClass = false;

				TEST_IS_TRUE(Ash::Type::isClass<TestBaseClass>);
				TEST_IS_TRUE(Ash::Type::isClass<TestDerivedClass>);
				TEST_IS_TRUE(Ash::Type::isClass<TestBaseStruct>);
				TEST_IS_TRUE(Ash::Type::isClass<TestDerivedStruct>);
				isClass = Ash::Type::isClass<TestDerivedClass, TestBaseClass>;
				TEST_IS_TRUE(isClass);
				isClass = Ash::Type::isClass<TestDerivedStruct, TestBaseStruct>;
				TEST_IS_TRUE(isClass);
				isClass = Ash::Type::isClass<TestBaseClass, TestDerivedClass>;
				TEST_IS_FALSE(isClass);
				isClass = Ash::Type::isClass<TestBaseStruct, TestDerivedStruct>;
				TEST_IS_FALSE(isClass);
				isClass = Ash::Type::isClass<TestDerivedClass, TestNonDerivedClass>;
				TEST_IS_FALSE(isClass);
				isClass = Ash::Type::isClass<TestDerivedStruct, TestNonDerivedStruct>;
				TEST_IS_FALSE(isClass);
				TEST_IS_FALSE(Ash::Type::isClass<float>);
				TEST_IS_FALSE(Ash::Type::isClass<double>);
				TEST_IS_FALSE(Ash::Type::isClass<long double>);
				TEST_IS_FALSE(Ash::Type::isClass<void>);
				TEST_IS_FALSE(Ash::Type::isClass<bool>);
				TEST_IS_FALSE(Ash::Type::isClass<char16_t>);
				TEST_IS_FALSE(Ash::Type::isClass<char32_t>);
				TEST_IS_FALSE(Ash::Type::isClass<wchar_t>);
				TEST_IS_FALSE(Ash::Type::isClass<signed char>);
				TEST_IS_FALSE(Ash::Type::isClass<unsigned char>);
				TEST_IS_FALSE(Ash::Type::isClass<signed int>);
				TEST_IS_FALSE(Ash::Type::isClass<unsigned int>);
				TEST_IS_FALSE(Ash::Type::isClass<signed short>);
				TEST_IS_FALSE(Ash::Type::isClass<unsigned short>);
				TEST_IS_FALSE(Ash::Type::isClass<signed long>);
				TEST_IS_FALSE(Ash::Type::isClass<unsigned long>);
				TEST_IS_FALSE(Ash::Type::isClass<signed long long>);
				TEST_IS_FALSE(Ash::Type::isClass<unsigned long long>);

				return {};
			}

			static Ash::Test::Assertion isFloatingPoint()
			{
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<float>);
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<double>);
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<long double>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<void>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<bool>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<char16_t>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<char32_t>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<wchar_t>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed char>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned char>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed int>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned int>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed short>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned short>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed long>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned long>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed long long>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestDerivedStruct>);

				return {};
			}

			static Ash::Test::Assertion isInteger()
			{
				TEST_IS_TRUE(Ash::Type::isInteger<bool>);
				TEST_IS_TRUE(Ash::Type::isInteger<char16_t>);
				TEST_IS_TRUE(Ash::Type::isInteger<char32_t>);
				TEST_IS_TRUE(Ash::Type::isInteger<wchar_t>);
				TEST_IS_TRUE(Ash::Type::isInteger<signed char>);
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isInteger<signed int>);
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isInteger<signed short>);
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isInteger<signed long>);
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isInteger<signed long long>);
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isInteger<float>);
				TEST_IS_FALSE(Ash::Type::isInteger<double>);
				TEST_IS_FALSE(Ash::Type::isInteger<long double>);
				TEST_IS_FALSE(Ash::Type::isInteger<void>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestDerivedStruct>);

				return {};
			}

			static Ash::Test::Assertion isUnsignedInteger()
			{
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<bool>);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<char16_t>);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<char32_t>);
				TEST_IS_EQ(Ash::Type::isUnsignedInteger<wchar_t>, sizeof(wchar_t) == 2);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed char>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed int>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed short>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed long>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed long long>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<float>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<double>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<long double>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<void>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestDerivedStruct>);

				return {};
			}

			static Ash::Test::Assertion isNumeric()
			{
				TEST_IS_TRUE(Ash::Type::isNumeric<bool>);
				TEST_IS_TRUE(Ash::Type::isNumeric<char16_t>);
				TEST_IS_TRUE(Ash::Type::isNumeric<char32_t>);
				TEST_IS_TRUE(Ash::Type::isNumeric<wchar_t>);
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned long long>);
				TEST_IS_TRUE(Ash::Type::isNumeric<signed char>);
				TEST_IS_TRUE(Ash::Type::isNumeric<signed int>);
				TEST_IS_TRUE(Ash::Type::isNumeric<signed short>);
				TEST_IS_TRUE(Ash::Type::isNumeric<signed long>);
				TEST_IS_TRUE(Ash::Type::isNumeric<signed long long>);
				TEST_IS_TRUE(Ash::Type::isNumeric<float>);
				TEST_IS_TRUE(Ash::Type::isNumeric<double>);
				TEST_IS_TRUE(Ash::Type::isNumeric<long double>);
				TEST_IS_FALSE(Ash::Type::isNumeric<void>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestDerivedStruct>);

				return {};
			}

			static Ash::Test::Assertion isConstant()
			{
				TEST_IS_TRUE(Ash::Type::isConstant<const bool>);
				TEST_IS_TRUE(Ash::Type::isConstant<const char16_t>);
				TEST_IS_TRUE(Ash::Type::isConstant<const char32_t>);
				TEST_IS_TRUE(Ash::Type::isConstant<const wchar_t>);
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned char>);
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned int>);
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned short>);
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned long>);
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned long long>);
				TEST_IS_TRUE(Ash::Type::isConstant<const signed char>);
				TEST_IS_TRUE(Ash::Type::isConstant<const signed int>);
				TEST_IS_TRUE(Ash::Type::isConstant<const signed short>);
				TEST_IS_TRUE(Ash::Type::isConstant<const signed long>);
				TEST_IS_TRUE(Ash::Type::isConstant<const signed long long>);
				TEST_IS_TRUE(Ash::Type::isConstant<const float>);
				TEST_IS_TRUE(Ash::Type::isConstant<const double>);
				TEST_IS_TRUE(Ash::Type::isConstant<const long double>);
				TEST_IS_TRUE(Ash::Type::isConstant<const void>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestBaseClass>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestDerivedClass>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestBaseStruct>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isConstant<bool>);
				TEST_IS_FALSE(Ash::Type::isConstant<char16_t>);
				TEST_IS_FALSE(Ash::Type::isConstant<char32_t>);
				TEST_IS_FALSE(Ash::Type::isConstant<wchar_t>);
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned char>);
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned int>);
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned short>);
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned long>);
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isConstant<signed char>);
				TEST_IS_FALSE(Ash::Type::isConstant<signed int>);
				TEST_IS_FALSE(Ash::Type::isConstant<signed short>);
				TEST_IS_FALSE(Ash::Type::isConstant<signed long>);
				TEST_IS_FALSE(Ash::Type::isConstant<signed long long>);
				TEST_IS_FALSE(Ash::Type::isConstant<float>);
				TEST_IS_FALSE(Ash::Type::isConstant<double>);
				TEST_IS_FALSE(Ash::Type::isConstant<long double>);
				TEST_IS_FALSE(Ash::Type::isConstant<void>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestDerivedStruct>);

				return {};
			}

			static Ash::Test::Assertion isNotConstant()
			{
				TEST_IS_TRUE(Ash::Type::isNotConstant<bool>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<char16_t>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<char32_t>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<wchar_t>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned long long>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed char>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed int>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed short>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed long>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed long long>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<float>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<double>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<long double>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<void>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestBaseClass>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestDerivedClass>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestBaseStruct>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const bool>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const char16_t>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const char32_t>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const wchar_t>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned char>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned int>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned short>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned long>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed char>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed int>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed short>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed long>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed long long>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const float>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const double>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const long double>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const void>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestDerivedStruct>);

				return {};
			}

			static Ash::Test::Assertion isSame()
			{
				bool isSame;

				isSame = Ash::Type::isSame<bool, bool>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<char16_t, char16_t>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<char32_t, char32_t>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<wchar_t, wchar_t>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<unsigned char, unsigned char>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<unsigned int, unsigned int>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<unsigned short, unsigned short>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<unsigned long, unsigned long>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<unsigned long long, unsigned long long>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<signed char, signed char>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<signed int, signed int>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<signed short, signed short>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<signed long, signed long>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<signed long long, signed long long>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<float, float>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<double, double>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<long double, long double>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<void, void>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<TestBaseClass, TestBaseClass>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<TestDerivedClass, TestDerivedClass>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<TestBaseStruct, TestBaseStruct>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<TestDerivedStruct, TestDerivedStruct>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<bool, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<char16_t, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<char32_t, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<wchar_t, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<unsigned char, signed char>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<unsigned int, signed int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<unsigned short, signed short>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<unsigned long, signed long>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<unsigned long long, signed long long>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<signed char, unsigned char>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<signed int, unsigned int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<signed short, unsigned short>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<signed long, unsigned long>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<signed long long, unsigned long long>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<float, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<double, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<long double, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<void, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<TestBaseClass, TestBaseStruct>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<TestBaseStruct, TestDerivedStruct>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<TestDerivedStruct, TestDerivedClass>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<TestDerivedClass, TestBaseClass>;
				TEST_IS_FALSE(isSame);

				return {};
			}

			static Ash::Test::Assertion isNotSame()
			{
				bool isNotSame;

				isNotSame = Ash::Type::isNotSame<bool, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<char16_t, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<char32_t, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<wchar_t, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned char, signed char>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned int, signed int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned short, signed short>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned long, signed long>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned long long, signed long long>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed char, unsigned char>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed int, unsigned int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed short, unsigned short>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed long, unsigned long>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed long long, unsigned long long>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<float, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<double, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<long double, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<void, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestBaseClass, TestBaseStruct>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestBaseStruct, TestDerivedStruct>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestDerivedStruct, TestDerivedClass>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestDerivedClass, TestBaseClass>;
				TEST_IS_TRUE(isNotSame);

				isNotSame = Ash::Type::isNotSame<bool, bool>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<char16_t, char16_t>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<char32_t, char32_t>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<wchar_t, wchar_t>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned char, unsigned char>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned int, unsigned int>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned short, unsigned short>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned long, unsigned long>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<unsigned long long, unsigned long long>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed char, signed char>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed int, signed int>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed short, signed short>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed long, signed long>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<signed long long, signed long long>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<float, float>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<double, double>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<long double, long double>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<void, void>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestBaseClass, TestBaseClass>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestDerivedClass, TestDerivedClass>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestBaseStruct, TestBaseStruct>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestDerivedStruct, TestDerivedStruct>;
				TEST_IS_FALSE(isNotSame);

				return {};
			}

			static Ash::Test::Assertion option()
			{
				bool isSame;

				isSame = Ash::Type::isSame<Ash::Type::Option<int, void, true>, int>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Type::Option<void, int, false>, int>;
				TEST_IS_TRUE(isSame);

				return {};
			}
		}

		TEST_UNIT
		(
			testType,
			TEST_CASE(Ash::Test::Type::isClass),
			TEST_CASE(Ash::Test::Type::isFloatingPoint),
			TEST_CASE(Ash::Test::Type::isInteger),
			TEST_CASE(Ash::Test::Type::isUnsignedInteger),
			TEST_CASE(Ash::Test::Type::isNumeric),
			TEST_CASE(Ash::Test::Type::isConstant),
			TEST_CASE(Ash::Test::Type::isNotConstant),
			TEST_CASE(Ash::Test::Type::isSame),
			TEST_CASE(Ash::Test::Type::isNotSame),
			TEST_CASE(Ash::Test::Type::option)
		);
	}
}
