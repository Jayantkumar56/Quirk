

#pragma once

#ifdef QK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsTime.h"
#endif // QK_PLATFORM_WINDOWS


namespace Quirk {

	class Time {
	public:
		static inline void RefreshTime() { m_NativeTime.RefreshTime(); }
		static inline double GetTimeMillisecond() { return m_NativeTime.GetTimeMillisecond(); }
		static inline double GetTimeSecond() { return m_NativeTime.GetTimeSecond(); }
		static inline double GetDeltaTime() { return m_NativeTime.GetDeltaTime(); }

	private:
		static NativeTime m_NativeTime;
	};

}
