

#pragma once

#ifdef QK_PLATFORM_WINDOWS

namespace Quirk {

	class NativeTime {
	public:
		NativeTime();
		~NativeTime() = default;

		inline void RefreshTime() {
			QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentPerfCounter);
			m_LastTime = m_CurrentTime;
			m_CurrentTime = (double)(m_CurrentPerfCounter - m_PerfCounterOffset) / m_Frequency;
		}

		inline double GetTimeMillisecond() const { return m_CurrentTime * 1000; }
		inline double GetTimeSecond() const { return m_CurrentTime; }

		// gives delta time in seconds
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
