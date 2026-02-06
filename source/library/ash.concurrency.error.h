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
				static inline bool getDescription(Code code, Ash::Error::Code::Description description)
				{
					static const char *errorDescriptions[] =
					{
						"Invalid environment name",
						"Environment name not found",
						"Thread not started",
						"Thread already started",
						"Process not started",
						"Process already started"
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

			constexpr Ash::Error::Value invalidEnvironmentName  = Ash::Error::Value(category, 0);
			constexpr Ash::Error::Value environmentNameNotFound = Ash::Error::Value(category, 1);
			constexpr Ash::Error::Value threadNotStarted        = Ash::Error::Value(category, 2);
			constexpr Ash::Error::Value threadAlreadyStarted    = Ash::Error::Value(category, 3);
			constexpr Ash::Error::Value processNotStarted       = Ash::Error::Value(category, 4);
			constexpr Ash::Error::Value processAlreadyStarted   = Ash::Error::Value(category, 5);
		}
	}
}
