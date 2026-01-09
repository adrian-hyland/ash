#pragma once

#include <cstring>
#include "ash.error.h"


namespace Ash
{
	namespace Calendar
	{
		namespace Error
		{
			using Code = unsigned int;

			class Category : public Ash::Error::Category
			{
			public:
				virtual Name getName() const override
				{
					return "Ash::Calendar::Error";
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
						"Invalid duration value",
						"Invalid year value",
						"Invalid month value",
						"Invalid weekday value",
						"Invalid day value",
						"Invalid ordinal value",
						"Invalid week value",
						"Invalid hour value",
						"Invalid minute value",
						"Invalid second value"
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

			constexpr Ash::Error::Value invalidDuration = { category, 0 };
			constexpr Ash::Error::Value invalidYear     = { category, 1 };
			constexpr Ash::Error::Value invalidMonth    = { category, 2 };
			constexpr Ash::Error::Value invalidWeekday  = { category, 3 };
			constexpr Ash::Error::Value invalidDay      = { category, 4 };
			constexpr Ash::Error::Value invalidOrdinal  = { category, 5 };
			constexpr Ash::Error::Value invalidWeek     = { category, 6 };
			constexpr Ash::Error::Value invalidHour     = { category, 7 };
			constexpr Ash::Error::Value invalidMinute   = { category, 8 };
			constexpr Ash::Error::Value invalidSecond   = { category, 9 };
		}
	}
}
