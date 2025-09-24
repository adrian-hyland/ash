#pragma once

#include <windows.h>
#include "ash.error.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			class Error : public Ash::Error::Value
			{
			public:
				using Code = DWORD;

				class Category : public Ash::Error::Category
				{
				public:
					virtual Name getName() const override
					{
						return "Ash::System::Windows";
					}

					virtual bool getErrorCodeDescription(Ash::Error::Code code, Ash::Error::Code::Description description) const override
					{
						return Ash::System::Windows::Error::getErrorCodeDescription(code, description);
					}
				};

				static inline const Category category;

				Error() : Error(GetLastError()) {}

				Error(Code code) : Ash::Error::Value()
				{
					if (code != ERROR_SUCCESS)
					{
						set(category, code);
					}
				}

			protected:
				static bool getErrorCodeDescription(Code code, Ash::Error::Code::Description description)
				{
					return ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), description, sizeof(Ash::Error::Code::Description) - 1, nullptr) > 0;
				}
			};
		}
	}
}
