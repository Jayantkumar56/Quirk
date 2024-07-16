

#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS

#include "Core/core.h"
#include "WindowsWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

//#include "windowsx.h"
// from windowsx.h :-
#define GET_X_LPARAM(lp)                        ((float)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((float)(short)HIWORD(lp))

namespace Quirk {

	HINSTANCE WindowsWindow::s_HInstance = nullptr;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

    void WindowsWindow::Init(HINSTANCE hInstance) {
		QK_CORE_ASSERT(hInstance, "Didn't got initial HINSTANCE from windows api!");
		s_HInstance = hInstance;

		// to maintin uniformity in window size among different dpi monitors
		QK_CORE_ASSERTEX(
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2), 
			"Unable to set Dpi Awareness to DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2"
		);
    }

	WindowsWindow::WindowsWindow(const WindowProps& props) : 
			Title(props.Title),
			m_Data({ nullptr, props.Width, props.Height, props.Width, props.Height, nullptr}),
			m_Context(GraphicalContext::CreateContext())
	{
		DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		DWORD windowExStyle = WS_EX_ACCEPTFILES;
		LPCWSTR windClassName = L"Quirk";

		WNDCLASSEXW wc = {};
		wc.cbSize			= sizeof(WNDCLASSEXW);
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wc.lpfnWndProc		= WindowProc;		
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= s_HInstance;
		wc.hIcon			= 0;
		wc.hCursor			= LoadCursorW(NULL, IDC_ARROW);
		wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName		= 0;
		wc.lpszClassName	= windClassName;
		wc.hIconSm			= 0;

		QK_CORE_ASSERTEX(RegisterClassExW(&wc), "Failed to Register the window class! {0}", GetLastError());

		AdjustClientArea(windowStyle, windowExStyle);
		
		m_Data.WindowHandle = CreateWindowExW(
			windowExStyle,							// The window accepts drag-drop files.
			windClassName,							// Window class
			Title.c_str(),							// Window text
			windowStyle,							// Window style

			CW_USEDEFAULT, CW_USEDEFAULT, 
			m_Data.WindWidth, m_Data.WindHeight,

			NULL,			// Parent window    
			NULL,			// Menu
			s_HInstance,	// Instance handle
			NULL			// Additional application data
		);

		QK_CORE_ASSERT(m_Data.WindowHandle, "Failed to create Window handle!");

		// Creating graphical context for current window
		m_Context->Init((void*)m_Data.WindowHandle);

		// putting this WindowsWindow pointer into created HWND
		SetPropW(m_Data.WindowHandle, L"wndptr", this);
		SetForegroundWindow(m_Data.WindowHandle);
	}

	WindowsWindow::~WindowsWindow() {
		delete m_Context;
	}

	void WindowsWindow::OnUpdate() {
		glClearColor(0.10156f, 0.17968f, 0.20703f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 
		MSG msg;
		while (PeekMessageW(&msg, m_Data.WindowHandle, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		m_Context->SwapBuffer();
	}

	LRESULT WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		WindowsWindow* window = (WindowsWindow*)GetPropW(hwnd, L"wndptr");

		switch (uMsg) {
			case WM_CLOSE: {
				WindowCloseEvent event;
				window->m_Data.EventCallbackFn(event);
				return (LRESULT)0;
			}

			case WM_ERASEBKGND: {
				return (LRESULT)0;
			}

			case WM_SYSKEYUP:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_KEYDOWN: {
				int keyCode = LOWORD(wParam);
				int keyState = (HIWORD(lParam) & KF_UP) ? QK_KEY_RELEASED : QK_KEY_PRESSED;

				if (keyCode == QK_Key_Control) {
					// Right Control key have the extended key bit set
					keyCode = (HIWORD(lParam) & KF_EXTENDED) ? QK_Key_RightCtrl : QK_Key_LeftCtrl;
				}
				else if (keyCode == QK_Key_Alt) {
					// Right Control key have the extended key bit set
					keyCode = (HIWORD(lParam) & KF_EXTENDED) ? QK_Key_RightAlt : QK_Key_LeftAlt;
				}

				// keyPessed from InputData is not updated yet 
				// so it contains previous keyState
				if (keyState == QK_KEY_PRESSED && Input::IsKeyPressed(keyCode)) {
					keyState = QK_KEY_REPEAT;
				}

				Input::UpdateKey(keyCode, keyState);

				switch (keyState) {
					case QK_KEY_PRESSED: {
						KeyPressedEvent event(keyCode);
						window->m_Data.EventCallbackFn(event);
						break;
					}

					case QK_KEY_REPEAT: {
						KeyRepeatEvent event(keyCode);
						window->m_Data.EventCallbackFn(event);
						break;
					}

					case QK_KEY_RELEASED: {
						KeyReleaseEvent event(keyCode);
						window->m_Data.EventCallbackFn(event);
						break;
					}
				}

				return (LRESULT)0;
			}

			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP: {
				int button, buttonState;
				float PosX = GET_X_LPARAM(lParam), PosY = GET_Y_LPARAM(lParam);

				if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP) {
					button = QK_Key_LeftMouseBtn;
				}
				else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP) {
					button = QK_Key_RightMouseBtn;
				}
				else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP) {
					button = QK_Key_MidMouseBtn;
				}
				else {
					button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? QK_Key_ThumbForward : QK_Key_ThumbBack;
				}

				if (uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_XBUTTONUP) {
					buttonState = QK_KEY_RELEASED;
				}
				else {
					buttonState = QK_KEY_PRESSED;
				}

				// keyPessed from InputData is not updated yet 
				// so it contains previous keyState
				if (buttonState == QK_KEY_PRESSED && Input::IsKeyPressed(button)) {
					buttonState = QK_KEY_REPEAT;
				}

				Input::UpdateKey(button, buttonState);

				switch (buttonState) {
					case QK_KEY_PRESSED: {
						MouseButtonPressedEvent event(button, PosX, PosY);
						window->m_Data.EventCallbackFn(event);
						break;
					}

					case QK_KEY_REPEAT: {
						MouseButtonRepeatEvent event(button, PosX, PosY);
						window->m_Data.EventCallbackFn(event);
						break;
					}

					case QK_KEY_RELEASED: {
						MouseButtonReleasedEvent event(button, PosX, PosY);
						window->m_Data.EventCallbackFn(event);
						break;
					}
				}

				return (LRESULT)0;
			}

			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK: 
			case WM_XBUTTONDBLCLK: {
				int button;
				float PosX = GET_X_LPARAM(lParam), PosY = GET_Y_LPARAM(lParam);

				switch (uMsg) {
					case WM_LBUTTONDBLCLK: button = QK_Key_LeftMouseBtn;	break;
					case WM_RBUTTONDBLCLK: button = QK_Key_RightMouseBtn;	break;
					case WM_MBUTTONDBLCLK: button = QK_Key_MidMouseBtn;		break;

					case WM_XBUTTONDBLCLK: 
						button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? QK_Key_ThumbForward : QK_Key_ThumbBack;
				}

				Input::UpdateMousePos(PosX, PosY);
				MouseButtonDblClickEvent event(button, PosX, PosY);
				window->m_Data.EventCallbackFn(event);

				return (LRESULT)0;
			}

			case WM_MOUSEMOVE :{
				float PosX = GET_X_LPARAM(lParam), PosY = GET_Y_LPARAM(lParam);
				Input::UpdateMousePos(PosX, PosY);

				MouseMovedEvent event(PosX, PosY);
				window->m_Data.EventCallbackFn(event);

				return (LRESULT)0;
			}
		}
		 
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void WindowsWindow::AdjustClientArea(DWORD windowStyle, DWORD windowExStyle) {
		RECT rect = { 0, 0, m_Data.WindWidth, m_Data.WindHeight };

		QK_ASSERTEX(
			AdjustWindowRectExForDpi(&rect, windowStyle, false, windowExStyle, GetDpiForSystem()),
			"Failed to Adjust window rectangle for client area in dpi aware window!"
		);

		m_Data.WindWidth = static_cast<uint16_t>(rect.right - rect.left);
		m_Data.WindHeight = static_cast<uint16_t>(rect.bottom - rect.top);
	}

}


#endif // QK_PLATFORM_WINDOWS
