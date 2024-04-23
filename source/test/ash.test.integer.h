#pragma once

#include "ash.test.h"
#include "ash.test.integer.value.h"
#include "ash.test.integer.bit.h"
#include "ash.test.integer.cycle.h"
#include "ash.test.integer.math.h"


namespace Ash
{
	namespace Test
	{
		constexpr TEST_UNIT
		(
			testInteger,
			Ash::Test::testIntegerValue,
			Ash::Test::testIntegerBit,
			Ash::Test::testIntegerCycle,
			Ash::Test::testIntegerMath
		);
	}
}
