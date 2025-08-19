#include "ash.callable.h"
#include "ash.ascii.h"
#include "ash.test.memory.trace.h"
#include "ash.test.callable.h"


namespace Ash
{
	namespace Test
	{
		namespace Callable
		{
			static int global = 0;

			static void voidFunction() { global = 0; }

			static void voidFunctionInt(int a) { global = a; }

			static int intFunction() { return global; }

			static int &intRefFunction() { static int internal = 0; return internal; };

			static const int &constIntRefFunction() { static int answer = 42; return answer; }

			static int intFunctionInt(int a) { return a * a; }

			static int intFunctionRefInt(int &a) { int old = a; a = (a + 1) * 2; return old; }

			static float floatFunctionFloatInt(float a, int b) { return a / b; }

			static int intFunctionTrace(Ash::Test::Memory::TraceValue<int> &&value) { return !value.isNull() ? *value : 0; }

			static int intFunctionIntCharString(int a, char b, const Ash::Ascii::View &c)
			{
				for (char n : c)
				{
					if (n == b)
					{
						a++;
					}
				}
				return a;
			}

			class Square
			{
			public:
				constexpr Square(double length) : m_Length(length) {}

				static constexpr Square create(double length) { return Square(length); }

				constexpr double getLength() const { return m_Length; }

				constexpr double getArea() const { return m_Length * m_Length; }

				constexpr double getPerimeter() const { return 4 * m_Length; }

				constexpr Square operator ()(double scale) const { return m_Length * scale; }

			private:
				double m_Length;
			};

			static Ash::Test::Assertion returnType()
			{
				bool isSame;

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(voidFunction)>::Type, void>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(voidFunctionInt), int>::Type, void>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(intFunction)>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(intRefFunction)>::Type, int &>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(constIntRefFunction)>::Type, const int &>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(intFunctionInt), int>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(intFunctionRefInt), int &>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(floatFunctionFloatInt), float, int>::Type, float>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(intFunctionTrace), Ash::Test::Memory::TraceValue<int> &&>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(intFunctionIntCharString), int, char, const Ash::Ascii::View &>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([]() {})>::Type, void>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([](int) {}), int>::Type, void>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([]() -> int { return 0; })>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([]() -> int & { return global; })>::Type, int &>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([]() -> const int & { return global; })>::Type, const int &>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([](int) -> int { return 0; }), int>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([](int &a) -> int { return a; }), int &>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([](float, int) -> float { return 0.5; }), float, int>::Type, float>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype([](int, char, const Ash::Ascii::View &) -> int { return 0; }), int, char, const Ash::Ascii::View &>::Type, int>;
				TEST_IS_TRUE(isSame);

				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(Square::create), double>::Type, Square>;
				TEST_IS_TRUE(isSame);

				Square square(2.0);
				isSame = Ash::Type::isSame<Ash::Callable::Return<decltype(&square), double>::Type, Square>;
				TEST_IS_TRUE(isSame);

				return {};
			}

			static Ash::Test::Assertion argument()
			{
				Ash::Callable::Argument<int> argumentInt(12);
				TEST_IS_EQ(argumentInt.get<0>(), 12);

				Ash::Callable::Argument<int &> argumentIntLValue(34);
				TEST_IS_EQ(argumentIntLValue.get<0>(), 34);

				Ash::Callable::Argument<const int &> argumentConstLValue(56);
				TEST_IS_EQ(argumentConstLValue.get<0>(), 56);

				Ash::Callable::Argument<int &&> argumentIntRValue(78);
				TEST_IS_EQ(argumentIntRValue.get<0>(), 78);

				int intValue = 1;
				Ash::Callable::Argument<decltype(std::ref(intValue))> argumentIntRef(std::ref(intValue));
				TEST_IS_EQ(argumentIntRef.get<0>(), 1);
				intValue = 2;
				TEST_IS_EQ(argumentIntRef.get<0>(), 2);

				unsigned char array1[4] = { 1, 2, 3, 4 };
				Ash::Callable::Argument<decltype(array1)> argumentArray(array1);
				TEST_IS_EQ(argumentArray.get<0>()[0], 1);
				TEST_IS_EQ(argumentArray.get<0>()[1], 2);
				TEST_IS_EQ(argumentArray.get<0>()[2], 3);
				TEST_IS_EQ(argumentArray.get<0>()[3], 4);
				Ash::Memory::clear(array1);
				TEST_IS_EQ(argumentArray.get<0>()[0], 1);
				TEST_IS_EQ(argumentArray.get<0>()[1], 2);
				TEST_IS_EQ(argumentArray.get<0>()[2], 3);
				TEST_IS_EQ(argumentArray.get<0>()[3], 4);

				unsigned char array2[4] = { 4, 3, 2, 1 };
				Ash::Callable::Argument<decltype(std::ref(array2))> argumentArrayRef(std::ref(array2));
				TEST_IS_EQ(argumentArrayRef.get<0>()[0], 4);
				TEST_IS_EQ(argumentArrayRef.get<0>()[1], 3);
				TEST_IS_EQ(argumentArrayRef.get<0>()[2], 2);
				TEST_IS_EQ(argumentArrayRef.get<0>()[3], 1);
				Ash::Memory::clear(array2);
				TEST_IS_EQ(argumentArrayRef.get<0>()[0], 0);
				TEST_IS_EQ(argumentArrayRef.get<0>()[1], 0);
				TEST_IS_EQ(argumentArrayRef.get<0>()[2], 0);
				TEST_IS_EQ(argumentArrayRef.get<0>()[3], 0);

				unsigned char array23[2][3] = { { 12, 23, 34 }, { 45, 56, 67 } };
				Ash::Callable::Argument<decltype(array23)> argumentArray23(array23);
				TEST_IS_EQ(argumentArray23.get<0>()[0][0], 12);
				TEST_IS_EQ(argumentArray23.get<0>()[0][1], 23);
				TEST_IS_EQ(argumentArray23.get<0>()[0][2], 34);
				TEST_IS_EQ(argumentArray23.get<0>()[1][0], 45);
				TEST_IS_EQ(argumentArray23.get<0>()[1][1], 56);
				TEST_IS_EQ(argumentArray23.get<0>()[1][2], 67);

				intValue = 55;
				unsigned char array3[4] = { 30, 31, 32, 33 };
				unsigned char array4[4] = { 40, 41, 42, 43 };
				Ash::Callable::Argument
				<
					int,
					int &,
					const int &,
					int &&,
					decltype(std::ref(intValue)),
					decltype(array1),
					decltype(std::ref(array2)),
					int
				> multiArgument(11, 22, 33, 44, std::ref(intValue), array3, std::ref(array4), 77);
				TEST_IS_EQ(multiArgument.get<0>(), 11);
				TEST_IS_EQ(multiArgument.get<1>(), 22);
				TEST_IS_EQ(multiArgument.get<2>(), 33);
				TEST_IS_EQ(multiArgument.get<3>(), 44);
				TEST_IS_EQ(multiArgument.get<4>(), 55);
				intValue = 66;
				TEST_IS_EQ(multiArgument.get<4>(), 66);
				TEST_IS_EQ(multiArgument.get<5>()[0], 30);
				TEST_IS_EQ(multiArgument.get<5>()[1], 31);
				TEST_IS_EQ(multiArgument.get<5>()[2], 32);
				TEST_IS_EQ(multiArgument.get<5>()[3], 33);
				Ash::Memory::clear(array3);
				TEST_IS_EQ(multiArgument.get<5>()[0], 30);
				TEST_IS_EQ(multiArgument.get<5>()[1], 31);
				TEST_IS_EQ(multiArgument.get<5>()[2], 32);
				TEST_IS_EQ(multiArgument.get<5>()[3], 33);
				TEST_IS_EQ(multiArgument.get<6>()[0], 40);
				TEST_IS_EQ(multiArgument.get<6>()[1], 41);
				TEST_IS_EQ(multiArgument.get<6>()[2], 42);
				TEST_IS_EQ(multiArgument.get<6>()[3], 43);
				Ash::Memory::clear(array4);
				TEST_IS_EQ(multiArgument.get<6>()[0], 0);
				TEST_IS_EQ(multiArgument.get<6>()[1], 0);
				TEST_IS_EQ(multiArgument.get<6>()[2], 0);
				TEST_IS_EQ(multiArgument.get<6>()[3], 0);
				TEST_IS_EQ(multiArgument.get<7>(), 77);

				return {};
			}

			static Ash::Test::Assertion function()
			{
				Ash::Callable::Function(voidFunctionInt, 12)();
				TEST_IS_EQ(global, 12);

				TEST_IS_EQ(Ash::Callable::Function(intFunction)(), 12);

				Ash::Callable::Function function = Ash::Callable::Function(voidFunction);
				function();
				TEST_IS_EQ(global, 0);

				TEST_IS_EQ(Ash::Callable::Function(intRefFunction)() = 123, 123);
				TEST_IS_EQ(Ash::Callable::Function(intRefFunction)(), 123);

				TEST_IS_EQ(Ash::Callable::Function(constIntRefFunction)(), 42);

				TEST_IS_EQ(Ash::Callable::Function(intFunctionInt, 12)(), 144);

				int initial = 24;
				TEST_IS_EQ(Ash::Callable::Function(intFunctionRefInt, std::ref(initial))(), 24);
				TEST_IS_EQ(initial, 50);

				TEST_IS_EQ(Ash::Callable::Function(floatFunctionFloatInt, 3, 2)(), 1.5);

				Ash::Test::Memory::TraceValue<int> traceValue = 99;
				TEST_IS_EQ(Ash::Callable::Function(intFunctionTrace, std::move(traceValue))(), 99);
				TEST_IS_TRUE(traceValue.isNull());
				TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

				TEST_IS_EQ(Ash::Callable::Function(intFunctionTrace, Ash::Test::Memory::TraceValue<int>(1234))(), 1234);
				TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

				TEST_IS_EQ(Ash::Callable::Function(intFunctionIntCharString, 10, 'l', Ash::Ascii::View("hello world"))(), 13);

				Ash::Callable::Function([](int a) { global = a; }, 24)();
				TEST_IS_EQ(global, 24);

				TEST_IS_EQ(Ash::Callable::Function([]() -> int { return global; })(), 24);

				Ash::Callable::Function([]() { global = 0; })();
				TEST_IS_EQ(global, 0);

				TEST_IS_EQ(Ash::Callable::Function([]() -> int & { return global; })() = 123, 123);
				TEST_IS_EQ(Ash::Callable::Function([]() -> int & { return global; })(), 123);

				TEST_IS_EQ(Ash::Callable::Function([]() -> const int & { return global; })(), 123);

				TEST_IS_EQ(Ash::Callable::Function([](int a) -> int { return a * a; }, 12)(), 144);

				TEST_IS_EQ(Ash::Callable::Function([](int &a) -> int { int old = a; a = (a + 1) * 2; return old; }, std::ref(initial))(), 50);
				TEST_IS_EQ(initial, 102);

				TEST_IS_EQ(Ash::Callable::Function([](float a, int b) -> float { return a / b; }, 3, 2)(), 1.5);

				TEST_IS_EQ(Ash::Callable::Function([](int a, char b, const Ash::Ascii::View &c) -> int
					{
						for (char n : c)
						{
							if (n == b)
							{
								a++;
							}
						}
						return a;
					},
					10, 'l', Ash::Ascii::View("hello world"))(), 13);

				TEST_IS_EQ(Ash::Callable::Function(Square::create, 2.0)().getArea(), 4.0);

				Square square(2.0);
				TEST_IS_EQ(Ash::Callable::Function(&square, 3.0)().getArea(), 36.0);

				return {};
			}

			static Ash::Test::Assertion genericFunction()
			{
				auto divide = [](double dividend, double divisor) -> double { return dividend / divisor; };
				auto square = [](double value) -> double { return value * value; };

				Ash::Callable::Function divideFunction(divide, 1, 2);
				Ash::Callable::Function squareFunction(square, 2);

				Ash::Callable::Generic::Function<double> *function;

				function = &divideFunction;
				TEST_IS_EQ((*function)(), 0.5);

				function = &squareFunction;
				TEST_IS_EQ((*function)(), 4);

				return {};
			}
		}

		TEST_UNIT
		(
			testCallable,
			TEST_CASE(Ash::Test::Callable::returnType),
			TEST_CASE(Ash::Test::Callable::argument),
			TEST_CASE(Ash::Test::Callable::function),
			TEST_CASE(Ash::Test::Callable::genericFunction)
		);
	}
}
