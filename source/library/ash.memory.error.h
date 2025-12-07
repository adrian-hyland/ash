#pragma once

#include <cstring>
#include "ash.error.h"


namespace Ash
{
	namespace Memory
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
					return "Ash::Memory::Error";
				}

				virtual bool getErrorCodeDescription(Ash::Error::Code code, Ash::Error::Code::Description description) const override
				{
					return Ash::Memory::Error::getErrorCodeDescription(code, description);
				}
			};

			static inline const Category category;

			static constexpr Ash::Error::Value allocationFailure     = Ash::Error::Value(category, 0);
			static constexpr Ash::Error::Value readAccessOutOfBound  = Ash::Error::Value(category, 1);
			static constexpr Ash::Error::Value writeAccessOutOfBound = Ash::Error::Value(category, 2);
			static constexpr Ash::Error::Value lengthIsFixed         = Ash::Error::Value(category, 3);
			static constexpr Ash::Error::Value lengthOverflow        = Ash::Error::Value(category, 4);
			static constexpr Ash::Error::Value capacityIsFixed       = Ash::Error::Value(category, 5);
			static constexpr Ash::Error::Value capacityOverrun       = Ash::Error::Value(category, 6);
			static constexpr Ash::Error::Value capacityBelowLength   = Ash::Error::Value(category, 7);

		protected:
			static inline bool getErrorCodeDescription(Code code, Ash::Error::Code::Description description)
			{
				static const char *errorDescriptions[] =
				{
					"Allocation failure",
					"Read access out of bound",
					"Write access out of bound",
					"Length is fixed",
					"Length overflow",
					"Capacity is fixed",
					"Capacity overrun",
					"Capacity below length"
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
