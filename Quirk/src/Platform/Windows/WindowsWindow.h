
#pragma once

#ifdef QK_PLATFORM_WINDOWS

namespace Quirk {

	struct WindowSpecification;
	class  Window;

	class WindowsWindow {
	public:
		static void Init(HINSTANCE hInstance);
		static void Terminate();
		static inline const std::wstring_view GetWindowClassName()  { return m_WindClassName; }
		static inline const HINSTANCE GetApplicationHInstance()     { return s_HInstance;     }

	public:
		WindowsWindow(const WindowSpecification& spec, Window* window);

		// two different objects should not hold ownership of window handle (m_WindowHandle)
		WindowsWindow(WindowsWindow& other)            = delete;
		WindowsWindow& operator=(WindowsWindow& other) = delete;

		// NOTE: A thread cannot destroy a window created by a different thread,
		//       so Window should not be moved to such different thread bound object
		WindowsWindow(WindowsWindow&& other)            noexcept;
		WindowsWindow& operator=(WindowsWindow&& other) noexcept;

		// NOTE: From Win32 api documentation :-
		//		 A thread cannot use DestroyWindow to destroy a window 
		//		 created by a different thread.
		~WindowsWindow() { if (m_WindowHandle) DestroyWindow(m_WindowHandle); }

		void OnUpdate() const;

		inline void* GetNativeHandle()	   noexcept { return m_WindowHandle;       }
		inline bool IsCursorLocked() const noexcept { return m_CursorLocked;       }
		inline bool TrackingCursor() const noexcept { return !m_CursorLeftWindow;  }

		inline void LockCursor()	noexcept { m_CursorLocked = true;  }
		inline void UnlockCursor()	noexcept { m_CursorLocked = false; }

	private:
		static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// adjusts for the width and height of the titlebar and window frame according to the monitor dpi
		void AdjustWindowSizeForDPI(uint16_t& width, uint16_t& height) const;

	private:
		static HINSTANCE s_HInstance;
		static DWORD	 m_WindowStyle;
		static DWORD	 m_WindowExStyle;
		static std::wstring_view m_WindClassName;

		HWND m_WindowHandle;
		bool m_CursorLocked     = false;
		bool m_CursorLeftWindow = false;
	};

}


#endif // QK_PLATFORM_WINDOWS
