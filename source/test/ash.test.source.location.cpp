#include <cstring>
#include "ash.source.location.h"
#include "ash.test.source.location.h"


namespace Ash
{
	namespace Test
	{
		namespace Source
		{
			namespace Location
			{
				static Ash::Test::Assertion core()
				{
					Ash::Source::Location location;

					TEST_IS_FALSE(location.isAvailable());
					TEST_IS_NULL(location.getFunctionName());
					TEST_IS_NULL(location.getFileName());
					TEST_IS_ZERO(location.getLineNumber());

					return {};
				}

				static Ash::Test::Assertion inFile()
				{
					Ash::Source::Location location = Ash::Source::Location::inFile();

					TEST_IS_TRUE(location.isAvailable());
					TEST_IS_NULL(location.getFunctionName());
					TEST_IS_ZERO(strcmp(location.getFileName(), "source/test/ash.test.source.location.cpp"));
					TEST_IS_EQ(location.getLineNumber(), 28);

					return {};
				}

				static Ash::Test::Assertion inFunction()
				{
					Ash::Source::Location location = Ash::Source::Location::inFunction();

					TEST_IS_TRUE(location.isAvailable());
					TEST_IS_ZERO(strcmp(location.getFunctionName(), "inFunction"));
					TEST_IS_ZERO(strcmp(location.getFileName(), "source/test/ash.test.source.location.cpp"));
					TEST_IS_EQ(location.getLineNumber(), 40);

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testSourceLocation,
			TEST_CASE(Ash::Test::Source::Location::core),
			TEST_CASE(Ash::Test::Source::Location::inFile),
			TEST_CASE(Ash::Test::Source::Location::inFunction),
		);
	}
}
