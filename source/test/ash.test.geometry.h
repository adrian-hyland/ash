#pragma once

#include "ash.test.geometry.angle.h"
#include "ash.test.geometry.coordinate2d.h"
#include "ash.test.geometry.vector2d.h"
#include "ash.test.geometry.point2d.h"
#include "ash.test.geometry.line2d.h"


namespace Ash
{
	namespace Test
	{
		constexpr TEST_UNIT
		(
			testGeometry,
			Ash::Test::testAngle,
			Ash::Test::testCoordinate2D,
			Ash::Test::testVector2D,
			Ash::Test::testPoint2D,
			Ash::Test::testLine2D
		);
	}
}
