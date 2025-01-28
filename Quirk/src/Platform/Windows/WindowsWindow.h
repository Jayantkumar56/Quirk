
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

		WindowsWindow(WindowsWindow&& other) noexcept {
			// taking ownership of window handle from the other WindowsWindow object
			m_WindowHandle = other.m_WindowHandle;
			other.m_WindowHandle = nullptr;
		}

		WindowsWindow& operator=(WindowsWindow&& other) noexcept {
			// taking ownership of window handle from the other WindowsWindow object
			m_WindowHandle = other.m_WindowHandle;
			other.m_WindowHandle = nullptr;
		}

		~WindowsWindow();

		void OnUpdate() const;

		inline void* GetNativeHandle()	   noexcept { return m_WindowHandle;       }
		inline bool IsCursorLocked() const noexcept { return m_CursorLocked;       }
		inline bool TrackingCursor() const noexcept { return !m_CursorLeftWindow;  }

		inline void LockCursor()	noexcept { m_CursorLocked = true;  }
		inline void UnlockCursor()	noexcept { m_CursorLocked = false; }

	private:
		static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
