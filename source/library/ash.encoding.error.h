#pragma once

#include <cstring>
#include "ash.error.h"


namespace Ash
{
	namespace Encoding
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
					return "Ash::Encoding::Error";
				}

				virtual bool getErrorCodeDescription(Ash::Error::Code code, Ash::Error::Code::Description description) const override
				{
					return Ash::Encoding::Error::getErrorCodeDescription(code, description);
				}
			};

			static inline const Category category;

			static constexpr Ash::Error::Value invalidCodeUnit      = Ash::Error::Value(category, 0);
			static constexpr Ash::Error::Value invalidCodePoint     = Ash::Error::Value(category, 1);
			static constexpr Ash::Error::Value overlongEncoding     = Ash::Error::Value(category, 2);

		protected:
			static inline bool getErrorCodeDescription(Code code, Ash::Error::Code::Description description)
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
	}
}
