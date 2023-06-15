#include <iostream>
#include "ash.test.test.h"
#include "ash.test.size.h"
#include "ash.test.integer.h"
#include "ash.test.calendar.h"


int main()
{
	TEST_UNIT
	(
		testSuite,
		Ash::Test::testTest,
		Ash::Test::testSize,
		Ash::Test::testInteger,
		Ash::Test::testDate,
	);

	Ash::Test::Result result = Ash::Test::Unit::run({ testSuite });

	std::cout << result;

	return 0;
}
