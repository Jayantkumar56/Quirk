
#pragma once

#ifdef QK_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Quirk {

	struct WindowSpecification;
	class  Window;

	class WindowsWindow{
	public:
		WindowsWindow(const WindowSpecification& spec, Window* window);
		~WindowsWindow();

		static void Init(HINSTANCE hInstance);

		void OnUpdate();

		inline void* GetNativeHandle()				   { return m_WindowHandle;       }
		inline bool IsCursorLocked()			 const { return m_CursorLocked;       }
		inline bool TrackingCursor()			 const { return !m_CursorLeftWindow;  }
		inline std::wstring GetWindowClassName() const { return m_WindClassName;      }

		inline void LockCursor()	{ m_CursorLocked = true; }
		inline void UnlockCursor()	{ m_CursorLocked = true; }

	private:
		static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void AdjustWindowSizeForDPI(uint16_t& width, uint16_t& height) const;

	private:
		static HINSTANCE s_HInstance;

		HWND		 m_WindowHandle;
		DWORD		 m_WindowStyle;
		DWORD		 m_WindowExStyle;
		std::wstring m_WindClassName;
		bool		 m_CursorLocked = false;
		bool		 m_CursorLeftWindow = false;
	};

}


#endif // QK_PLATFORM_WINDOWS
