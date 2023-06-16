#pragma once

#include "ash.test.calendar.date.h"
#include "ash.test.calendar.time.h"


namespace Ash
{
	namespace Test
	{
		TEST_UNIT
		(
			testCalendar,
			Ash::Test::testDate,
			Ash::Test::testTime
		);
	}
}
