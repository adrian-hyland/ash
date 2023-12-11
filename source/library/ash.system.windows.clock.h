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
					ThreadTime
				};

				using Tick = uint64_t;

				using Nanosecond = uint64_t;

				constexpr Clock(Type type) : m_Type(type) {}

				constexpr Type getType() const { return m_Type; }

				inline Tick getTick() const
				{
					FILETIME creationTime, exitTime, kernelTime, userTime;

					switch (m_Type)
					{
						case ThreadTime:
							::GetThreadTimes(GetCurrentThread(), &creationTime, &exitTime, &kernelTime, &userTime);
							return addFileTime(kernelTime, userTime);
						break;

						case ProcessTime:
							::GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime);
							return addFileTime(kernelTime, userTime);
						break;

						case LowResolution:
							return ::GetTickCount64();
						break;

						case HighResolution:
						default:
							LARGE_INTEGER counter;
							::QueryPerformanceCounter(&counter);
							return counter.QuadPart;
						break;
					}
				}

				inline Nanosecond getTickDuration() const
				{
					switch (m_Type)
					{
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
					::Sleep(duration / 1000000ULL);
				}

			protected:
				static constexpr Tick addFileTime(FILETIME left, FILETIME right)
				{
					ULARGE_INTEGER leftValue = {};
					ULARGE_INTEGER rightValue = {};

					leftValue.LowPart = left.dwLowDateTime;
					leftValue.HighPart = left.dwHighDateTime;

					rightValue.LowPart = right.dwLowDateTime;
					rightValue.HighPart = right.dwHighDateTime;

					return (leftValue.QuadPart + rightValue.QuadPart);
				}

			private:
				Type m_Type;
			};
		}
	}
}
