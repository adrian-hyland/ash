#pragma once

#include <windows.h>
#include "ash.system.windows.string.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace Library
			{
				class Module
				{
				public:
					using Handle = HMODULE;

					template
					<
						typename FUNCTION
					>
					FUNCTION getFunctionAddress(const char *functionName)
					{
						return ((m_Handle != nullptr) && (functionName != nullptr)) ? reinterpret_cast<FUNCTION>(::GetProcAddress(m_Handle, functionName)) : nullptr;
					}

				protected:
					Module(const wchar_t *name)
					{
						m_Handle = ::LoadLibraryW(name);
					}

					~Module()
					{
						if (m_Handle != nullptr)
						{
							::FreeLibrary(m_Handle);
						}
					}

				private:
					Handle m_Handle;

					Module(const Module &module) = delete;
					Module(Module &&module) = delete;
					Module &operator = (const Module &module) = delete;
					Module &operator = (Module &&module) = delete;
				};
			}
		}
	}
}
