#pragma once

#include <shellscalingapi.h>
#include "ash.system.windows.library.module.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace Library
			{
				class Shcore
				{
				public:
					using GetProcessDpiAwareness = HRESULT (*)(HANDLE, PROCESS_DPI_AWARENESS *);
					using SetProcessDpiAwareness = HRESULT (*)(PROCESS_DPI_AWARENESS);

					static inline GetProcessDpiAwareness getProcessDpiAwareness()
					{
						return m_Instance.getProcessDpiAwareness();
					}

					static inline SetProcessDpiAwareness setProcessDpiAwareness()
					{
						return m_Instance.setProcessDpiAwareness();
					}

				protected:
					class Instance : public Ash::System::Windows::Library::Module
					{
					public:
						using Module = Ash::System::Windows::Library::Module;

						inline Instance() : Module(L"shcore.dll")
						{
							m_GetProcessDpiAwareness = getFunctionAddress<GetProcessDpiAwareness>("GetProcessDpiAwareness");
							m_SetProcessDpiAwareness = getFunctionAddress<SetProcessDpiAwareness>("SetProcessDpiAwareness");
						}

						constexpr GetProcessDpiAwareness getProcessDpiAwareness() const
						{
							return m_GetProcessDpiAwareness;
						}

						constexpr SetProcessDpiAwareness setProcessDpiAwareness() const
						{
							return m_SetProcessDpiAwareness;
						}

					private:
						GetProcessDpiAwareness m_GetProcessDpiAwareness = nullptr;
						SetProcessDpiAwareness m_SetProcessDpiAwareness = nullptr;
					};

				private:
					static inline Instance m_Instance = Instance();

					Shcore() = delete;
				};
			}
		}
	}
}
