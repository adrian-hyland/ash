#pragma once

#include <cstring>
#include "ash.error.h"


namespace Ash
{
	namespace Concurrency
	{
		namespace Error
		{
			using Code = unsigned int;

			class Category : public Ash::Error::Category
			{
			public:
				virtual Name getName() const override
				{
					return "Ash::Concurrency::Error";
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
						"Invalid environment name",
						"Environment name not found",
						"Thread not started",
						"Thread already started",
						"Process not started",
						"Process already started",
						"Invalid queue",
						"Time out",
						"Acquire failed",
						"Wait failed"
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

			constexpr Ash::Error::Value invalidEnvironmentName  = { category, 0 };
			constexpr Ash::Error::Value environmentNameNotFound = { category, 1 };
			constexpr Ash::Error::Value threadNotStarted        = { category, 2 };
			constexpr Ash::Error::Value threadAlreadyStarted    = { category, 3 };
			constexpr Ash::Error::Value processNotStarted       = { category, 4 };
			constexpr Ash::Error::Value processAlreadyStarted   = { category, 5 };
			constexpr Ash::Error::Value invalidQueue            = { category, 6 };
			constexpr Ash::Error::Value timeOut                 = { category, 7 };
			constexpr Ash::Error::Value acquireFailed           = { category, 8 };
			constexpr Ash::Error::Value waitFailed              = { category, 9 };
		}
	}
}
