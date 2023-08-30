#pragma once

#include "ash.test.calendar.date.h"
#include "ash.test.calendar.time.h"
#include "ash.test.calendar.datetime.h"


namespace Ash
{
	namespace Test
	{
		constexpr TEST_UNIT
		(
			testCalendar,
			Ash::Test::testDate,
			Ash::Test::testTime,
			Ash::Test::testDateTime
		);
	}
}
