#pragma once

#include "ash.ui.core.h"
#include "ash.system.windows.library.user32.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace UI
			{
				class Metrics
				{
				public:
					static inline Ash::UI::Boundary getScreenBoundary()
					{
						return Ash::UI::Boundary({ get(SM_XVIRTUALSCREEN), get(SM_YVIRTUALSCREEN) },
						                         { Ash::UI::Size::Dimension(get(SM_CXVIRTUALSCREEN)), Ash::UI::Size::Dimension(get(SM_CYVIRTUALSCREEN)) });
					}

					static inline Ash::UI::Boundary getDesktopBoundary()
					{
						RECT rect;
						if (!SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0))
						{
							return getScreenBoundary();
						}
						return Ash::UI::Boundary({ rect.left, rect.top }, { Ash::UI::Size::Dimension(rect.right - rect.left), Ash::UI::Size::Dimension(rect.bottom - rect.top) });
					}

				protected:
					static inline UINT getSystemDpi()
					{
						Ash::System::Windows::Library::User32::GetDpiForSystem getDpiForSystem = Ash::System::Windows::Library::User32::getDpiForSystem();
						return (getDpiForSystem != nullptr) ? getDpiForSystem() : USER_DEFAULT_SCREEN_DPI;
					}

					static inline int get(int index)
					{
						Ash::System::Windows::Library::User32::GetSystemMetricsForDpi getSystemMetricsForDpi = Ash::System::Windows::Library::User32::getSystemMetricsForDpi();
						return (getSystemMetricsForDpi != nullptr) ? getSystemMetricsForDpi(index, getSystemDpi()) : ::GetSystemMetrics(index);
					}
				};
			}
		}
	}
}
