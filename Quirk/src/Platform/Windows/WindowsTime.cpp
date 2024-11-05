

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

}

#endif // QK_PLATFORM_WINDOWS
