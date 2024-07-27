

#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS

#include "WindowsTime.h"
#include "Core/Utility/Time.h"

namespace Quirk {

	NativeTime Time::m_NativeTime;

	NativeTime::NativeTime() {
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_PerfCounterOffset);

		RefreshTime();
	}

	inline void NativeTime::RefreshTime() {
		QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentPerfCounter);

		m_LastTime = m_CurrentTime;
		m_CurrentTime = (double)(m_CurrentPerfCounter - m_PerfCounterOffset) / m_Frequency;
	}

}

#endif // QK_PLATFORM_WINDOWS
