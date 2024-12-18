#include <cstdint>
#include <limits>
#include "ash.test.compiler.h"


namespace Ash
{
	namespace Test
	{
		namespace Compiler
		{
			// This test case is used to test the tree-vrp optimization that is available in GCC compilers.
			// Tree VRP optimization is enabled by the '-ftree-vrp' compiler flag. This flag is included in optimization
			// level 'O2' (or equivalent or above). The optimization can be turned off by using the '-fno-tree-vrp'
			// compiler flag.
			// This optimization is intended to track the range of values that a variable can have and then use this
			// information to remove unnecessary or unreachable code. It can also be used for detecting buffer overruns.
			// Unfortunately, this optimization can fail under certain edge cases as demonstrated by the test code given
			// below. This test fails when using GCC version 11, 12 or 13 with the '-ftree-vrp' optimization flag enabled.
			// However, it does look to have been resolved in GCC version 14...
			// Further information on tree VRP optimization can be found here:
			//   https://developers.redhat.com/blog/2021/04/28/value-range-propagation-in-gcc-with-project-ranger#
			template
			<
				typename TYPE
			>
			Ash::Test::Assertion valueRangePropogation()
			{
				TYPE previous = std::numeric_limits<TYPE>::max();

				for (TYPE n = std::numeric_limits<TYPE>::min(); n <= std::numeric_limits<TYPE>::min() + 1; n++)
				{
					previous = n;
				}

				TEST_IS_EQ(previous, std::numeric_limits<TYPE>::min() + 1);

				return {};
			}
		}

		TEST_UNIT
		(
			testCompiler,
			TEST_CASE_GENERIC(Ash::Test::Compiler::valueRangePropogation, int),
			TEST_CASE_GENERIC(Ash::Test::Compiler::valueRangePropogation, int8_t),
			TEST_CASE_GENERIC(Ash::Test::Compiler::valueRangePropogation, int16_t),
			TEST_CASE_GENERIC(Ash::Test::Compiler::valueRangePropogation, int32_t),
			TEST_CASE_GENERIC(Ash::Test::Compiler::valueRangePropogation, int64_t),
		);
	}
}
