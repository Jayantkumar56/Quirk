

#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS

#include "Core/core.h"
#include "WindowsWindow.h"
#include "Core/Input/KeyCodes.h"

#include "Core/Input/Events.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Input/KeyboardEvents.h"
#include "Core/Input/ApplicationEvents.h"

namespace Quirk {
	HINSTANCE WindowsWindow::s_HInstance = nullptr;

	std::bitset<256> WindowsWindow::s_KeyPressed;
	std::bitset<256> WindowsWindow::s_KeyRepeat;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

    void WindowsWindow::Init(HINSTANCE hInstance) {
		QK_CORE_ASSERT(hInstance, "Didn't got HINSTANCE from windows api!");
		s_HInstance = hInstance;

		// to maintin uniformity in window size among different dpi monitors
		QK_CORE_ASSERTEX(
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2), 
			"Unable to set Dpi Awareness to DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2"
		);
    }

	WindowsWindow::WindowsWindow(const WindowProps& props) : 
			m_WindowHandle(nullptr), 
			m_Data({props.Title, props.Width, props.Height})
	{
		WNDCLASSEXW wc = {};
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.hInstance = s_HInstance;
		wc.lpszClassName = L"Quirk Engine";
		wc.style = CS_HREDRAW | CS_VREDRAW;			// refresh window on resize
		wc.lpfnWndProc = WindowProc;				// pass our callback function

		QK_CORE_ASSERTEX(RegisterClassExW(&wc), "Failed to Register the window class!");

		m_WindowHandle = CreateWindowExW(
			WS_EX_ACCEPTFILES,						// The window accepts drag-drop files.
			L"Quirk Engine",						// Window class
			props.Title.c_str(),					// Window text
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,       // Window style

			CW_USEDEFAULT, CW_USEDEFAULT, props.Width, props.Height,

			NULL,			// Parent window    
			NULL,			// Menu
			s_HInstance,	// Instance handle
			NULL			// Additional application data
		);

		QK_CORE_ASSERT(m_WindowHandle, "Failed to create Window handle!");

		SetPropA(m_WindowHandle, "wndptr", this);
		SetForegroundWindow(m_WindowHandle);
	}

	void WindowsWindow::OnUpdate() {
		MSG msg = {};
		 
		while (PeekMessage(&msg, m_WindowHandle, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		s_KeyRepeat.reset();
	}

	LRESULT WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		WindowsWindow* window = (WindowsWindow*)GetPropA(hwnd, "wndptr");

		switch (uMsg) {
			case WM_CLOSE: {
				break;
			}

			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYUP: 
			case WM_KEYDOWN: {
				s_KeyPressed.set(wParam, (HIWORD(lParam) & KF_UP) == 0);

				// checking if the key is repeating
				s_KeyRepeat = s_KeyPressed;
				s_KeyRepeat.set(wParam, HIWORD(lParam) & KF_REPEAT);

				break;
			}
		}
		 
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}


#endif // QK_PLATFORM_WINDOWS
