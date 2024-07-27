

#pragma once

#ifdef QK_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <utility>


namespace Quirk {

	class NativeTime {
	public:
		NativeTime();
		~NativeTime() = default;

		inline void RefreshTime();

		inline double GetTimeMillisecond() const { return m_CurrentTime * 1000; }
		inline double GetTimeSecond() const { return m_CurrentTime; }
		inline double GetDeltaTime() const { return (m_CurrentTime - m_LastTime); }

	private:
		uint64_t m_Frequency;
		uint64_t m_PerfCounterOffset;
		uint64_t m_CurrentPerfCounter;
		double m_LastTime;
		double m_CurrentTime;
	};

}

#endif // QK_PLATFORM_WINDOWS
