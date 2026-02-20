#pragma once

#include <cstring>
#include "ash.system.linux.error.h"
#include "ash.system.linux.x11.connection.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Server
				{
					namespace Error
					{
						using Code = uint8_t;

						class Category : public Ash::Error::Category
						{
						public:
							virtual Name getName() const override
							{
								return "Ash::System::Linux::X11::Server::Error";
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
									nullptr,
									"Bad request",
									"Bad value",
									"Bad window",
									"Bad pixmap",
									"Bad atom",
									"Bad cursor",
									"Bad font",
									"Bad match",
									"Bad drawable",
									"Bad access",
									"Bad alloc",
									"Bad colormap",
									"Bad GContext",
									"Bad IDChoice",
									"Bad name",
									"Bad length",
									"Bad implementation"
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

						constexpr Ash::Error::Value request        = { category, 1  };
						constexpr Ash::Error::Value value          = { category, 2  };
						constexpr Ash::Error::Value window         = { category, 3  };
						constexpr Ash::Error::Value pixmap         = { category, 4  };
						constexpr Ash::Error::Value atom           = { category, 5  };
						constexpr Ash::Error::Value cursor         = { category, 6  };
						constexpr Ash::Error::Value font           = { category, 7  };
						constexpr Ash::Error::Value match          = { category, 8  };
						constexpr Ash::Error::Value drawable       = { category, 9  };
						constexpr Ash::Error::Value access         = { category, 10 };
						constexpr Ash::Error::Value alloc          = { category, 11 };
						constexpr Ash::Error::Value colormap       = { category, 12 };
						constexpr Ash::Error::Value gContext       = { category, 13 };
						constexpr Ash::Error::Value idChoice       = { category, 14 };
						constexpr Ash::Error::Value name           = { category, 15 };
						constexpr Ash::Error::Value length         = { category, 16 };
						constexpr Ash::Error::Value implementation = { category, 17 };
					}

					constexpr Ash::Error::Value error(Ash::System::Linux::X11::Server::Error::Code code)
					{
						return Ash::Error::Value(Ash::System::Linux::X11::Server::Error::category, code);
					}
				}
			}
		}
	}
}
