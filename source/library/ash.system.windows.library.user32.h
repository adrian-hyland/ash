#pragma once

#include <windows.h>
#include "ash.system.windows.library.module.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace Library
			{
				class User32
				{
				public:
					using GetDpiForSystem = UINT (*)();
					using GetDpiForWindow = UINT (*)(HWND);
					using GetSystemMetricsForDpi = int (*)(int, UINT);
					using AdjustWindowRectExForDpi = BOOL (*)(LPRECT, DWORD, BOOL, DWORD, UINT);

					static inline GetDpiForSystem getDpiForSystem()
					{
						return m_Instance.getDpiForSystem();
					}

					static inline GetDpiForWindow getDpiForWindow()
					{
						return m_Instance.getDpiForWindow();
					}

					static inline GetSystemMetricsForDpi getSystemMetricsForDpi()
					{
						return m_Instance.getSystemMetricsForDpi();
					}

					static inline AdjustWindowRectExForDpi adjustWindowRectExForDpi()
					{
						return m_Instance.adjustWindowRectExForDpi();
					}

				protected:
					class Instance : public Ash::System::Windows::Library::Module
					{
					public:
						using Module = Ash::System::Windows::Library::Module;

						inline Instance() : Module(L"user32.dll")
						{
							m_GetDpiForSystem = getFunctionAddress<GetDpiForSystem>("GetDpiForSystem");
							m_GetDpiForWindow = getFunctionAddress<GetDpiForWindow>("GetDpiForWindow");
							m_GetSystemMetricsForDpi = getFunctionAddress<GetSystemMetricsForDpi>("GetSystemMetricsForDpi");
							m_AdjustWindowRectExForDpi = getFunctionAddress<AdjustWindowRectExForDpi>("AdjustWindowRectExForDpi");
						}

						inline GetDpiForSystem getDpiForSystem()
						{
							return m_GetDpiForSystem;
						}

						inline GetDpiForWindow getDpiForWindow()
						{
							return m_GetDpiForWindow;
						}

						inline GetSystemMetricsForDpi getSystemMetricsForDpi()
						{
							return m_GetSystemMetricsForDpi;
						}

						inline AdjustWindowRectExForDpi adjustWindowRectExForDpi()
						{
							return m_AdjustWindowRectExForDpi;
						}

					private:
						GetDpiForSystem          m_GetDpiForSystem          = nullptr;
						GetDpiForWindow          m_GetDpiForWindow          = nullptr;
						GetSystemMetricsForDpi   m_GetSystemMetricsForDpi   = nullptr;
						AdjustWindowRectExForDpi m_AdjustWindowRectExForDpi = nullptr;
					};

				private:
					static inline Instance m_Instance = Instance();

					User32() = delete;
				};
			}
		}
	}
}
