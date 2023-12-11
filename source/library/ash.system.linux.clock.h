#pragma once

#include <cstdint>
#include <ctime>
#include <cerrno>


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			class Clock
			{
			public:
				enum Type
				{
					HighResolution,
					LowResolution,
					ProcessTime,
					ThreadTime
				};

				using Tick = uint64_t;

				using Nanosecond = uint64_t;

				constexpr Clock(Type type) : m_Type(type) {}

				constexpr Type getType() const { return m_Type; }

				inline Tick getTick() const
				{
					timespec time = {};
				
					::clock_gettime(getClockId(), &time);

					return time.tv_sec * 1000000000ULL + time.tv_nsec;
				}

				constexpr Nanosecond getTickDuration() const
				{
					return 1;
				}

				inline Tick getTickResolution() const
				{
					timespec time = {};
				
					::clock_getres(getClockId(), &time);
					
					return time.tv_sec * 1000000000ULL + time.tv_nsec;
				}

				static inline void sleep(Nanosecond duration)
				{
					timespec wakeTime = {};
				
					::clock_gettime(CLOCK_MONOTONIC, &wakeTime);

					wakeTime = addTime(wakeTime, duration);

					while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeTime, nullptr) == EINTR)
						;
				}

			protected:
				constexpr clockid_t getClockId() const
				{
					switch (m_Type)
					{
						case ProcessTime:
							return CLOCK_PROCESS_CPUTIME_ID;
						break;

						case ThreadTime:
							return CLOCK_THREAD_CPUTIME_ID;
						break;

						case LowResolution:
							return CLOCK_REALTIME_COARSE;
						break;

						case HighResolution:
						default:
							return CLOCK_MONOTONIC_RAW;
						break;
					}
				}

				static constexpr timespec addTime(timespec time, uint64_t value)
				{
					timespec result = {};

					value = value + time.tv_nsec;
					result.tv_nsec = value % 1000000000ULL;
					result.tv_sec = time.tv_sec + value / 1000000000ULL;

					return result;
				}

			private:
				Type m_Type;
			};
		}
	}
}
