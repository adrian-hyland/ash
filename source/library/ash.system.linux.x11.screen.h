#pragma once

#include "ash.utf8.h"
#include "ash.system.linux.x11.message.h"
#include "ash.system.linux.x11.randr.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				class Screen
				{
				public:
					class Display
					{
					public:
						using Name = Ash::System::Linux::X11::Atom::Name;

						constexpr Display() : m_Name(), m_Boundary() {}

						constexpr Display(Ash::Utf8::View name, Ash::UI::Boundary boundary) : m_Name(name), m_Boundary(boundary) {}

						constexpr Ash::Utf8::View getName() const { return m_Name; }

						constexpr Ash::UI::Boundary getBoundary() const { return m_Boundary; }

						constexpr Ash::UI::AspectRatio getAspectRatio() const { return m_Boundary.size; }

					private:
						Name              m_Name;
						Ash::UI::Boundary m_Boundary;
					};

					static inline Ash::UI::Boundary getBoundary() { return m_Boundary; }

					static const Display &getPrimaryDisplay() { return *m_DisplayList.at(0); }

					static Ash::Memory::View<Display> getDisplays() { return m_DisplayList; }

					static bool onScreenChange(Ash::UI::Boundary screenBoundary)
					{
						m_Boundary = screenBoundary;
						m_DisplayList = getDisplayList();

						return true;
					}

				protected:
					using DisplayList = Ash::Memory::ArrayBuffer<Display, 4>;

					static DisplayList getDisplayList()
					{
						DisplayList displayList;

						Ash::System::Linux::X11::RandR::GetMonitors::Reply getMonitors = Ash::System::Linux::X11::RandR::GetMonitors(Ash::System::Linux::X11::Connection::getRootWindow(), true).getReply();
						for (xcb_randr_monitor_info_t monitor : Ash::System::Linux::X11::RandR::GetMonitors::iterate(getMonitors))
						{
							Ash::System::Linux::X11::Screen::Display::Name name;
							if (Ash::System::Linux::X11::GetAtomName(monitor.name).getReply().getValue(name))
							{
								Ash::UI::Boundary boundary = Ash::UI::Boundary({ monitor.x, monitor.y }, { monitor.width, monitor.height });
								displayList.append(Ash::System::Linux::X11::Screen::Display(name, boundary));
							}
						}

						return displayList;
					}

				private:
					static inline Ash::UI::Boundary m_Boundary    = Ash::UI::Boundary({ 0, 0 }, { Ash::System::Linux::X11::Connection::getScreen()->width_in_pixels, Ash::System::Linux::X11::Connection::getScreen()->height_in_pixels });
					static inline DisplayList       m_DisplayList = getDisplayList();

					Screen() = delete;
				};
			}
		}
	}
}
