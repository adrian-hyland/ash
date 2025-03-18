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
					namespace EWMH
					{
						inline Ash::System::Linux::X11::Event::ClientMessage requestFrameExtents(xcb_window_t window)
						{
							if (Ash::System::Linux::X11::Atom::EWMH::RequestFrameExtents::value == XCB_ATOM_NONE)
							{
								return Ash::System::Linux::X11::Event::ClientMessage();
							}

							return Ash::System::Linux::X11::Event::newClientMessage(window, Ash::System::Linux::X11::Atom::EWMH::RequestFrameExtents::value, {});
						}

						inline Ash::System::Linux::X11::Event::ClientMessage setDesktop(xcb_window_t window, uint32_t newDesktop, uint32_t sourceIndication)
						{
							if (Ash::System::Linux::X11::Atom::EWMH::Desktop::value == XCB_ATOM_NONE)
							{
								return Ash::System::Linux::X11::Event::ClientMessage();
							}

							return Ash::System::Linux::X11::Event::newClientMessage(window, Ash::System::Linux::X11::Atom::EWMH::Desktop::value, { newDesktop, sourceIndication });
						}

						inline Ash::System::Linux::X11::Event::ClientMessage removeState(xcb_window_t window, xcb_atom_t state1, xcb_atom_t state2, uint32_t sourceIndication)
						{
							if ((Ash::System::Linux::X11::Atom::EWMH::State::value == XCB_ATOM_NONE) || (state1 == XCB_ATOM_NONE))
							{
								return Ash::System::Linux::X11::Event::ClientMessage();
							}

							return Ash::System::Linux::X11::Event::newClientMessage(window, Ash::System::Linux::X11::Atom::EWMH::State::value, { 0, state1, state2, sourceIndication });
						}

						inline Ash::System::Linux::X11::Event::ClientMessage addState(xcb_window_t window, xcb_atom_t state1, xcb_atom_t state2, uint32_t sourceIndication)
						{
							if ((Ash::System::Linux::X11::Atom::EWMH::State::value == XCB_ATOM_NONE) || (state1 == XCB_ATOM_NONE))
							{
								return Ash::System::Linux::X11::Event::ClientMessage();
							}

							return Ash::System::Linux::X11::Event::newClientMessage(window, Ash::System::Linux::X11::Atom::EWMH::State::value, { 1, state1, state2, sourceIndication });
						}

						inline Ash::System::Linux::X11::Event::ClientMessage toggleState(xcb_window_t window, xcb_atom_t state1, xcb_atom_t state2, uint32_t sourceIndication)
						{
							if ((Ash::System::Linux::X11::Atom::EWMH::State::value == XCB_ATOM_NONE) || (state1 == XCB_ATOM_NONE))
							{
								return Ash::System::Linux::X11::Event::ClientMessage();
							}

							return Ash::System::Linux::X11::Event::newClientMessage(window, Ash::System::Linux::X11::Atom::EWMH::State::value, { 2, state1, state2, sourceIndication });
						}
					}
				}
			}
		}
	}
}
