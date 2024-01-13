#pragma once

#include <cstdint>
#include <windows.h>


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			class Clock
			{
			public:
				enum Type
				{
					HighResolution,
					LowResolution,
					ProcessTime,
					ThreadTime,
					System
				};

				using Tick = uint64_t;

				using Nanosecond = uint64_t;

				constexpr Clock(Type type) : m_Type(type) {}

				constexpr Type getType() const { return m_Type; }

				inline Tick getTick() const
				{
					switch (m_Type)
					{
						case System:
							return getSystemTick();
						break;

						case ThreadTime:
							return getThreadTimeTick();
						break;

						case ProcessTime:
							return getProcessTimeTick();
						break;

						case LowResolution:
							return getLowResolutionTick();
						break;

						case HighResolution:
						default:
							return getHighResolutionTick();
						break;
					}
				}

				inline Nanosecond getTickDuration() const
				{
					switch (m_Type)
					{
						case System:
							return 1000000ULL;
						break;

						case ProcessTime:
						case ThreadTime:
							return 100ULL;
						break;

						case LowResolution:
							return 1000000ULL;
						break;

						case HighResolution:
						default:
							static uint64_t resolution;
							if (resolution == 0)
							{
								LARGE_INTEGER frequency;
								::QueryPerformanceFrequency(&frequency);
								resolution = 1000000000ULL / frequency.QuadPart;
							}
							return resolution;
						break;
					}
				}

				constexpr Tick getTickResolution() const
				{
					return 1;
				}

				static inline void sleep(Nanosecond duration)
				{
					::Sleep(DWORD(duration / 1000000ULL));
				}

			protected:
				static inline Tick getSystemTick()
				{
					SYSTEMTIME systemTime;
					FILETIME fileTime;

					::GetSystemTime(&systemTime);
					::SystemTimeToFileTime(&systemTime, &fileTime);

					return getTick(fileTime) / 10000ULL;
				}

				static inline Tick getThreadTimeTick()
				{
					FILETIME creationTime, exitTime, kernelTime, userTime;

					::GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime);

					return getTick(kernelTime) + getTick(userTime);
				}

				static inline Tick getProcessTimeTick()
				{
					FILETIME creationTime, exitTime, kernelTime, userTime;

					::GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime);
					
					return getTick(kernelTime) + getTick(userTime);
				}

				static inline Tick getLowResolutionTick()
				{
					return ::GetTickCount64();
				}

				static inline Tick getHighResolutionTick()
				{
					LARGE_INTEGER counter;

					::QueryPerformanceCounter(&counter);

					return counter.QuadPart;
				}

				static constexpr Tick getTick(FILETIME fileTime)
				{
					ULARGE_INTEGER value = {};

					value.LowPart = fileTime.dwLowDateTime;
					value.HighPart = fileTime.dwHighDateTime;

					return value.QuadPart;
				}

			private:
				Type m_Type;
			};
		}
	}
}
