#pragma once

#include <windows.h>
#include "ash.calendar.date.h"
#include "ash.calendar.time.h"
#include "ash.system.windows.error.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			class Time
			{
			public:
				using Value = int64_t;

				static inline Ash::Error::Value getNow(Value &utcTime, Ash::Calendar::TimeDuration &utcOffset, Ash::Calendar::TimeDuration &dstOffset)
				{
					Value sinceEpoch;

					Ash::Error::Value error = getTime(sinceEpoch);
					if (error == Ash::Error::none)
					{
						TIME_ZONE_INFORMATION timeZoneInformation;

						utcTime = m_Epoch + sinceEpoch;
						dstOffset = (::GetTimeZoneInformation(&timeZoneInformation) == TIME_ZONE_ID_DAYLIGHT) ? -timeZoneInformation.DaylightBias * 60 : 0;
						utcOffset = -timeZoneInformation.Bias * 60;
					}

					return error;
				}

			protected:
				static inline Ash::Error::Value getTime(Value &sinceEpoch)
				{
					FILETIME fileTime;
					ULARGE_INTEGER integer;

					::GetSystemTimeAsFileTime(&fileTime);

					integer.LowPart = fileTime.dwLowDateTime;
					integer.HighPart = fileTime.dwHighDateTime;
					sinceEpoch = integer.QuadPart / 10000000ULL;

					return Ash::Error::none;
				}

				static inline Ash::Error::Value getGmtTime(Value sinceEpoch, SYSTEMTIME &gmtTime)
				{
					FILETIME fileTime;
					ULARGE_INTEGER integer;

					integer.QuadPart = sinceEpoch * 10000000ULL;
					fileTime.dwLowDateTime = integer.LowPart;
					fileTime.dwHighDateTime = integer.HighPart;

					return !::FileTimeToSystemTime(&fileTime, &gmtTime) ? Ash::System::Windows::Error() : Ash::Error::none;
				}

				static inline Ash::Calendar::Date getEpoch()
				{
					SYSTEMTIME epoch;

					getGmtTime(Value(), epoch).throwOnError();

					return Ash::Calendar::Date::withYearMonthDay(epoch.wYear, epoch.wMonth, epoch.wDay);
				}

			private:
				static inline Value m_Epoch = getEpoch() * Value(Ash::Calendar::Time::day);
			};
		}
	}
}
