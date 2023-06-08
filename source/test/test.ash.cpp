#include <iostream>
#include "ash.test.test.h"
#include "ash.test.size.h"


int main()
{
	const Ash::Test::Unit testSuite TEST_UNIT
	(
		Ash::Test::Test::test,
		Ash::Test::Size::test,
	);

	Ash::Test::Result result = Ash::Test::Unit::run({ testSuite });

	std::cout << result;

	return 0;
}
