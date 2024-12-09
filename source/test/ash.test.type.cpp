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
				#if STD >= 20
				TEST_IS_FALSE(Ash::Type::isClass<char8_t>);
				#endif
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
				TEST_IS_FALSE(Ash::Type::isClass<void *>);
				TEST_IS_FALSE(Ash::Type::isClass<int []>);
				TEST_IS_FALSE(Ash::Type::isClass<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isClass<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isNotClass()
			{
				bool isNotClass = false;

				isNotClass = Ash::Type::isNotClass<TestBaseClass, TestDerivedClass>;
				TEST_IS_TRUE(isNotClass);
				isNotClass = Ash::Type::isNotClass<TestBaseStruct, TestDerivedStruct>;
				TEST_IS_TRUE(isNotClass);
				isNotClass = Ash::Type::isNotClass<TestDerivedClass, TestNonDerivedClass>;
				TEST_IS_TRUE(isNotClass);
				isNotClass = Ash::Type::isNotClass<TestDerivedStruct, TestNonDerivedStruct>;
				TEST_IS_TRUE(isNotClass);
				TEST_IS_TRUE(Ash::Type::isNotClass<float>);
				TEST_IS_TRUE(Ash::Type::isNotClass<double>);
				TEST_IS_TRUE(Ash::Type::isNotClass<long double>);
				TEST_IS_TRUE(Ash::Type::isNotClass<void>);
				TEST_IS_TRUE(Ash::Type::isNotClass<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isNotClass<char8_t>);
				#endif
				TEST_IS_TRUE(Ash::Type::isNotClass<char16_t>);
				TEST_IS_TRUE(Ash::Type::isNotClass<char32_t>);
				TEST_IS_TRUE(Ash::Type::isNotClass<wchar_t>);
				TEST_IS_TRUE(Ash::Type::isNotClass<signed char>);
				TEST_IS_TRUE(Ash::Type::isNotClass<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isNotClass<signed int>);
				TEST_IS_TRUE(Ash::Type::isNotClass<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isNotClass<signed short>);
				TEST_IS_TRUE(Ash::Type::isNotClass<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isNotClass<signed long>);
				TEST_IS_TRUE(Ash::Type::isNotClass<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isNotClass<signed long long>);
				TEST_IS_TRUE(Ash::Type::isNotClass<unsigned long long>);
				TEST_IS_TRUE(Ash::Type::isNotClass<void *>);
				TEST_IS_TRUE(Ash::Type::isNotClass<int []>);
				TEST_IS_TRUE(Ash::Type::isNotClass<int [][2]>);
				TEST_IS_TRUE(Ash::Type::isNotClass<TestBaseClass []>);
				TEST_IS_FALSE(Ash::Type::isNotClass<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isNotClass<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isNotClass<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isNotClass<TestDerivedStruct>);
				isNotClass = Ash::Type::isNotClass<TestDerivedClass, TestBaseClass>;
				TEST_IS_FALSE(isNotClass);
				isNotClass = Ash::Type::isNotClass<TestDerivedStruct, TestBaseStruct>;
				TEST_IS_FALSE(isNotClass);

				return {};
			}

			static Ash::Test::Assertion isArray()
			{
				TEST_IS_TRUE(Ash::Type::isArray<int []>);
				TEST_IS_TRUE(Ash::Type::isArray<int [][2]>);
				TEST_IS_TRUE(Ash::Type::isArray<TestBaseClass []>);
				TEST_IS_FALSE(Ash::Type::isArray<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isArray<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isArray<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isArray<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isArray<float>);
				TEST_IS_FALSE(Ash::Type::isArray<double>);
				TEST_IS_FALSE(Ash::Type::isArray<long double>);
				TEST_IS_FALSE(Ash::Type::isArray<void>);
				TEST_IS_FALSE(Ash::Type::isArray<bool>);
				#if STD >= 20
				TEST_IS_FALSE(Ash::Type::isArray<char8_t>);
				#endif
				TEST_IS_FALSE(Ash::Type::isArray<char16_t>);
				TEST_IS_FALSE(Ash::Type::isArray<char32_t>);
				TEST_IS_FALSE(Ash::Type::isArray<wchar_t>);
				TEST_IS_FALSE(Ash::Type::isArray<signed char>);
				TEST_IS_FALSE(Ash::Type::isArray<unsigned char>);
				TEST_IS_FALSE(Ash::Type::isArray<signed int>);
				TEST_IS_FALSE(Ash::Type::isArray<unsigned int>);
				TEST_IS_FALSE(Ash::Type::isArray<signed short>);
				TEST_IS_FALSE(Ash::Type::isArray<unsigned short>);
				TEST_IS_FALSE(Ash::Type::isArray<signed long>);
				TEST_IS_FALSE(Ash::Type::isArray<unsigned long>);
				TEST_IS_FALSE(Ash::Type::isArray<signed long long>);
				TEST_IS_FALSE(Ash::Type::isArray<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isArray<void *>);

				return {};
			}

			static Ash::Test::Assertion isNotArray()
			{
				TEST_IS_TRUE(Ash::Type::isNotArray<TestBaseClass>);
				TEST_IS_TRUE(Ash::Type::isNotArray<TestDerivedClass>);
				TEST_IS_TRUE(Ash::Type::isNotArray<TestBaseStruct>);
				TEST_IS_TRUE(Ash::Type::isNotArray<TestDerivedStruct>);
				TEST_IS_TRUE(Ash::Type::isNotArray<float>);
				TEST_IS_TRUE(Ash::Type::isNotArray<double>);
				TEST_IS_TRUE(Ash::Type::isNotArray<long double>);
				TEST_IS_TRUE(Ash::Type::isNotArray<void>);
				TEST_IS_TRUE(Ash::Type::isNotArray<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isNotArray<char8_t>);
				#endif
				TEST_IS_TRUE(Ash::Type::isNotArray<char16_t>);
				TEST_IS_TRUE(Ash::Type::isNotArray<char32_t>);
				TEST_IS_TRUE(Ash::Type::isNotArray<wchar_t>);
				TEST_IS_TRUE(Ash::Type::isNotArray<signed char>);
				TEST_IS_TRUE(Ash::Type::isNotArray<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isNotArray<signed int>);
				TEST_IS_TRUE(Ash::Type::isNotArray<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isNotArray<signed short>);
				TEST_IS_TRUE(Ash::Type::isNotArray<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isNotArray<signed long>);
				TEST_IS_TRUE(Ash::Type::isNotArray<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isNotArray<signed long long>);
				TEST_IS_TRUE(Ash::Type::isNotArray<unsigned long long>);
				TEST_IS_TRUE(Ash::Type::isNotArray<void *>);
				TEST_IS_FALSE(Ash::Type::isNotArray<int []>);
				TEST_IS_FALSE(Ash::Type::isNotArray<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isNotArray<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isPrimitive()
			{
				TEST_IS_TRUE(Ash::Type::isPrimitive<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isPrimitive<char8_t>);
				#endif
				TEST_IS_TRUE(Ash::Type::isPrimitive<char16_t>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<char32_t>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<wchar_t>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<unsigned long long>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<signed char>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<signed int>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<signed short>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<signed long>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<signed long long>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<float>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<double>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<long double>);
				TEST_IS_TRUE(Ash::Type::isPrimitive<void *>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<void>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<int []>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isPrimitive<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isFloatingPoint()
			{
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<float>);
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<double>);
				TEST_IS_TRUE(Ash::Type::isFloatingPoint<long double>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<void>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<bool>);
				#if STD >= 20
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<char8_t>);
				#endif
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
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<void *>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<int []>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isFloatingPoint<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isInteger()
			{
				TEST_IS_TRUE(Ash::Type::isInteger<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isInteger<char8_t>);
				#endif
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
				TEST_IS_FALSE(Ash::Type::isInteger<void *>);
				TEST_IS_FALSE(Ash::Type::isInteger<void>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isInteger<int []>);
				TEST_IS_FALSE(Ash::Type::isInteger<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isInteger<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isSignedInteger()
			{
				TEST_IS_TRUE(Ash::Type::isSignedInteger<signed char>);
				TEST_IS_TRUE(Ash::Type::isSignedInteger<signed int>);
				TEST_IS_TRUE(Ash::Type::isSignedInteger<signed short>);
				TEST_IS_TRUE(Ash::Type::isSignedInteger<signed long>);
				TEST_IS_TRUE(Ash::Type::isSignedInteger<signed long long>);
				TEST_IS_EQ(Ash::Type::isSignedInteger<wchar_t>, sizeof(wchar_t) != 2);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<bool>);
				#if STD >= 20
				TEST_IS_FALSE(Ash::Type::isSignedInteger<char8_t>);
				#endif
				TEST_IS_FALSE(Ash::Type::isSignedInteger<char16_t>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<char32_t>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<unsigned char>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<unsigned int>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<unsigned short>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<unsigned long>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<float>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<double>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<long double>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<void *>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<void>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<int []>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isSignedInteger<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isUnsignedInteger()
			{
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isUnsignedInteger<char8_t>);
				#endif
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
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<void *>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<void>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<int []>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isUnsignedInteger<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isNumeric()
			{
				TEST_IS_TRUE(Ash::Type::isNumeric<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isNumeric<char8_t>);
				#endif
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
				TEST_IS_FALSE(Ash::Type::isNumeric<void *>);
				TEST_IS_FALSE(Ash::Type::isNumeric<void>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isNumeric<int []>);
				TEST_IS_FALSE(Ash::Type::isNumeric<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isNumeric<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isPointer()
			{
				TEST_IS_TRUE(Ash::Type::isPointer<void *>);
				TEST_IS_FALSE(Ash::Type::isPointer<bool>);
				#if STD >= 20
				TEST_IS_FALSE(Ash::Type::isPointer<char8_t>);
				#endif
				TEST_IS_FALSE(Ash::Type::isPointer<char16_t>);
				TEST_IS_FALSE(Ash::Type::isPointer<char32_t>);
				TEST_IS_FALSE(Ash::Type::isPointer<wchar_t>);
				TEST_IS_FALSE(Ash::Type::isPointer<unsigned char>);
				TEST_IS_FALSE(Ash::Type::isPointer<unsigned int>);
				TEST_IS_FALSE(Ash::Type::isPointer<unsigned short>);
				TEST_IS_FALSE(Ash::Type::isPointer<unsigned long>);
				TEST_IS_FALSE(Ash::Type::isPointer<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isPointer<signed char>);
				TEST_IS_FALSE(Ash::Type::isPointer<signed int>);
				TEST_IS_FALSE(Ash::Type::isPointer<signed short>);
				TEST_IS_FALSE(Ash::Type::isPointer<signed long>);
				TEST_IS_FALSE(Ash::Type::isPointer<signed long long>);
				TEST_IS_FALSE(Ash::Type::isPointer<float>);
				TEST_IS_FALSE(Ash::Type::isPointer<double>);
				TEST_IS_FALSE(Ash::Type::isPointer<long double>);
				TEST_IS_FALSE(Ash::Type::isPointer<void>);
				TEST_IS_FALSE(Ash::Type::isPointer<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isPointer<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isPointer<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isPointer<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isPointer<int []>);
				TEST_IS_FALSE(Ash::Type::isPointer<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isPointer<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isIntegerOrPointer()
			{
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<char8_t>);
				#endif
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<char16_t>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<char32_t>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<wchar_t>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<signed char>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<signed int>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<unsigned int>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<signed short>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<unsigned short>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<signed long>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<unsigned long>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<signed long long>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<unsigned long long>);
				TEST_IS_TRUE(Ash::Type::isIntegerOrPointer<void *>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<float>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<double>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<long double>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<void>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<int []>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isIntegerOrPointer<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isConstant()
			{
				TEST_IS_TRUE(Ash::Type::isConstant<const bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isConstant<const char8_t>);
				#endif
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
				TEST_IS_TRUE(Ash::Type::isConstant<void * const>);
				TEST_IS_TRUE(Ash::Type::isConstant<const void>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestBaseClass>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestDerivedClass>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestBaseStruct>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestDerivedStruct>);
				TEST_IS_TRUE(Ash::Type::isConstant<const int []>);
				TEST_IS_TRUE(Ash::Type::isConstant<const int [][2]>);
				TEST_IS_TRUE(Ash::Type::isConstant<const TestBaseClass []>);
				TEST_IS_FALSE(Ash::Type::isConstant<bool>);
				#if STD >= 20
				TEST_IS_FALSE(Ash::Type::isConstant<char8_t>);
				#endif
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
				TEST_IS_FALSE(Ash::Type::isConstant<void *>);
				TEST_IS_FALSE(Ash::Type::isConstant<void>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isConstant<int []>);
				TEST_IS_FALSE(Ash::Type::isConstant<int [][2]>);
				TEST_IS_FALSE(Ash::Type::isConstant<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isNotConstant()
			{
				TEST_IS_TRUE(Ash::Type::isNotConstant<bool>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isNotConstant<char8_t>);
				#endif
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
				TEST_IS_TRUE(Ash::Type::isNotConstant<void *>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<void>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestBaseClass>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestDerivedClass>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestBaseStruct>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestDerivedStruct>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<int []>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<int [][2]>);
				TEST_IS_TRUE(Ash::Type::isNotConstant<TestBaseClass []>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const bool>);
				#if STD >= 20
				TEST_IS_FALSE(Ash::Type::isNotConstant<const char8_t>);
				#endif
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
				TEST_IS_FALSE(Ash::Type::isNotConstant<void * const>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const void>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestDerivedStruct>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const int []>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const int [][2]>);
				TEST_IS_FALSE(Ash::Type::isNotConstant<const TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion isSame()
			{
				bool isSame;

				isSame = Ash::Type::isSame<bool, bool>;
				TEST_IS_TRUE(isSame);
				#if STD >= 20
				isSame = Ash::Type::isSame<char8_t, char8_t>;
				TEST_IS_TRUE(isSame);
				#endif
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
				isSame = Ash::Type::isSame<void *, void *>;
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
				isSame = Ash::Type::isSame<int [], int []>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<int [][2], int [][2]>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<TestBaseClass [], TestBaseClass []>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<int, int, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<bool, int>;
				TEST_IS_FALSE(isSame);
				#if STD >= 20
				isSame = Ash::Type::isSame<char8_t, int>;
				TEST_IS_FALSE(isSame);
				#endif
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
				isSame = Ash::Type::isSame<void *, int>;
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
				isSame = Ash::Type::isSame<int [], int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<int [][2], int []>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<TestBaseClass [], TestBaseClass>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<bool, int, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<int, bool, int>;
				TEST_IS_FALSE(isSame);
				isSame = Ash::Type::isSame<int, int, bool>;
				TEST_IS_FALSE(isSame);

				return {};
			}

			static Ash::Test::Assertion isNotSame()
			{
				bool isNotSame;

				isNotSame = Ash::Type::isNotSame<bool, int>;
				TEST_IS_TRUE(isNotSame);
				#if STD >= 20
				isNotSame = Ash::Type::isNotSame<char8_t, int>;
				TEST_IS_TRUE(isNotSame);
				#endif
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
				isNotSame = Ash::Type::isNotSame<void *, int>;
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
				isNotSame = Ash::Type::isNotSame<int [], int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<int [][2], int []>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestBaseClass [], TestBaseClass>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<bool, int, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<int, bool, int>;
				TEST_IS_TRUE(isNotSame);
				isNotSame = Ash::Type::isNotSame<int, int, bool>;
				TEST_IS_TRUE(isNotSame);

				isNotSame = Ash::Type::isNotSame<bool, bool>;
				TEST_IS_FALSE(isNotSame);
				#if STD >= 20
				isNotSame = Ash::Type::isNotSame<char8_t, char8_t>;
				TEST_IS_FALSE(isNotSame);
				#endif
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
				isNotSame = Ash::Type::isNotSame<void *, void *>;
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
				isNotSame = Ash::Type::isNotSame<int [], int []>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<int [][2], int [][2]>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<TestBaseClass [], TestBaseClass []>;
				TEST_IS_FALSE(isNotSame);
				isNotSame = Ash::Type::isNotSame<int, int, int>;
				TEST_IS_FALSE(isNotSame);

				return {};
			}

			static Ash::Test::Assertion isSameSize()
			{
				bool isSameSize;

				isSameSize = Ash::Type::isSameSize<signed char, unsigned char>;
				TEST_IS_TRUE(isSameSize);
				isSameSize = Ash::Type::isSameSize<signed char, signed short>;
				TEST_IS_FALSE(isSameSize);
				isSameSize = Ash::Type::isSameSize<signed char, unsigned char, signed char>;
				TEST_IS_TRUE(isSameSize);
				isSameSize = Ash::Type::isSameSize<signed short, signed char, unsigned char>;
				TEST_IS_FALSE(isSameSize);
				isSameSize = Ash::Type::isSameSize<signed char, signed short, unsigned char>;
				TEST_IS_FALSE(isSameSize);
				isSameSize = Ash::Type::isSameSize<signed char, unsigned char, signed short>;
				TEST_IS_FALSE(isSameSize);

				return {};
			}

			static Ash::Test::Assertion isByteSizeInteger()
			{
				TEST_IS_TRUE(Ash::Type::isByteSizeInteger<unsigned char>);
				TEST_IS_TRUE(Ash::Type::isByteSizeInteger<signed char>);
				TEST_IS_TRUE(Ash::Type::isByteSizeInteger<char>);
				#if STD >= 20
				TEST_IS_TRUE(Ash::Type::isByteSizeInteger<char8_t>);
				#endif
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<char16_t>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<char32_t>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<wchar_t>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<unsigned int>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<unsigned short>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<unsigned long>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<unsigned long long>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<signed int>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<signed short>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<signed long>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<signed long long>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<float>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<double>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<long double>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<void *>);
				//TEST_IS_FALSE(Ash::Type::isByteSizeInteger<void>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<TestBaseClass>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<TestDerivedClass>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<TestBaseStruct>);
				TEST_IS_FALSE(Ash::Type::isByteSizeInteger<TestDerivedStruct>);
				//TEST_IS_FALSE(Ash::Type::isByteSizeInteger<int []>);
				//TEST_IS_FALSE(Ash::Type::isByteSizeInteger<int [][2]>);
				//TEST_IS_FALSE(Ash::Type::isByteSizeInteger<TestBaseClass []>);

				return {};
			}

			static Ash::Test::Assertion array()
			{
				bool isSame;

				using Array3 = int [2][3][4];
				TEST_IS_EQ(Ash::Type::Array<Array3>::size, 2);

				using Array2 = Ash::Type::Array<Array3>::Type;
				isSame = Ash::Type::isSame<Array2, int [3][4]>;
				TEST_IS_TRUE(isSame);
				TEST_IS_EQ(Ash::Type::Array<Array2>::size, 3);

				using Array1 = Ash::Type::Array<Array2>::Type;
				isSame = Ash::Type::isSame<Array1, int [4]>;
				TEST_IS_TRUE(isSame);
				TEST_IS_EQ(Ash::Type::Array<Array1>::size, 4);

				using Type = Ash::Type::Array<Array1>::Type;
				isSame = Ash::Type::isSame<Type, int>;
				TEST_IS_TRUE(isSame);

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

			static Ash::Test::Assertion index()
			{
				bool isSame;

				isSame = Ash::Type::isSame<Ash::Type::Index<0, int, float, const char *>, int>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Type::Index<1, int, float, const char *>, float>;
				TEST_IS_TRUE(isSame);
				isSame = Ash::Type::isSame<Ash::Type::Index<2, int, float, const char *>, const char *>;
				TEST_IS_TRUE(isSame);

				return {};
			}

			static Ash::Test::Assertion checkIfAny()
			{
				bool isValid;
				
				isValid = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<char, short>, Ash::Type::Requirement::IsSame<char, long>>::isValid;
				TEST_IS_FALSE(isValid);
				isValid = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<short, short>, Ash::Type::Requirement::IsSame<short, long>>::isValid;
				TEST_IS_TRUE(isValid);
				isValid = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<long, short>, Ash::Type::Requirement::IsSame<long, long>>::isValid;
				TEST_IS_TRUE(isValid);

				return {};
			}

			static Ash::Test::Assertion checkIfAll()
			{
				bool isValid;
				
				isValid = Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<signed char>, Ash::Type::Requirement::IsUnsigned<signed char>>::isValid;
				TEST_IS_FALSE(isValid);
				isValid = Ash::Type::CheckIfAll<Ash::Type::Requirement::IsUnsigned<signed char>, Ash::Type::Requirement::IsInteger<signed char>>::isValid;
				TEST_IS_FALSE(isValid);
				isValid = Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<unsigned char>, Ash::Type::Requirement::IsUnsigned<unsigned char>>::isValid;
				TEST_IS_TRUE(isValid);
				isValid = Ash::Type::CheckIfAll<Ash::Type::Requirement::IsUnsigned<unsigned char>, Ash::Type::Requirement::IsInteger<unsigned char>>::isValid;
				TEST_IS_TRUE(isValid);

				return {};
			}
		}

		TEST_UNIT
		(
			testType,
			TEST_CASE(Ash::Test::Type::isClass),
			TEST_CASE(Ash::Test::Type::isNotClass),
			TEST_CASE(Ash::Test::Type::isArray),
			TEST_CASE(Ash::Test::Type::isNotArray),
			TEST_CASE(Ash::Test::Type::isPrimitive),
			TEST_CASE(Ash::Test::Type::isFloatingPoint),
			TEST_CASE(Ash::Test::Type::isInteger),
			TEST_CASE(Ash::Test::Type::isSignedInteger),
			TEST_CASE(Ash::Test::Type::isUnsignedInteger),
			TEST_CASE(Ash::Test::Type::isNumeric),
			TEST_CASE(Ash::Test::Type::isPointer),
			TEST_CASE(Ash::Test::Type::isIntegerOrPointer),
			TEST_CASE(Ash::Test::Type::isConstant),
			TEST_CASE(Ash::Test::Type::isNotConstant),
			TEST_CASE(Ash::Test::Type::isSame),
			TEST_CASE(Ash::Test::Type::isNotSame),
			TEST_CASE(Ash::Test::Type::isSameSize),
			TEST_CASE(Ash::Test::Type::isByteSizeInteger),
			TEST_CASE(Ash::Test::Type::array),
			TEST_CASE(Ash::Test::Type::option),
			TEST_CASE(Ash::Test::Type::index),
			TEST_CASE(Ash::Test::Type::checkIfAny),
			TEST_CASE(Ash::Test::Type::checkIfAll)
		);
	}
}
