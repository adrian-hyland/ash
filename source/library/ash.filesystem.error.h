#pragma once

#include <cstring>
#if defined PLATFORM_WINDOWS
#include "ash.system.windows.error.h"
#elif defined PLATFORM_LINUX
#include "ash.system.linux.error.h"
#endif


namespace Ash
{
	namespace FileSystem
	{
		namespace Error
		{
#if defined PLATFORM_WINDOWS
			namespace System = Ash::System::Windows;
#elif defined PLATFORM_LINUX
			namespace System = Ash::System::Linux;
#endif

			using Code = unsigned int;

			class Category : public Ash::Error::Category
			{
			public:
				virtual Name getName() const override
				{
					return "Ash::FileSystem::Error";
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
						"Invalid drive",
						"Invalid drive path",
						"Invalid network name",
						"Invalid network path",
						"Invalid path component",
						"Invalid path",
						"Path is not relative",
						"Root path is not supported",
						"Drive path is not supported",
						"Network path is not supported",
						"End of file"
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

			constexpr Ash::Error::Value invalidDrive            = { category, 0  };
			constexpr Ash::Error::Value invalidDrivePath        = { category, 1  };
			constexpr Ash::Error::Value invalidNetworkName      = { category, 2  };
			constexpr Ash::Error::Value invalidNetworkPath      = { category, 3  };
			constexpr Ash::Error::Value invalidPathComponent    = { category, 4  };
			constexpr Ash::Error::Value invalidPath             = { category, 5  };
			constexpr Ash::Error::Value pathNotRelative         = { category, 6  };
			constexpr Ash::Error::Value rootPathNotSupported    = { category, 7  };
			constexpr Ash::Error::Value drivePathNotSupported   = { category, 8  };
			constexpr Ash::Error::Value networkPathNotSupported = { category, 9  };
			constexpr Ash::Error::Value endOfFile               = { category, 10 };

			constexpr Ash::Error::Value pathNotFound            = System::Error::pathNotFound;
			constexpr Ash::Error::Value accessDenied            = System::Error::accessDenied;
			constexpr Ash::Error::Value fileNotOpen             = System::Error::fileNotOpen;
			constexpr Ash::Error::Value diskOutOfSpace          = System::Error::diskOutOfSpace;
		}
	}
}
