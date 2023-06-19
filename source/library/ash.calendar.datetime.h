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
			static constexpr TimeDuration daylightSaving = Time::hours;

			constexpr DateTime() : m_UtcTime(0), m_UtcOffset(0), m_DstOffset(0) {}

			constexpr DateTime(DateDuration dateDuration, TimeDuration timeDuration, TimeDuration utcOffset = 0, TimeDuration dstOffset = 0) : m_UtcTime(DateTimeDuration(dateDuration) * Time::days + DateTimeDuration(timeDuration)), m_UtcOffset(utcOffset), m_DstOffset(dstOffset) {}

			constexpr DateDuration getUtcDate() const { return getDate(m_UtcTime); }

			constexpr TimeDuration getUtcTime() const { return getTime(m_UtcTime); }

			constexpr DateDuration getLocalDate() const { return getDate(m_UtcTime + m_UtcOffset + m_DstOffset); }

			constexpr TimeDuration getLocalTime() const { return getTime(m_UtcTime + m_UtcOffset + m_DstOffset); }

			constexpr TimeDuration getUtcOffset() const { return m_UtcOffset; }

			static DateTime getNow()
			{
				time_t timeNow = time(nullptr);

				DateTime dateTime;
				dateTime.m_UtcTime = getSystemEpoch() + timeNow;
				dateTime.m_UtcOffset = -timezone;
				dateTime.m_DstOffset = (getLocalTime(timeNow).tm_isdst == 0) ? 0 : daylightSaving;

				return dateTime;
			}

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

				Date date;
				date.setYearMonthDay(gmtEpoch.tm_year + 1900, Month(gmtEpoch.tm_mon + 1), Day(gmtEpoch.tm_mday));
				Time time(gmtEpoch.tm_hour, gmtEpoch.tm_min, gmtEpoch.tm_sec);

				return DateTimeDuration(date) * Time::days + time;
			}

			static constexpr DateDuration getDate(DateTimeDuration duration)
			{
				duration = (duration < 0) ? duration - Time::days + 1 : duration;
				return duration / Time::days;
			}

			static constexpr TimeDuration getTime(DateTimeDuration duration)
			{
				duration = duration % Time::days;
				return (duration < 0) ? duration + Time::days : duration;
			}

		private:
			DateTimeDuration m_UtcTime;
			TimeDuration     m_UtcOffset;
			TimeDuration     m_DstOffset;
		};
	}
}
