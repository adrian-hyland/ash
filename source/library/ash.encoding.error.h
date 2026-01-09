#pragma once

#include <cstring>
#include "ash.error.h"


namespace Ash
{
	namespace Encoding
	{
		namespace Error
		{
			using Code = unsigned int;

			class Category : public Ash::Error::Category
			{
			public:
				virtual Name getName() const override
				{
					return "Ash::Encoding::Error";
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
						"Invalid code unit",
						"Invalid code point",
						"Overlong encoding"
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

			constexpr Ash::Error::Value invalidCodeUnit  = { category, 0 };
			constexpr Ash::Error::Value invalidCodePoint = { category, 1 };
			constexpr Ash::Error::Value overlongEncoding = { category, 2 };
		}
	}
}
