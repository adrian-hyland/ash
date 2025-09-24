#pragma once

#include <ctime>
#include "ash.calendar.date.h"
#include "ash.calendar.time.h"
#include "ash.system.linux.error.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
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
						struct tm localTime;

						error = getLocalTime(sinceEpoch, localTime);
						if (error == Ash::Error::none)
						{
							utcTime = m_Epoch + sinceEpoch;
							dstOffset = (localTime.tm_isdst == 0) ? 0 : Ash::Calendar::Time::hour;
							utcOffset = -timezone;
						}
					}

					return error;
				}

			protected:
				static inline Ash::Error::Value getTime(Value &sinceEpoch)
				{
					return (time(&sinceEpoch) == -1) ? Ash::System::Linux::Error() : Ash::Error::none;
				}

				static inline Ash::Error::Value getGmtTime(Value sinceEpoch, struct tm &gmtTime)
				{
					return (gmtime_r(&sinceEpoch, &gmtTime) == nullptr) ? Ash::System::Linux::Error() : Ash::Error::none;
				}

				static inline Ash::Error::Value getLocalTime(Value sinceEpoch, struct tm &localTime)
				{
					return (localtime_r(&sinceEpoch, &localTime) == nullptr) ? Ash::System::Linux::Error() : Ash::Error::none;
				}

				static inline Ash::Calendar::Date getEpoch()
				{
					struct tm epoch;

					getGmtTime(Value(), epoch).throwOnError();

					return Ash::Calendar::Date::withYearMonthDay(epoch.tm_year + 1900, Ash::Calendar::Month(epoch.tm_mon + 1), Ash::Calendar::Day(epoch.tm_mday));
				}

			private:
				static inline Value m_Epoch = getEpoch() * Value(Ash::Calendar::Time::day);
			};
		}
	}
}
