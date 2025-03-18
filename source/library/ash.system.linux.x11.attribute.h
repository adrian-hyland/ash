#pragma once

#include <xcb/xcb.h>
#include "ash.system.linux.x11.set.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Attribute
				{
					namespace Window
					{
						using BackPixmap = Ash::System::Linux::X11::Set<XCB_CW_BACK_PIXMAP>;

						using BackPixel = Ash::System::Linux::X11::Set<XCB_CW_BACK_PIXEL>;

						using BorderPixmap = Ash::System::Linux::X11::Set<XCB_CW_BORDER_PIXMAP>;

						using BorderPixel = Ash::System::Linux::X11::Set<XCB_CW_BORDER_PIXEL>;

						using BitGravity = Ash::System::Linux::X11::Set<XCB_CW_BIT_GRAVITY>;

						using WindowGravity = Ash::System::Linux::X11::Set<XCB_CW_WIN_GRAVITY>;

						using BackingStore = Ash::System::Linux::X11::Set<XCB_CW_BACKING_STORE>;

						using BackingPlanes = Ash::System::Linux::X11::Set<XCB_CW_BACKING_PLANES>;

						using BackingPixel = Ash::System::Linux::X11::Set<XCB_CW_BACKING_PIXEL>;

						using OverrideRedirect = Ash::System::Linux::X11::Set<XCB_CW_OVERRIDE_REDIRECT>;

						using SaveUnder = Ash::System::Linux::X11::Set<XCB_CW_SAVE_UNDER>;

						using EventMask = Ash::System::Linux::X11::Set<XCB_CW_EVENT_MASK>;

						using DoNotPropogate = Ash::System::Linux::X11::Set<XCB_CW_DONT_PROPAGATE>;

						using Colormap = Ash::System::Linux::X11::Set<XCB_CW_COLORMAP>;

						using Cursor = Ash::System::Linux::X11::Set<XCB_CW_CURSOR>;
					}
				}
			}
		}
	}
}
