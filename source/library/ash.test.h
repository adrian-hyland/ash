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

			std::ostream &operator << (std::ostream &stream) const
			{
				return (m_FileName != nullptr) ? stream << m_FileName << ':' << m_LineNumber : stream;
			}

		private:
			const char *m_FileName;
			LineNumber  m_LineNumber;
		};

		inline std::ostream &operator << (std::ostream &stream, const Location &location)
		{
			return location.operator << (stream);
		}


		class Assertion
		{
		public:
			constexpr Assertion() : m_IsValid(true), m_Location(), m_IsConditionEqual(false), m_Actual(nullptr), m_Expected(nullptr) {}

			constexpr Assertion(const char *actual, const char *expected, bool isConditionEqual, const Location &location) : m_IsValid(false), m_IsConditionEqual(isConditionEqual), m_Actual(actual), m_Expected(expected), m_Location(location) {}

			#ifdef DEBUG
			#define TEST_ASSERTION(predicate, actual, expected, equal) if (!(predicate)) { return __builtin_trap(), Ash::Test::Assertion(actual, expected, equal, Ash::Test::Location(__FILE__, __LINE__)); }
			#else
			#define TEST_ASSERTION(predicate, actual, expected, equal) if (!(predicate)) { return Ash::Test::Assertion(actual, expected, equal, Ash::Test::Location(__FILE__, __LINE__)); }
			#endif

			#define TEST_IS_TRUE(value) TEST_ASSERTION((value) == true, #value, "true", true)

			#define TEST_IS_FALSE(value) TEST_ASSERTION((value) == false, #value, "false", true)

			#define TEST_IS_NULL(value) TEST_ASSERTION((value) == nullptr, #value, "nullptr", true)

			#define TEST_IS_NOT_NULL(value) TEST_ASSERTION((value) != nullptr, #value, "nullptr", false)

			#define TEST_IS_ZERO(value) TEST_ASSERTION((value) == 0, #value, "zero", true)

			#define TEST_IS_NOT_ZERO(value) TEST_ASSERTION((value) != 0, #value, "zero", false)

			#define TEST_IS_EQ(actual, expected) TEST_ASSERTION((actual) == (expected), #actual, #expected, true)

			#define TEST_IS_NOT_EQ(actual, expected) TEST_ASSERTION((actual) != (expected), #actual, #expected, false)

			#define TEST(function) { Ash::Test::Assertion assertion = function(); if (!(assertion).isValid()) { return assertion; } }

			#define TEST_GENERIC(function, ...) { Ash::Test::Assertion assertion = function<__VA_ARGS__>(); if (!(assertion).isValid()) { return assertion; } }

			constexpr bool isValid() const { return m_IsValid; }

			constexpr bool isConditionEqual() const { return m_IsConditionEqual; }

			constexpr const char *getActual() const { return m_Actual; }

			constexpr const char *getExpected() const { return m_Expected; }

			constexpr Location getLocation() const { return m_Location; }

			std::ostream &operator << (std::ostream &stream) const
			{
				if (m_IsValid)
				{
					return stream << "PASS";
				}
				else
				{
					return stream << "FAIL: Expected '" << m_Actual << "' "
					              << (m_IsConditionEqual ? "to be equal to" : "to not be equal to")
									  << " '" << m_Expected << "' - see '" << m_Location << '\'';
				}
			}

		private:
			bool        m_IsValid;
			bool        m_IsConditionEqual;
			const char *m_Actual;
			const char *m_Expected;
			Location    m_Location;
		};

		inline std::ostream &operator << (std::ostream &stream, const Assertion &assertion)
		{
			return assertion.operator << (stream);
		}


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

			std::ostream &operator << (std::ostream &stream) const
			{
				stream << "Total tests: " << (m_PassCount + m_FailureCount) << '\n';
				stream << "Passed: " << m_PassCount << '\n';
				stream << "Failed: " << m_FailureCount << '\n';
				return stream;
			}

		protected:
			constexpr Result(Count passCount, Count failureCount) : m_PassCount(passCount), m_FailureCount(failureCount) {}

		private:
			Count m_PassCount;
			Count m_FailureCount;
		};

		inline std::ostream &operator << (std::ostream &stream, const Result &result)
		{
			return result.operator << (stream);
		}


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

#if STD >= 17
		#define TEST_UNIT(name, ...) constexpr Ash::Test::Unit name( [](Ash::Test::Capture capture) { return Ash::Test::Unit::run({ __VA_ARGS__ }, capture); } )
#else
		#define TEST_UNIT(name, ...) static Ash::Test::Unit name( [](Ash::Test::Capture capture) { return Ash::Test::Unit::run({ __VA_ARGS__ }, capture); } )
#endif
	}
}
