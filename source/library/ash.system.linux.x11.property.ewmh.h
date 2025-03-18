#pragma once

#include "ash.system.linux.x11.property.h"
#include "ash.system.linux.x11.atom.registry.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Property
				{
					// Ref: Extended Window Manager Hints - see https://specifications.freedesktop.org/wm-spec/1.4/index.html#id-1.2)
					namespace EWMH
					{
						using NumberOfDesktops = Ash::System::Linux::X11::Property::Cardinal<Ash::System::Linux::X11::Atom::EWMH::NumberOfDesktops>;

						using DesktopGeometry = Ash::System::Linux::X11::Property::Size<Ash::System::Linux::X11::Atom::EWMH::DesktopGeometry>;

						using DesktopViewport = Ash::System::Linux::X11::Property::PositionArray<Ash::System::Linux::X11::Atom::EWMH::DesktopViewport>;

						using CurrentDesktop = Ash::System::Linux::X11::Property::Cardinal<Ash::System::Linux::X11::Atom::EWMH::CurrentDesktop>;

						using ActiveWindow = Ash::System::Linux::X11::Property::Window<Ash::System::Linux::X11::Atom::EWMH::ActiveWindow>;

						using WorkArea = Ash::System::Linux::X11::Property::BoundaryArray<Ash::System::Linux::X11::Atom::EWMH::WorkArea>;

						using Name = Ash::System::Linux::X11::Property::Utf8String<Ash::System::Linux::X11::Atom::EWMH::Name>;

						using VisibleName = Ash::System::Linux::X11::Property::Utf8String<Ash::System::Linux::X11::Atom::EWMH::VisibleName>;

						using IconName = Ash::System::Linux::X11::Property::Utf8String<Ash::System::Linux::X11::Atom::EWMH::IconName>;

						using VisibleIconName = Ash::System::Linux::X11::Property::Utf8String<Ash::System::Linux::X11::Atom::EWMH::VisibleIconName>;

						using Desktop = Ash::System::Linux::X11::Property::Cardinal<Ash::System::Linux::X11::Atom::EWMH::Desktop>;

						using WindowType = Ash::System::Linux::X11::Property::AtomArray<Ash::System::Linux::X11::Atom::EWMH::WindowType>;

						using State = Ash::System::Linux::X11::Property::AtomArray<Ash::System::Linux::X11::Atom::EWMH::State>;

						using AllowedActions = Ash::System::Linux::X11::Property::AtomArray<Ash::System::Linux::X11::Atom::EWMH::AllowedActions>;

						using Strut = Ash::System::Linux::X11::Property::Frame<Ash::System::Linux::X11::Atom::EWMH::Strut>;

						using StrutPartial = Ash::System::Linux::X11::Property::FrameStruts<Ash::System::Linux::X11::Atom::EWMH::StrutPartial>;

						using IconGeometry = Ash::System::Linux::X11::Property::Boundary<Ash::System::Linux::X11::Atom::EWMH::IconGeometry>;

						using ProcessId = Ash::System::Linux::X11::Property::Cardinal<Ash::System::Linux::X11::Atom::EWMH::ProcessId>;

						using UserTime = Ash::System::Linux::X11::Property::Cardinal<Ash::System::Linux::X11::Atom::EWMH::UserTime>;

						using UserTimeWindow = Ash::System::Linux::X11::Property::Window<Ash::System::Linux::X11::Atom::EWMH::UserTimeWindow>;

						using FrameExtents = Ash::System::Linux::X11::Property::Frame<Ash::System::Linux::X11::Atom::EWMH::FrameExtents>;
					}
				}
			}
		}
	}
}
