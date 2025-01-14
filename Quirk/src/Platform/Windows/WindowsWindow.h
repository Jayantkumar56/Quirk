
#pragma once

#ifdef QK_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Quirk {

	struct WindowSpecification;
	class  Window;

	class WindowsWindow{
	public:
		static void Init(HINSTANCE hInstance);
		static inline const std::wstring& GetWindowClassName()  { return m_WindClassName; }
		static inline const HINSTANCE GetApplicationHInstance() { return s_HInstance;     }

	public:
		WindowsWindow(const WindowSpecification& spec, Window* window);
		~WindowsWindow();

		void OnUpdate();

		inline void* GetNativeHandle()				   { return m_WindowHandle;       }
		inline bool IsCursorLocked()			 const { return m_CursorLocked;       }
		inline bool TrackingCursor()			 const { return !m_CursorLeftWindow;  }

		inline void LockCursor()	{ m_CursorLocked = true; }
		inline void UnlockCursor()	{ m_CursorLocked = true; }

	private:
		static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void AdjustWindowSizeForDPI(uint16_t& width, uint16_t& height) const;

	private:
		inline static HINSTANCE    s_HInstance;
		inline static DWORD		   m_WindowStyle;
		inline static DWORD		   m_WindowExStyle;
		inline static std::wstring m_WindClassName;

		HWND m_WindowHandle;
		bool m_CursorLocked     = false;
		bool m_CursorLeftWindow = false;
	};

}


#endif // QK_PLATFORM_WINDOWS
