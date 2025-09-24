#pragma once

#include <ctime>
#include "ash.calendar.date.h"
#include "ash.calendar.time.h"

#if defined PLATFORM_WINDOWS
#include "ash.system.windows.time.h"
#elif defined PLATFORM_LINUX
#include "ash.system.linux.time.h"
#endif


namespace Ash
{
	namespace System
	{
#if defined PLATFORM_WINDOWS
		using Time = Ash::System::Windows::Time;
#elif defined PLATFORM_LINUX
		using Time = Ash::System::Linux::Time;
#endif
	}

	namespace Calendar
	{
		using DateTimeDuration = Ash::System::Time::Value;


		class DateTime
		{
		public:
			static constexpr TimeDuration daylightSaving = Time::hour;

			constexpr DateTime() : m_UtcTime(0), m_UtcOffset(0), m_DstOffset(0) {}

			constexpr DateTime(DateDuration dateDuration, TimeDuration timeDuration, TimeDuration utcOffset = 0, TimeDuration dstOffset = 0) : m_UtcTime(DateTimeDuration(dateDuration) * Time::day + DateTimeDuration(timeDuration)), m_UtcOffset(utcOffset), m_DstOffset(dstOffset) {}

			constexpr DateDuration getUtcDate() const { return getDate(m_UtcTime); }

			constexpr TimeDuration getUtcTime() const { return getTime(m_UtcTime); }

			constexpr DateDuration getLocalDate() const { return getDate(m_UtcTime + m_UtcOffset + m_DstOffset); }

			constexpr TimeDuration getLocalTime() const { return getTime(m_UtcTime + m_UtcOffset + m_DstOffset); }

			constexpr TimeDuration getUtcOffset() const { return m_UtcOffset; }

			[[nodiscard]]
			static Ash::Error::Value getNow(DateTime &now) { return Ash::System::Time::getNow(now.m_UtcTime, now.m_UtcOffset, now.m_DstOffset); }

			static DateTime getNow()
			{
				DateTime now;

				getNow(now).throwOnError();

				return now;
			}

		protected:
			static constexpr DateDuration getDate(DateTimeDuration duration)
			{
				duration = (duration < 0) ? duration - Time::day + 1 : duration;
				return duration / Time::day;
			}

			static constexpr TimeDuration getTime(DateTimeDuration duration)
			{
				duration = duration % Time::day;
				return (duration < 0) ? duration + Time::day : duration;
			}

		private:
			DateTimeDuration m_UtcTime;
			TimeDuration     m_UtcOffset;
			TimeDuration     m_DstOffset;
		};
	}
}
