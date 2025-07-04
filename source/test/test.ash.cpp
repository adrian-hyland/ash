#include <iostream>
#include "ash.test.test.h"
#include "ash.test.compiler.h"
#include "ash.test.type.h"
#include "ash.test.callable.h"
#include "ash.test.size.h"
#include "ash.test.iterate.h"
#include "ash.test.integer.h"
#include "ash.test.real.h"
#include "ash.test.statistic.h"
#include "ash.test.geometry.h"
#include "ash.test.calendar.h"
#include "ash.test.unicode.h"
#include "ash.test.memory.h"
#include "ash.test.pointer.h"
#include "ash.test.nullable.h"
#include "ash.test.encoding.h"
#include "ash.test.string.h"
#include "ash.test.filesystem.h"


int main()
{
	TEST_UNIT
	(
		testSuite,

		Ash::Test::testTest,
		Ash::Test::testCompiler,
		Ash::Test::testType,
		Ash::Test::testCallable,
		Ash::Test::testSize,
		Ash::Test::testIterate,
		Ash::Test::testInteger,
		Ash::Test::testReal,
		Ash::Test::testStatistic,
		Ash::Test::testGeometry,
		Ash::Test::testCalendar,
		Ash::Test::testUnicode,
		Ash::Test::testMemory,
		Ash::Test::testPointer,
		Ash::Test::testNullable,
		Ash::Test::testEncoding,
		Ash::Test::testString,
		Ash::Test::testFileSystem
	);

	Ash::Test::Result result = Ash::Test::Unit::run({ testSuite });

	std::cout << result;

	return 0;
}
