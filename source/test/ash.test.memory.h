#pragma once

#include "ash.test.memory.core.h"
#include "ash.test.memory.allocation.h"
#include "ash.test.memory.unique.h"


namespace Ash
{
	namespace Test
	{
		constexpr TEST_UNIT
		(
			testMemory,
			Ash::Test::testMemoryCore,
			Ash::Test::testMemoryAllocation,
			Ash::Test::testMemoryUnique
		);
	}
}
