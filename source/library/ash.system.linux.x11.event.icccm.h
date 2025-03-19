#pragma once

#include "ash.system.linux.x11.atom.registry.h"
#include "ash.system.linux.x11.event.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Event
				{
					namespace ICCCM
					{
						inline Ash::System::Linux::X11::Event::ClientMessage deleteWindow(xcb_window_t window, xcb_timestamp_t timestamp = XCB_CURRENT_TIME)
						{
							if ((Ash::System::Linux::X11::Atom::ICCCM::Protocols::value == XCB_ATOM_NONE) || (Ash::System::Linux::X11::Atom::ICCCM::DeleteWindow::value == XCB_ATOM_NONE))
							{
								return Ash::System::Linux::X11::Event::ClientMessage();
							}

							return Ash::System::Linux::X11::Event::newClientMessage(window, Ash::System::Linux::X11::Atom::ICCCM::Protocols::value, { Ash::System::Linux::X11::Atom::ICCCM::DeleteWindow::value, timestamp });
						}
					}
				}
			}
		}
	}
}
