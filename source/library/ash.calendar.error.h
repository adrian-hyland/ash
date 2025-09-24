#pragma once

#include <cstring>
#include "ash.error.h"


namespace Ash
{
	namespace Calendar
	{
		class Error : public Ash::Error::Value
		{
		public:
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
					return Ash::Calendar::Error::getErrorCodeDescription(code, description);
				}
			};

			static inline const Category category;

			static constexpr Ash::Error::Value invalidDuration     = Ash::Error::Value(category, 0);
			static constexpr Ash::Error::Value invalidYear         = Ash::Error::Value(category, 1);
			static constexpr Ash::Error::Value invalidMonth        = Ash::Error::Value(category, 2);
			static constexpr Ash::Error::Value invalidWeekday      = Ash::Error::Value(category, 3);
			static constexpr Ash::Error::Value invalidYearMonthDay = Ash::Error::Value(category, 4);
			static constexpr Ash::Error::Value invalidYearOrdinal  = Ash::Error::Value(category, 5);
			static constexpr Ash::Error::Value invalidYearWeek     = Ash::Error::Value(category, 6);

		protected:
			static inline bool getErrorCodeDescription(Code code, Ash::Error::Code::Description description)
			{
				static const char *errorDescriptions[] =
				{
					"Invalid duration value",
					"Invalid year value",
					"Invalid month value",
					"Invalid weekday value",
					"Invalid day value for the month and year that was given",
					"Invalid ordinal value for the year that was given",
					"Invalid week value for the year that was given"
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
	}
}
