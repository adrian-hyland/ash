#pragma once

#include <windows.h>
#include "ash.wide.h"
#include "ash.ui.core.h"
#include "ash.system.windows.library.shcore.h"
#include "ash.system.windows.ui.metrics.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace UI
			{
				class Screen
				{
				public:
					class Display
					{
					public:
						using Name = Ash::Wide::Buffer<32>;

						constexpr Display() : m_Name(), m_Boundary() {}

						constexpr Display(Ash::Wide::View name, Ash::UI::Boundary boundary) : m_Name(name), m_Boundary(boundary) {}

						constexpr Ash::Wide::View getName() const { return m_Name; }

						constexpr Ash::UI::Boundary getBoundary() const { return m_Boundary; }

						constexpr Ash::UI::AspectRatio getAspectRatio() const { return m_Boundary.size; }

					private:
						Name              m_Name;
						Ash::UI::Boundary m_Boundary;
					};

					static inline Ash::UI::Boundary getBoundary() { return m_Instance.getBoundary(); }

					static Display getPrimaryDisplay() { return *m_Instance.getDisplays().at(0); }

					static Ash::Memory::View<Display> getDisplays() { return m_Instance.getDisplays(); }

					static bool onScreenChange(Ash::UI::Boundary boundary)
					{
						return m_Instance.screenChange(boundary);
					}

				protected:
					using DisplayList = Ash::Memory::ArrayBuffer<Display, 4>;

					class Instance
					{
					public:
						inline Instance() : m_Boundary(), m_DisplayList()
						{
							Ash::System::Windows::Library::Shcore::SetProcessDpiAwareness setProcessDpiAwareness = Ash::System::Windows::Library::Shcore::setProcessDpiAwareness();
							if (setProcessDpiAwareness != nullptr)
							{
								setProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
							}

							m_Boundary = Ash::System::Windows::UI::Metrics::getScreenBoundary();
							m_DisplayList = getDisplayList();
						}

						constexpr Ash::UI::Boundary getBoundary() const { return m_Boundary; }

						constexpr Ash::Memory::View<Display> getDisplays() const { return m_DisplayList; }

						inline bool screenChange(Ash::UI::Boundary boundary)
						{
							m_Boundary = boundary;
							m_DisplayList = getDisplayList();

							return true;
						}

					protected:
						static DisplayList getDisplayList()
						{
							DisplayList displayList;
							DISPLAY_DEVICEW adapterDevice = {};

							adapterDevice.cb = sizeof(adapterDevice);
							for (DWORD adapterNumber = 0; ::EnumDisplayDevicesW(nullptr, adapterNumber, &adapterDevice, 0); adapterNumber++)
							{
								DISPLAY_DEVICEW displayDevice = {};

								displayDevice.cb = sizeof(displayDevice);
								if (::EnumDisplayDevicesW(adapterDevice.DeviceName, 0, &displayDevice, 0))
								{
									Display display;
									bool isPrimary;
									if (getDisplay(displayDevice, display, isPrimary))
									{
										if (isPrimary)
										{
											displayList.insert(0, display);
										}
										else
										{
											displayList.append(display);
										}
									}
								}
							}

							return displayList;
						}

						static bool getDisplay(const DISPLAY_DEVICEW &displayDevice, Display &display, bool &isPrimary)
						{
							MONITORINFO monitorInfo;
							DEVMODEW deviceMode;
							POINT point = { 0, 0 };

							deviceMode.dmSize = sizeof(deviceMode);
							deviceMode.dmDriverExtra = 0;
							if (::EnumDisplaySettingsExW(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0) && (deviceMode.dmFields & DM_POSITION))
							{
								point.x = deviceMode.dmPosition.x;
								point.y = deviceMode.dmPosition.y;
							}

							monitorInfo.cbSize = sizeof(monitorInfo);
							if (!::GetMonitorInfo(::MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST), &monitorInfo))
							{
								return false;
							}

							display = Display(Ash::Wide::View(displayDevice.DeviceName), getBoundary(monitorInfo.rcMonitor));
							isPrimary = ((monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0);
							return true;
						}

						static Ash::UI::Boundary getBoundary(RECT rect)
						{
							return { { rect.left, rect.top }, { Ash::UI::Size::Dimension(rect.right - rect.left), Ash::UI::Size::Dimension(rect.bottom - rect.top) } };
						}

					private:
						Ash::UI::Boundary m_Boundary;
						DisplayList       m_DisplayList;
					};

				private:
					static inline Instance m_Instance = Instance();

					Screen() = delete;
				};
			}
		}
	}
}
