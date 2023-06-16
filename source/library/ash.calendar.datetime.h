#pragma once

#include <ctime>
#include "ash.calendar.date.h"
#include "ash.calendar.time.h"


namespace Ash
{
	namespace Calendar
	{
		using DateTimeDuration = int64_t;


		class DateTime
		{
		public:
			constexpr DateTime() : m_UtcTime(0), m_UtcOffset(0), m_DstOffset(0) {}

			constexpr DateTime(DateDuration dateDuration, TimeDuration timeDuration, TimeDuration utcOffset = 0, TimeDuration dstOffset = 0) : m_UtcTime(DateTimeDuration(dateDuration) * Time::days + DateTimeDuration(timeDuration)), m_UtcOffset(utcOffset), m_DstOffset(dstOffset) {}

			constexpr DateTime(DateTimeDuration duration, TimeDuration utcOffset = 0, TimeDuration dstOffset = 0) : m_UtcTime(duration), m_UtcOffset(utcOffset), m_DstOffset(dstOffset) {}

			constexpr operator DateTimeDuration () const { return m_UtcTime; }

			constexpr DateDuration getUtcDate() const { return m_UtcTime / Time::days; }

			constexpr DateDuration getUtcTime() const { return m_UtcTime % Time::days; }

			constexpr DateDuration getLocalDate() const { return (m_UtcTime + m_UtcOffset + m_DstOffset) / Time::days; }

			constexpr TimeDuration getLocalTime() const { return (m_UtcTime + m_UtcOffset + m_DstOffset) % Time::days; }

			static DateTime getNow()
			{
				time_t timeNow = time(nullptr);

				struct tm localNow = getLocalTime(timeNow);

				return { getSystemEpoch() + timeNow, TimeDuration(-timezone), (localNow.tm_isdst == 0) ? 0 : Time::hours * 1 };
			}

			constexpr DateTime &operator ++ () { *this = *this + 1; return *this; }

			constexpr DateTime &operator -- () { *this = *this - 1; return *this; }

			constexpr DateTime operator ++ (int) { DateTime date = *this; ++(*this); return date; }

			constexpr DateTime operator -- (int) { DateTime date = *this; --(*this); return date; }

		protected:
			static struct tm getGmtTime(time_t systemTime)
			{
				struct tm gmtTime;

				#ifdef __WIN32__
				gmtime_s(&gmtTime, &systemTime);
				#else
				gmtime_r(&systemTime, &gmtTime);
				#endif

				return gmtTime;
			}

			static struct tm getLocalTime(time_t systemTime)
			{
				struct tm localTime;

				#ifdef __WIN32__
				localtime_s(&localTime, &systemTime);
				#else
				localtime_r(&systemTime, &localTime);
				#endif

				return localTime;
			}

			static DateTimeDuration getSystemEpoch()
			{
				struct tm gmtEpoch = getGmtTime(0);

				Gregorian::Date date;
				date.setYearMonthDay(gmtEpoch.tm_year + 1900, Month(gmtEpoch.tm_mon + 1), Day(gmtEpoch.tm_mday));
				Time time(gmtEpoch.tm_hour, gmtEpoch.tm_min, gmtEpoch.tm_sec);

				return DateTime(date, time);
			}

		private:
			DateTimeDuration m_UtcTime;
			TimeDuration     m_UtcOffset;
			TimeDuration     m_DstOffset;
		};
	}
}
