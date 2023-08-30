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

				TEST_IS_TRUE(Ash::Type::isClass<TestBaseClass>());
				TEST_IS_TRUE(Ash::Type::isClass<TestDerivedClass>());
				TEST_IS_TRUE(Ash::Type::isClass<TestBaseStruct>());
				TEST_IS_TRUE(Ash::Type::isClass<TestDerivedStruct>());
				isClass = Ash::Type::isClass<TestDerivedClass, TestBaseClass>();
				TEST_IS_TRUE(isClass);
				isClass = Ash::Type::isClass<TestDerivedStruct, TestBaseStruct>();
				TEST_IS_TRUE(isClass);
				isClass = Ash::Type::isClass<TestBaseClass, TestDerivedClass>();
				TEST_IS_FALSE(isClass);
				isClass = Ash::Type::isClass<TestBaseStruct, TestDerivedStruct>();
				TEST_IS_FALSE(isClass);
				isClass = Ash::Type::isClass<TestDerivedClass, TestNonDerivedClass>();
				TEST_IS_FALSE(isClass);
				isClass = Ash::Type::isClass<TestDerivedStruct, TestNonDerivedStruct>();
				TEST_IS_FALSE(isClass);
				TEST_IS_FALSE(Ash::Type::isClass<float>());
				TEST_IS_FALSE(Ash::Type::isClass<double>());
				TEST_IS_FALSE(Ash::Type::isClass<long double>());
				TEST_IS_FALSE(Ash::Type::isClass<void>());
				TEST_IS_FALSE(Ash::Type::isClass<bool>());
				TEST_IS_FALSE(Ash::Type::isClass<char16_t>());
				TEST_IS_FALSE(Ash::Type::isClass<char32_t>());
				TEST_IS_FALSE(Ash::Type::isClass<wchar_t>());
				TEST_IS_FALSE(Ash::Type::isClass<signed char>());
				TEST_IS_FALSE(Ash::Type::isClass<unsigned char>());
				TEST_IS_FALSE(Ash::Type::isClass<signed short>());
				TEST_IS_FALSE(Ash::Type::isClass<unsigned short>());
				TEST_IS_FALSE(Ash::Type::isClass<signed long>());
				TEST_IS_FALSE(Ash::Type::isClass<unsigned long>());
				TEST_IS_FALSE(Ash::Type::isClass<signed long long>());
				TEST_IS_FALSE(Ash::Type::isClass<unsigned long long>());

				return {};
			}

			static Ash::Test::Assertion isFloatingPoint()
			{
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<float>());
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<double>());
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<long double>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<void>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<bool>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<char16_t>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<char32_t>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<wchar_t>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed char>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned char>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed short>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned short>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed long>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned long>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<signed long long>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<unsigned long long>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestBaseClass>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestDerivedClass>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestBaseStruct>());
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestDerivedStruct>());

				return {};
			}

			static Ash::Test::Assertion isInteger()
			{
				TEST_IS_TRUE(Ash::Type::isInteger<bool>());
				TEST_IS_TRUE(Ash::Type::isInteger<char16_t>());
				TEST_IS_TRUE(Ash::Type::isInteger<char32_t>());
				TEST_IS_TRUE(Ash::Type::isInteger<wchar_t>());
				TEST_IS_TRUE(Ash::Type::isInteger<signed char>());
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned char>());
				TEST_IS_TRUE(Ash::Type::isInteger<signed short>());
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned short>());
				TEST_IS_TRUE(Ash::Type::isInteger<signed long>());
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned long>());
				TEST_IS_TRUE(Ash::Type::isInteger<signed long long>());
				TEST_IS_TRUE(Ash::Type::isInteger<unsigned long long>());
				TEST_IS_FALSE(Ash::Type::isInteger<float>());
				TEST_IS_FALSE(Ash::Type::isInteger<double>());
				TEST_IS_FALSE(Ash::Type::isInteger<long double>());
				TEST_IS_FALSE(Ash::Type::isInteger<void>());
				TEST_IS_FALSE(Ash::Type::isInteger<TestBaseClass>());
				TEST_IS_FALSE(Ash::Type::isInteger<TestDerivedClass>());
				TEST_IS_FALSE(Ash::Type::isInteger<TestBaseStruct>());
				TEST_IS_FALSE(Ash::Type::isInteger<TestDerivedStruct>());

				return {};
			}

			static Ash::Test::Assertion isUnsignedInteger()
			{
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<bool>());
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<char16_t>());
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<char32_t>());
				TEST_IS_EQ(Ash::Type::isUnsignedInteger<wchar_t>(), sizeof(wchar_t) == 2);
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned char>());
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned short>());
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned long>());
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<unsigned long long>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed char>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed short>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed long>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<signed long long>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<float>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<double>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<long double>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<void>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestBaseClass>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestDerivedClass>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestBaseStruct>());
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestDerivedStruct>());

				return {};
			}

			static Ash::Test::Assertion isNumeric()
			{
				TEST_IS_TRUE(Ash::Type::isNumeric<bool>());
				TEST_IS_TRUE(Ash::Type::isNumeric<char16_t>());
				TEST_IS_TRUE(Ash::Type::isNumeric<char32_t>());
				TEST_IS_TRUE(Ash::Type::isNumeric<wchar_t>());
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned char>());
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned short>());
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned long>());
				TEST_IS_TRUE(Ash::Type::isNumeric<unsigned long long>());
				TEST_IS_TRUE(Ash::Type::isNumeric<signed char>());
				TEST_IS_TRUE(Ash::Type::isNumeric<signed short>());
				TEST_IS_TRUE(Ash::Type::isNumeric<signed long>());
				TEST_IS_TRUE(Ash::Type::isNumeric<signed long long>());
				TEST_IS_TRUE(Ash::Type::isNumeric<float>());
				TEST_IS_TRUE(Ash::Type::isNumeric<double>());
				TEST_IS_TRUE(Ash::Type::isNumeric<long double>());
				TEST_IS_FALSE(Ash::Type::isNumeric<void>());
				TEST_IS_FALSE(Ash::Type::isNumeric<TestBaseClass>());
				TEST_IS_FALSE(Ash::Type::isNumeric<TestDerivedClass>());
				TEST_IS_FALSE(Ash::Type::isNumeric<TestBaseStruct>());
				TEST_IS_FALSE(Ash::Type::isNumeric<TestDerivedStruct>());

				return {};
			}

			static Ash::Test::Assertion isConstant()
			{
				TEST_IS_TRUE(Ash::Type::isConstant<const bool>());
				TEST_IS_TRUE(Ash::Type::isConstant<const char16_t>());
				TEST_IS_TRUE(Ash::Type::isConstant<const char32_t>());
				TEST_IS_TRUE(Ash::Type::isConstant<const wchar_t>());
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned char>());
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned short>());
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned long>());
				TEST_IS_TRUE(Ash::Type::isConstant<const unsigned long long>());
				TEST_IS_TRUE(Ash::Type::isConstant<const signed char>());
				TEST_IS_TRUE(Ash::Type::isConstant<const signed short>());
				TEST_IS_TRUE(Ash::Type::isConstant<const signed long>());
				TEST_IS_TRUE(Ash::Type::isConstant<const signed long long>());
				TEST_IS_TRUE(Ash::Type::isConstant<const float>());
				TEST_IS_TRUE(Ash::Type::isConstant<const double>());
				TEST_IS_TRUE(Ash::Type::isConstant<const long double>());
				TEST_IS_TRUE(Ash::Type::isConstant<const void>());
				TEST_IS_TRUE(Ash::Type::isConstant<const TestBaseClass>());
				TEST_IS_TRUE(Ash::Type::isConstant<const TestDerivedClass>());
				TEST_IS_TRUE(Ash::Type::isConstant<const TestBaseStruct>());
				TEST_IS_TRUE(Ash::Type::isConstant<const TestDerivedStruct>());
				TEST_IS_FALSE(Ash::Type::isConstant<bool>());
				TEST_IS_FALSE(Ash::Type::isConstant<char16_t>());
				TEST_IS_FALSE(Ash::Type::isConstant<char32_t>());
				TEST_IS_FALSE(Ash::Type::isConstant<wchar_t>());
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned char>());
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned short>());
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned long>());
				TEST_IS_FALSE(Ash::Type::isConstant<unsigned long long>());
				TEST_IS_FALSE(Ash::Type::isConstant<signed char>());
				TEST_IS_FALSE(Ash::Type::isConstant<signed short>());
				TEST_IS_FALSE(Ash::Type::isConstant<signed long>());
				TEST_IS_FALSE(Ash::Type::isConstant<signed long long>());
				TEST_IS_FALSE(Ash::Type::isConstant<float>());
				TEST_IS_FALSE(Ash::Type::isConstant<double>());
				TEST_IS_FALSE(Ash::Type::isConstant<long double>());
				TEST_IS_FALSE(Ash::Type::isConstant<void>());
				TEST_IS_FALSE(Ash::Type::isConstant<TestBaseClass>());
				TEST_IS_FALSE(Ash::Type::isConstant<TestDerivedClass>());
				TEST_IS_FALSE(Ash::Type::isConstant<TestBaseStruct>());
				TEST_IS_FALSE(Ash::Type::isConstant<TestDerivedStruct>());

				return {};
			}

			static Ash::Test::Assertion isNotConstant()
			{
				TEST_IS_TRUE(Ash::Type::isNotConstant<bool>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<char16_t>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<char32_t>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<wchar_t>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned char>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned short>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned long>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<unsigned long long>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed char>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed short>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed long>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<signed long long>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<float>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<double>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<long double>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<void>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestBaseClass>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestDerivedClass>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestBaseStruct>());
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestDerivedStruct>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const bool>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const char16_t>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const char32_t>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const wchar_t>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned char>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned short>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned long>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const unsigned long long>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed char>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed short>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed long>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const signed long long>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const float>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const double>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const long double>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const void>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestBaseClass>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestDerivedClass>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestBaseStruct>());
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestDerivedStruct>());

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
			TEST_CASE(Ash::Test::Type::isNotConstant)
		);
	}
}
