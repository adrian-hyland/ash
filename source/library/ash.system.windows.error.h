#pragma once

#include <windows.h>
#include "ash.error.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace Error
			{
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
						return getDescription(code, description);
					}

				protected:
					static bool getDescription(Code code, Ash::Error::Code::Description description)
					{
						return ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), description, sizeof(Ash::Error::Code::Description) - 1, nullptr) > 0;
					}
				};

				inline constexpr Category category;

				constexpr Ash::Error::Value pathNotFound            = { category, ERROR_PATH_NOT_FOUND   };
				constexpr Ash::Error::Value accessDenied            = { category, ERROR_ACCESS_DENIED    };
				constexpr Ash::Error::Value fileNotOpen             = { category, ERROR_INVALID_HANDLE   };
				constexpr Ash::Error::Value diskOutOfSpace          = { category, ERROR_DISK_FULL        };
				constexpr Ash::Error::Value environmentNameNotFound = { category, ERROR_ENVVAR_NOT_FOUND };
				constexpr Ash::Error::Value timeOut                 = { category, ERROR_TIMEOUT          };
			}

			constexpr Ash::Error::Value error(Ash::System::Windows::Error::Code code = ::GetLastError())
			{
				return (code == ERROR_SUCCESS) ? Ash::Error::none : Ash::Error::Value(Ash::System::Windows::Error::category, code);
			}
		}
	}
}
