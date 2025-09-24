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
			class Error : public Ash::Error::Value
			{
			public:
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
						return Ash::System::Linux::Error::getErrorCodeDescription(code, description);
					}
				};

				static inline const Category category;

				Error() : Error(errno) {}

				Error(Code code) : Ash::Error::Value()
				{
					if (code != 0)
					{
						set(category, code);
					}
				}

			protected:
				static bool getErrorCodeDescription(Code code, Ash::Error::Code::Description description)
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
		}
	}
}
