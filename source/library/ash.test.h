#pragma once

#include <iostream>


namespace Ash
{
	namespace Test
	{
		class Location
		{
		public:
			using LineNumber = unsigned long;

			constexpr Location() : m_FileName(nullptr), m_LineNumber(0) {}

			constexpr Location(const char *fileName, LineNumber lineNumber) : m_FileName(fileName), m_LineNumber(lineNumber) {}

			constexpr const char *getFileName() const { return m_FileName; }

			constexpr LineNumber getLineNumber() const { return m_LineNumber; }

			friend std::ostream &operator << (std::ostream &stream, const Location &location)
			{
				return (location.m_FileName != nullptr) ? stream << location.m_FileName << ':' << location.m_LineNumber : stream;
			}

		private:
			const char *m_FileName;
			LineNumber  m_LineNumber;
		};

		class Assertion
		{
		public:
			constexpr Assertion() : m_IsValid(true), m_Condition(nullptr), m_Actual(nullptr), m_Expected(nullptr), m_Location() {}

			constexpr Assertion(const char *actual, const char *condition, const char *expected, const Location &location) : m_IsValid(false), m_Condition(condition), m_Actual(actual), m_Expected(expected), m_Location(location) {}

			#ifdef DEBUG
			#define TEST_ASSERTION(predicate, actual, condition, expected) if (!(predicate)) { return __builtin_trap(), Ash::Test::Assertion(actual, condition, expected, Ash::Test::Location(__FILE__, __LINE__)); }
			#else
			#define TEST_ASSERTION(predicate, actual, condition, expected) if (!(predicate)) { return Ash::Test::Assertion(actual, condition, expected, Ash::Test::Location(__FILE__, __LINE__)); }
			#endif

			#define TEST_IS_EQ(actual, expected) TEST_ASSERTION((actual) == (expected), #actual, "equal to", #expected)

			#define TEST_IS_NOT_EQ(actual, expected) TEST_ASSERTION((actual) != (expected), #actual, "not equal to", #expected)

			#define TEST_IS_LT(actual, expected) TEST_ASSERTION((actual) < (expected), #actual, "less than", #expected)

			#define TEST_IS_LTE(actual, expected) TEST_ASSERTION((actual) <= (expected), #actual, "less than or equal to", #expected)

			#define TEST_IS_GT(actual, expected) TEST_ASSERTION((actual) > (expected), #actual, "greater than", #expected)

			#define TEST_IS_GTE(actual, expected) TEST_ASSERTION((actual) >= (expected), #actual, "greater than or equal to", #expected)

			#define TEST_IS_TRUE(value) TEST_IS_EQ(value, true)

			#define TEST_IS_FALSE(value) TEST_IS_EQ(value, false)

			#define TEST_IS_NULL(value) TEST_IS_EQ(value, nullptr)

			#define TEST_IS_NOT_NULL(value) TEST_IS_NOT_EQ(value, nullptr)

			#define TEST_IS_ZERO(value) TEST_IS_EQ(value, 0)

			#define TEST_IS_NOT_ZERO(value) TEST_IS_NOT_EQ(value, 0)

			#define TEST(function) { Ash::Test::Assertion assertion = function(); if (!(assertion).isValid()) { return assertion; } }

			#define TEST_GENERIC(function, ...) { Ash::Test::Assertion assertion = function<__VA_ARGS__>(); if (!(assertion).isValid()) { return assertion; } }

			#define TEST_CLASS_GENERIC(class, function, ...) { Ash::Test::Assertion assertion = class<__VA_ARGS__>::function(); if (!(assertion).isValid()) { return assertion; } }

			constexpr bool isValid() const { return m_IsValid; }

			constexpr bool getCondition() const { return m_Condition; }

			constexpr const char *getActual() const { return m_Actual; }

			constexpr const char *getExpected() const { return m_Expected; }

			constexpr Location getLocation() const { return m_Location; }

			friend std::ostream &operator << (std::ostream &stream, const Assertion &assertion)
			{
				if (assertion.m_IsValid)
				{
					stream << "PASS";
				}
				else
				{
					stream << "FAIL: Expected ";
					stream << '\'' << assertion.m_Actual << '\'';
					stream << " to be " << assertion.m_Condition << ' ';
					stream << '\'' << assertion.m_Expected << '\'';
					stream << " - see ";
					stream << '\'' << assertion.m_Location << '\'';
				}
				return stream;
			}

		private:
			bool        m_IsValid;
			const char *m_Condition;
			const char *m_Actual;
			const char *m_Expected;
			Location    m_Location;
		};

		class Result
		{
		public:
			using Count = unsigned long;

			constexpr Result() : m_PassCount(0), m_FailureCount(0) {}

			constexpr Result(bool hasPassed) : m_PassCount(hasPassed ? 1 : 0), m_FailureCount(hasPassed ? 0 : 1) {}

			constexpr Result operator + (const Result &result) const
			{
				return { m_PassCount + result.m_PassCount, m_FailureCount + result.m_FailureCount };
			}

			constexpr Count getPassCount() const { return m_PassCount; }

			constexpr Count getFailureCount() const { return m_FailureCount; }

			constexpr Count getTotalCount() const { return m_PassCount + m_FailureCount; }

			friend std::ostream &operator << (std::ostream &stream, const Result &result)
			{
				stream << "Total tests: " << (result.m_PassCount + result.m_FailureCount) << '\n';
				stream << "Passed: " << result.m_PassCount << '\n';
				stream << "Failed: " << result.m_FailureCount << '\n';
				return stream;
			}

		protected:
			constexpr Result(Count passCount, Count failureCount) : m_PassCount(passCount), m_FailureCount(failureCount) {}

		private:
			Count m_PassCount;
			Count m_FailureCount;
		};

		using Capture = void (*)(const char *name, const Assertion &assertion);

		inline void outputCapture(const char *name, const Assertion &assertion)
		{
			std::cout << name << ", " << assertion << '\n';
		}

		class Case
		{
		public:
			using Function = Assertion (*)();

			constexpr Case(const char *name, Function function) : m_Name(name), m_Function(function) {}

			constexpr const char *getName() const { return m_Name; }

			constexpr Assertion run(Capture capture = outputCapture) const
			{
				Assertion assert = m_Function();

				capture(m_Name, assert);

				return assert;
			}

		private:
			const char *m_Name;
			Function    m_Function;
		};

		class Unit
		{
		public:
			using Function = Result (*)(Capture);

			constexpr Unit(Function function) : m_Function(function) {}

			static constexpr Result run(std::initializer_list<Case> caseList, Capture capture = outputCapture)
			{
				Result result;
				
				for (Case testCase : caseList)
				{
					Assertion assertion = testCase.run(capture);

					result = result + Result(assertion.isValid());
				}

				return result;
			}

			static constexpr Result run(std::initializer_list<Unit> unitList, Capture capture = outputCapture)
			{
				Result result;
				
				for (Unit testUnit : unitList)
				{
					result = result + testUnit.m_Function(capture);
				}

				return result;
			}

		private:
			Function m_Function;
		};

		#define TEST_CASE(name) Ash::Test::Case(#name, name)

		#define TEST_CASE_GENERIC(name, ...) Ash::Test::Case(#name "<" #__VA_ARGS__ ">", name<__VA_ARGS__>)

		#define TEST_UNIT(name, ...) Ash::Test::Unit name( [](Ash::Test::Capture capture) { return Ash::Test::Unit::run({ __VA_ARGS__ }, capture); } )
	}
}
