#pragma once

#include <xcb/xcb.h>
#include "ash.system.linux.x11.set.h"
#include "ash.ui.core.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Configuration
				{
					namespace Window
					{
						class Position : public Ash::System::Linux::X11::Set<XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y>
						{
						public:
							using Set = Ash::System::Linux::X11::Set<XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y>;

							constexpr Position(Ash::UI::Position position) : Set({ static_cast<uint32_t>(position.x), static_cast<uint32_t>(position.y) }) {}
						};

						class Size : public Ash::System::Linux::X11::Set<XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT>
						{
						public:
							using Set = Ash::System::Linux::X11::Set<XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT>;

							constexpr Size(Ash::UI::Size size) : Set({ size.width, size.height }) {}
						};

						class Boundary : public Ash::System::Linux::X11::Set<XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT>
						{
						public:
							using Set = Ash::System::Linux::X11::Set<XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT>;

							constexpr Boundary(Ash::UI::Boundary boundary) : Set({ static_cast<uint32_t>(boundary.position.x), static_cast<uint32_t>(boundary.position.y), boundary.size.width, boundary.size.height }) {}
						};
					}
				}
			}
		}
	}
}
