#pragma once

#include "ash.test.memory.core.h"
#include "ash.test.memory.allocation.h"


namespace Ash
{
	namespace Test
	{
		constexpr TEST_UNIT
		(
			testMemory,
			Ash::Test::testMemoryCore,
			Ash::Test::testMemoryAllocation,
		);
	}
}
