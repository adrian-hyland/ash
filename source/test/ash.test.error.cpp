#include <limits>
#include <cstring>
#include "ash.error.h"
#include "ash.test.error.h"


namespace Ash
{
	namespace Test
	{
		namespace Error
		{
			namespace Code
			{
				constexpr int32_t  min = std::numeric_limits<int32_t>::min();
				constexpr uint32_t max = std::numeric_limits<uint32_t>::max();

				static Ash::Test::Assertion value()
				{
					Ash::Error::Code code;
					TEST_IS_ZERO(code);

					code = -1;
					TEST_IS_EQ(code, -1);
					TEST_IS_NOT_EQ(code, 1);

					code = 1;
					TEST_IS_EQ(code, 1);
					TEST_IS_NOT_EQ(code, -1);

					code = max;
					TEST_IS_EQ(code, max);
					TEST_IS_NOT_EQ(code, min);

					code = min;
					TEST_IS_EQ(code, min);
					TEST_IS_NOT_EQ(code, max);

					return {};
				}

				static Ash::Test::Assertion string()
				{
					Ash::Error::Code::String codeString;

					Ash::Error::Code(0).getString(codeString);
					TEST_IS_ZERO(strcmp(codeString, "0"));

					Ash::Error::Code(-1).getString(codeString);
					TEST_IS_ZERO(strcmp(codeString, "-1"));

					Ash::Error::Code(1).getString(codeString);
					TEST_IS_ZERO(strcmp(codeString, "1"));

					Ash::Error::Code(max).getString(codeString);
					TEST_IS_ZERO(strcmp(codeString, "4294967295"));

					Ash::Error::Code(min).getString(codeString);
					TEST_IS_ZERO(strcmp(codeString, "-2147483648"));

					return {};
				}
			}

			namespace Category
			{
				class General : public Ash::Error::Category
				{
				public:
					virtual Name getName() const override
					{
						return "Ash::Test::Error::Category::General";
					}

					virtual bool getErrorCodeDescription(Ash::Error::Code code, Ash::Error::Code::Description description) const override
					{
						memset(description, 0, sizeof(Ash::Error::Code::Description));

						if (code == -1)
						{
							strncpy(description, "warning", sizeof(Ash::Error::Code::Description) - 1);
							return true;
						}

						if (code == 1)
						{
							strncpy(description, "critical", sizeof(Ash::Error::Code::Description) - 1);
							return true;
						}

						return false;
					}
				};

				constexpr General general;

				static Ash::Test::Assertion getName()
				{
					TEST_IS_ZERO(strcmp(general.getName(), "Ash::Test::Error::Category::General"));

					return {};
				}

				static Ash::Test::Assertion getErrorCodeDescription()
				{
					Ash::Error::Code::Description description;

					TEST_IS_FALSE(general.getErrorCodeDescription(0, description));

					TEST_IS_TRUE(general.getErrorCodeDescription(-1, description));
					TEST_IS_ZERO(strcmp(description, "warning"));

					TEST_IS_TRUE(general.getErrorCodeDescription(1, description));
					TEST_IS_ZERO(strcmp(description, "critical"));

					return {};
				}
			}

			namespace Exception
			{
				static Ash::Test::Assertion core()
				{
					Ash::Error::Exception exception(&Ash::Test::Error::Category::general, 1, Ash::Source::Location::inFunction());
					Ash::Error::Code::Description description;
					Ash::Source::Location location;

					TEST_IS_EQ(exception.getCategory(), &Ash::Test::Error::Category::general);
					TEST_IS_EQ(exception.getCode(), 1);
					TEST_IS_TRUE(exception.getDescription(description));
					TEST_IS_ZERO(strcmp(description, "critical"));
					location = exception.getSourceLocation();
					TEST_IS_TRUE(location.isAvailable());
					TEST_IS_ZERO(strcmp(location.getFunctionName(), "core"));
					TEST_IS_ZERO(strcmp(location.getFileName(), "source/test/ash.test.error.cpp"));
					TEST_IS_EQ(location.getLineNumber(), 124);

					return {};
				}
			}

			namespace Value
			{
				constexpr Ash::Error::Value warning  = Ash::Error::Value(Ash::Test::Error::Category::general, -1);
				constexpr Ash::Error::Value critical = Ash::Error::Value(Ash::Test::Error::Category::general, 1);

				static Ash::Test::Assertion code()
				{
					TEST_IS_EQ((int)critical, 1);
					TEST_IS_EQ((int)warning, -1);

					return {};
				}

				static Ash::Test::Assertion equals()
				{
					TEST_IS_EQ(Ash::Error::none, Ash::Error::none);
					TEST_IS_EQ(critical, critical);
					TEST_IS_EQ(warning, warning);

					TEST_IS_NOT_EQ(Ash::Error::none, critical);
					TEST_IS_NOT_EQ(critical, Ash::Error::none);
					TEST_IS_NOT_EQ(Ash::Error::none, warning);
					TEST_IS_NOT_EQ(warning, Ash::Error::none);
					TEST_IS_NOT_EQ(critical, warning);
					TEST_IS_NOT_EQ(warning, critical);

					return {};
				}

				static Ash::Test::Assertion getCategory()
				{
					TEST_IS_NULL(Ash::Error::none.getCategory());

					TEST_IS_EQ(critical.getCategory(), &Ash::Test::Error::Category::general);

					return {};
				}

				static Ash::Test::Assertion getCode()
				{
					TEST_IS_ZERO(Ash::Error::none.getCode());

					TEST_IS_EQ(warning.getCode(), -1);

					TEST_IS_EQ(critical.getCode(), 1);

					return {};
				}

				static Ash::Test::Assertion getDescription()
				{
					Ash::Error::Code::Description description;

					TEST_IS_FALSE(Ash::Error::none.getDescription(description));

					TEST_IS_TRUE(warning.getDescription(description));
					TEST_IS_ZERO(strcmp(description, "warning"));

					TEST_IS_TRUE(critical.getDescription(description));
					TEST_IS_ZERO(strcmp(description, "critical"));

					return {};
				}

				static Ash::Test::Assertion hasErrorSet()
				{
					TEST_IS_FALSE(Ash::Error::none.hasErrorSet());

					TEST_IS_TRUE(critical.hasErrorSet());

					return {};
				}

				static Ash::Test::Assertion ignoreError()
				{
					Ash::Error::Value error = warning;
					TEST_IS_EQ(error.ignoreError(warning), Ash::Error::none);

					error = critical;
					TEST_IS_EQ(error.ignoreError(warning), critical);

					return {};
				}

				static Ash::Test::Assertion throwOnError()
				{
					try
					{
						Ash::Error::none.throwOnError();
					}
					catch (...)
					{
						TEST_FAIL("no thrown error");
					}

					try
					{
						critical.throwOnError();

						TEST_FAIL("thrown error");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(exception.getCategory(), critical.getCategory());
						TEST_IS_EQ(exception.getCode(), critical.getCode());
					}

					try
					{
						Ash::Error::Value error = warning;
						TEST_IS_EQ(error.throwOnError(critical), warning);
					}
					catch (...)
					{
						TEST_FAIL("no thrown error");
					}

					try
					{
						Ash::Error::Value error = critical;
						TEST_IS_NOT_EQ(error.throwOnError(critical), critical);
						TEST_FAIL("thrown error");
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(exception.getCategory(), critical.getCategory());
						TEST_IS_EQ(exception.getCode(), critical.getCode());
					}

					return {};
				}

				static Ash::Test::Assertion terminateOnError()
				{
					Ash::Error::Value error = warning;
					TEST_IS_EQ(error.terminateOnError(critical), warning);

					return {};
				}

				static Ash::Test::Assertion callChain()
				{
					Ash::Error::Value error = warning;
					TEST_IS_EQ(error.ignoreError(warning).terminateOnError(critical), Ash::Error::none);
					TEST_IS_EQ(error.ignoreError(warning).throwOnError(critical), Ash::Error::none);
					try
					{
						TEST_IS_EQ(error.throwOnError(warning).terminateOnError(critical), Ash::Error::none);
					}
					catch (const Ash::Error::Exception &exception)
					{
						TEST_IS_EQ(exception.getCategory(), warning.getCategory());
						TEST_IS_EQ(exception.getCode(), warning.getCode());
					}

					return {};
				}
			}

			static Ash::Test::Assertion throwUsing()
			{
				try
				{
					Ash::Error::throwUsing(Ash::Test::Error::Value::critical);

					TEST_FAIL("thrown error");
				}
				catch (const Ash::Error::Exception &exception)
				{
					TEST_IS_EQ(exception.getCategory(), Ash::Test::Error::Value::critical.getCategory());
					TEST_IS_EQ(exception.getCode(), Ash::Test::Error::Value::critical.getCode());
				}

				return {};
			}
		}

		TEST_UNIT
		(
			testError,

			TEST_CASE(Ash::Test::Error::Code::value),
			TEST_CASE(Ash::Test::Error::Code::string),

			TEST_CASE(Ash::Test::Error::Category::getName),
			TEST_CASE(Ash::Test::Error::Category::getErrorCodeDescription),

			TEST_CASE(Ash::Test::Error::Exception::core),

			TEST_CASE(Ash::Test::Error::Value::code),
			TEST_CASE(Ash::Test::Error::Value::equals),
			TEST_CASE(Ash::Test::Error::Value::getCategory),
			TEST_CASE(Ash::Test::Error::Value::getCode),
			TEST_CASE(Ash::Test::Error::Value::getDescription),
			TEST_CASE(Ash::Test::Error::Value::hasErrorSet),
			TEST_CASE(Ash::Test::Error::Value::ignoreError),
			TEST_CASE(Ash::Test::Error::Value::throwOnError),
			TEST_CASE(Ash::Test::Error::Value::terminateOnError),
			TEST_CASE(Ash::Test::Error::Value::callChain),

			TEST_CASE(Ash::Test::Error::throwUsing)
		);
	}
}
