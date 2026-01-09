#pragma once

#include <cstring>
#include <cerrno>
#include "ash.error.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace Error
			{
				using Code = int;

				class Category : public Ash::Error::Category
				{
				public:
					virtual Name getName() const override
					{
						return "Ash::System::Linux";
					}

					virtual bool getErrorCodeDescription(Ash::Error::Code code, Ash::Error::Code::Description description) const override
					{
						return getDescription(code, description);
					}

				protected:
					static bool getDescription(Code code, Ash::Error::Code::Description description)
					{
						const char *value = strerror(code);
						if ((value == nullptr) || (strlen(value) > sizeof(Ash::Error::Code::Description) - 1))
						{
							return false;
						}

						strcpy(description, value);
						return true;
					}
				};

				inline constexpr Category category;

				constexpr Ash::Error::Value pathNotFound   = { category, ENOENT };
				constexpr Ash::Error::Value accessDenied   = { category, EACCES };
				constexpr Ash::Error::Value fileNotOpen    = { category, EBADF  };
				constexpr Ash::Error::Value diskOutOfSpace = { category, ENOSPC };
			}

			constexpr Ash::Error::Value error(Ash::System::Linux::Error::Code code = errno)
			{
				return (code == 0) ? Ash::Error::none : Ash::Error::Value(Ash::System::Linux::Error::category, code);
			}
		}
	}
}
