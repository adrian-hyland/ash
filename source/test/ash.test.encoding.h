#include "ash.test.encoding.ascii.h"
#include "ash.test.encoding.iso8859.h"
#include "ash.test.encoding.wide.h"
#include "ash.test.encoding.utf8.h"
#include "ash.test.encoding.utf16be.h"
#include "ash.test.encoding.utf16le.h"
#include "ash.test.encoding.utf32be.h"
#include "ash.test.encoding.utf32le.h"
#include "ash.test.encoding.core.h"


namespace Ash
{
	namespace Test
	{
		constexpr TEST_UNIT
		(
			testEncoding,

			Ash::Test::testAscii,
			Ash::Test::testIso8859,
			Ash::Test::testWide,
			Ash::Test::testUtf8,
			Ash::Test::testUtf16be,
			Ash::Test::testUtf16le,
			Ash::Test::testUtf32be,
			Ash::Test::testUtf32le,
			Ash::Test::testEncodingCore
		);
	}
}
