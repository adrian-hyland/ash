#pragma once

#include <windows.h>
#include "ash.wide.h"
#include "ash.ui.core.h"
#include "ash.system.windows.ui.error.h"
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

					static Ash::UI::Boundary getBoundary() { return m_Instance.getBoundary(); }

					static Display getPrimaryDisplay() { return *m_Instance.getDisplays().at(0); }

					static Ash::Memory::View<Display> getDisplays() { return m_Instance.getDisplays(); }

					[[nodiscard]]
					static Ash::Error::Value onScreenChange(Ash::UI::Boundary screenBoundary)
					{
						return m_Instance.onScreenChange(screenBoundary);
					}

				protected:
					using DisplayList = Ash::Memory::ArrayBuffer<Display, 4>;

					class Instance
					{
					public:
						Instance() : m_Boundary(), m_DisplayList()
						{
							Ash::System::Windows::Library::Shcore::SetProcessDpiAwareness setProcessDpiAwareness = Ash::System::Windows::Library::Shcore::setProcessDpiAwareness();
							if (setProcessDpiAwareness != nullptr)
							{
								setProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
							}

							m_Boundary = Ash::System::Windows::UI::Metrics::getScreenBoundary();
							m_DisplayList = getDisplayList();
						}

						Ash::UI::Boundary getBoundary() const { return m_Boundary; }

						Ash::Memory::View<Display> getDisplays() const { return m_DisplayList; }

						[[nodiscard]]
						Ash::Error::Value onScreenChange(Ash::UI::Boundary boundary)
						{
							Ash::Error::Value error = getDisplayList(m_DisplayList);
							if (!Ash::Error::isSet(error))
							{
								m_Boundary = boundary;
							}

							return error;
						}

					protected:
						[[nodiscard]]
						static Ash::Error::Value getDisplayList(DisplayList &displayList)
						{
							DisplayList list;
							DISPLAY_DEVICEW adapterDevice = {};

							adapterDevice.cb = sizeof(adapterDevice);
							for (DWORD adapterNumber = 0; ::EnumDisplayDevicesW(nullptr, adapterNumber, &adapterDevice, 0); adapterNumber++)
							{
								DISPLAY_DEVICEW displayDevice = {};

								displayDevice.cb = sizeof(displayDevice);
								for (DWORD displayNumber = 0; ::EnumDisplayDevicesW(adapterDevice.DeviceName, displayNumber, &displayDevice, 0); displayNumber++)
								{
									Display display;
									bool isPrimary;

									Ash::Error::Value error = getDisplay(displayDevice, display, isPrimary);
									if (error)
									{
										return error;
									}

									error = isPrimary ? list.insert(0, display) : list.append(display);
									if (error)
									{
										return error;
									}
								}
							}

							displayList.moveFrom(list);

							return Ash::Error::none;
						}

						static DisplayList getDisplayList()
						{
							DisplayList displayList;

							getDisplayList(displayList).throwOnError();

							return displayList;
						}

						[[nodiscard]]
						static Ash::Error::Value getDisplay(const DISPLAY_DEVICEW &displayDevice, Display &display, bool &isPrimary)
						{
							MONITORINFO monitorInfo = {};
							DEVMODEW deviceMode = {};
							POINT point = { 0, 0 };

							deviceMode.dmSize = sizeof(deviceMode);
							deviceMode.dmDriverExtra = 0;
							if (!::EnumDisplaySettingsExW(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0) && (deviceMode.dmFields & DM_POSITION))
							{
								return Ash::System::Windows::UI::Error::noDisplaySettings;
							}

							point.x = deviceMode.dmPosition.x;
							point.y = deviceMode.dmPosition.y;

							monitorInfo.cbSize = sizeof(monitorInfo);
							if (!::GetMonitorInfo(::MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST), &monitorInfo))
							{
								return Ash::System::Windows::UI::Error::noMonitorSettings;
							}

							display = Display(Ash::Wide::View(displayDevice.DeviceName), getBoundary(monitorInfo.rcMonitor));
							isPrimary = ((monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0);
							return Ash::Error::none;
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
