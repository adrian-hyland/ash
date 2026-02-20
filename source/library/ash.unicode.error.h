#pragma once

#include <cstring>
#include "ash.error.h"


namespace Ash
{
	namespace Unicode
	{
		namespace Error
		{
			using Code = unsigned int;

			class Category : public Ash::Error::Category
			{
			public:
				virtual Name getName() const override
				{
					return "Ash::Unicode::Error";
				}

				virtual bool getErrorCodeDescription(Ash::Error::Code code, Ash::Error::Code::Description description) const override
				{
					return getDescription(code, description);
				}

			protected:
				static bool getDescription(Code code, Ash::Error::Code::Description description)
				{
					static const char *errorDescriptions[] =
					{
						"Invalid value",
					};

					const char *errorDescription = (code < sizeof(errorDescriptions) / sizeof(errorDescriptions[0])) ? errorDescriptions[code] : nullptr;
					if ((errorDescription == nullptr) || (strlen(errorDescription) > sizeof(Ash::Error::Code::Description) - 1))
					{
						return false;
					}

					strcpy(description, errorDescription);
					return true;
				}
			};

			inline constexpr Category category;

			constexpr Ash::Error::Value invalidValue = { category, 0 };
		}
	}
}
