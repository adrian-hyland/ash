#pragma once

#include <cstring>
#include "ash.system.linux.error.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Client
				{
					namespace Error
					{
						using Code = unsigned int;

						class Category : public Ash::Error::Category
						{
						public:
							virtual Name getName() const override
							{
								return "Ash::System::Linux::X11::Client::Error";
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
									"Exit event loop signalled",
									"Invalid window",
									"XKB extension not supported",
									"Invalid keyboard context",
									"Invalid keyboard device ID",
									"Invalid keyboard state",
									"Invalid keyboard map",
									"Invalid keyboard modifier index",
									"Invalid reply",
									"No request cookie",
									"Invalid event",
									"Invalid property string"
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

						constexpr Ash::Error::Value exitEventLoop                = { category, 0  };
						constexpr Ash::Error::Value invalidWindow                = { category, 1  };
						constexpr Ash::Error::Value xkbExtensionNotSupported     = { category, 2  };
						constexpr Ash::Error::Value invalidKeyboardContext       = { category, 3  };
						constexpr Ash::Error::Value invalidKeyboardDeviceId      = { category, 4  };
						constexpr Ash::Error::Value invalidKeyboardState         = { category, 5  };
						constexpr Ash::Error::Value invalidKeyboardMap           = { category, 6  };
						constexpr Ash::Error::Value invalidKeyboardModifierIndex = { category, 7  };
						constexpr Ash::Error::Value invalidReply                 = { category, 8  };
						constexpr Ash::Error::Value noRequestCookie              = { category, 9  };
						constexpr Ash::Error::Value invalidEvent                 = { category, 10 };
						constexpr Ash::Error::Value invalidPropertyString        = { category, 11 };
					}
				}
			}
		}
	}
}
