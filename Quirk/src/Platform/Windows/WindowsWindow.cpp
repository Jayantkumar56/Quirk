

#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS

#include "Core/Core.h"
#include "Core/Input/Input.h"
#include "Core/Input/Events.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Input/KeyboardEvents.h"
#include "Core/Input/ApplicationEvents.h"
#include "Core/Application/Window.h"
#include "WindowsWindow.h"

#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <WindowsX.h>
#include "dwmapi.h"

#define	DCX_USESTYLE 0x00010000

// to provide imgui with the event data
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef BOOL(*WIN32_EnableNonClientDpiScaling)(HWND);

namespace Quirk {

	HINSTANCE WindowsWindow::s_HInstance     = 0;
	DWORD	  WindowsWindow::m_WindowStyle   = 0;
	DWORD	  WindowsWindow::m_WindowExStyle = 0;
	std::wstring_view WindowsWindow::m_WindClassName;

	WindowsWindow::WindowsWindow(const WindowSpecification& spec, Window* window) {
		DWORD windExStyles = m_WindowExStyle;
		DWORD windStyles   = m_WindowStyle;
		if (spec.Maximized) 
			windStyles |= WS_MAXIMIZE;

		uint16_t windWidth  = spec.Width;
		uint16_t windHeight = spec.Height;
		AdjustWindowSizeForDPI(windWidth, windHeight);

		std::wstring title(spec.Title.begin(), spec.Title.end());

		m_WindowHandle = CreateWindowExW(
			windExStyles,								// The window accepts drag-drop files.
			m_WindClassName.data(),						// Window class
			title.c_str(),								// Window text
			windStyles,									// Window style
			spec.PosX,			spec.PosY,				// Postion of window on the screen
			windWidth,			windHeight,				// height and width of the window
			NULL,										// Parent window    
			NULL,										// Menu
			s_HInstance,								// Instance handle
			NULL										// Additional application data
		);

		QK_CORE_ASSERT(m_WindowHandle, "Failed to create Window handle!");

		// putting this Window pointer into created HWND
		SetPropW(m_WindowHandle, L"wndptr", window);
		SetActiveWindow(m_WindowHandle);

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		QK_CORE_ASSERT(SUCCEEDED(hr), "Unable to Initialize COM!");

		// Inform the application of the frame change to force redrawing without default titlebar
		SetWindowPos(m_WindowHandle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

		// in the spec PosX and PosY contains windowframe position 
		// calculating client area position and updating in the window object
		POINT clientOrigin = { 0, 0 };
		ClientToScreen(m_WindowHandle, &clientOrigin);
		window->m_PosX = clientOrigin.x;
		window->m_PosY = clientOrigin.y;
	}

	WindowsWindow::~WindowsWindow() {
		CoUninitialize();
		DestroyWindow(m_WindowHandle);
	}

    void WindowsWindow::Init(HINSTANCE hInstance) {
		QK_CORE_ASSERT(hInstance, "Didn't got initial HINSTANCE from windows api!");
		s_HInstance = hInstance;

		// to maintin uniformity in window size among different dpi monitors
		QK_CORE_ASSERTEX(
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2), 
			"Unable to set Dpi Awareness to DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2"
		);

		m_WindowStyle   = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		m_WindowExStyle = WS_EX_ACCEPTFILES;
		m_WindClassName = L"QuirkApp";

		WNDCLASSEXW wc   = {};
		wc.cbSize        = sizeof(WNDCLASSEXW);
		wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wc.lpfnWndProc   = WindowProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = s_HInstance;
		wc.hIcon         = 0;
		wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName  = 0;
		wc.lpszClassName = m_WindClassName.data();
		wc.hIconSm       = 0;

		QK_CORE_ASSERTEX(RegisterClassExW(&wc), "Failed to Register the window class! {0}", GetLastError());
    }

	void WindowsWindow::OnUpdate() {
		MSG msg;
		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	static void DrawWindowFrame(HWND hwnd) {
		HDC dc = GetDCEx(hwnd, 0, DCX_WINDOW | DCX_USESTYLE);
		PAINTSTRUCT paint;
		BeginPaint(hwnd, &paint);

		RECT frameRect;
		RECT clientRect;
		GetWindowRect(hwnd, &frameRect);
		GetClientRect(hwnd, &clientRect);

		ClientToScreen(hwnd, (LPPOINT)&clientRect.left);
		ClientToScreen(hwnd, (LPPOINT)&clientRect.right);

		{
			SetDCBrushColor(dc, RGB(27, 47, 51));
			SetDCPenColor(dc, RGB(27, 47, 51));

			SelectObject(dc, GetStockObject(DC_PEN));
			SelectObject(dc, GetStockObject(DC_BRUSH));

			// left part of the frame
			Rectangle(dc,
				0,
				0,
				clientRect.left - frameRect.left,
				clientRect.bottom - frameRect.top
			);
			// top part of the frame
			Rectangle(dc,
				clientRect.left - frameRect.left,
				0,
				frameRect.right - clientRect.left,
				clientRect.top - frameRect.top
			);
			// right part of the frame
			Rectangle(dc,
				clientRect.right - frameRect.left,
				0,
				frameRect.right - frameRect.left,
				frameRect.bottom - frameRect.top
			);
			// bottom part of the frame
			Rectangle(dc,
				0,
				clientRect.bottom - frameRect.top,
				clientRect.right - frameRect.left,
				frameRect.bottom - frameRect.top
			);
		}

		EndPaint(hwnd, &paint);
		ReleaseDC(hwnd, dc);
	}

	LRESULT WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Window* window = (Window*)GetPropW(hwnd, L"wndptr");
		WindowsWindow* windowNative = (WindowsWindow*)window->GetNativeWindowObject();

		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		if (!window) return DefWindowProc(hwnd, uMsg, wParam, lParam);

		switch (uMsg) {
			case WM_SETFOCUS:
			case WM_CREATE: 
			case WM_ACTIVATE: {
				return (LRESULT)0;
			}

			// TO DO: figure out what WM_NCACTIVATE can do?
			//		  occasional titlebar appearance can be removed by return (LRESULT)0; here but
			//		  facing issues with other child window when return (LRESULT)0;
			/*case WM_NCACTIVATE: {
				DrawWindowFrame(hwnd);
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}*/

			case WM_QUIT:
			case WM_CLOSE: {
				WindowCloseEvent event;
				EventDispatcher::DispatchEvent(event);
				return (LRESULT)0;
			}

			case WM_NCCALCSIZE: {
				if (lParam == NULL) return (LRESULT)0;

				UINT dpi = GetDpiForWindow(hwnd);
				int frameX  = 4;
				int frameY  = 4;
				int padding = 0;

				if (window->m_Maximized) {
					frameX  = GetSystemMetricsForDpi(SM_CXFRAME, dpi);
					frameY  = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
					padding = GetSystemMetricsForDpi(92, dpi);
				}

				NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lParam;
				RECT* windowRect = params->rgrc;
				windowRect->right  -= frameX + padding;
				windowRect->left   += frameX + padding;
				windowRect->bottom -= frameY + padding;
				windowRect->top    += frameY + padding;
				
				return (LRESULT)0;
			}

			case WM_NCPAINT: {
				DrawWindowFrame(hwnd);
				return (LRESULT)0;
			}

			case WM_NCHITTEST: {
				// Expand the hit test area for resizing
				const int borderWidth = 4;

				POINTS mousePos = MAKEPOINTS(lParam);
				POINT clientMousePos = { mousePos.x, mousePos.y };
				ScreenToClient(hwnd, &clientMousePos);

				RECT windowRect;
				GetClientRect(hwnd, &windowRect);

				if (clientMousePos.y >= windowRect.bottom - borderWidth) {
					if (clientMousePos.x <= borderWidth)
						return HTBOTTOMLEFT;
					else if (clientMousePos.x >= windowRect.right - borderWidth)
						return HTBOTTOMRIGHT;
					else
						return HTBOTTOM;
				}
				else if (clientMousePos.y <= borderWidth) {
					if (clientMousePos.x <= borderWidth)
						return HTTOPLEFT;
					else if (clientMousePos.x >= windowRect.right - borderWidth)
						return HTTOPRIGHT;
					else
						return HTTOP;
				}
				else if (clientMousePos.x <= borderWidth) {
					return HTLEFT;
				}
				else if (clientMousePos.x >= windowRect.right - borderWidth) {
					return HTRIGHT;
				}
	
				if (window->m_MoveWithCursor) {
					return HTCAPTION;
				}

				return HTCLIENT;
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
						EventDispatcher::DispatchEvent(event);
						break;
					}

					case QK_KEY_REPEAT: {
						KeyRepeatEvent event(keyCode);
						EventDispatcher::DispatchEvent(event);
						break;
					}

					case QK_KEY_RELEASED: {
						KeyReleaseEvent event(keyCode);
						EventDispatcher::DispatchEvent(event);
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
				float PosX = (float)GET_X_LPARAM(lParam), PosY = (float)GET_Y_LPARAM(lParam);

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
						EventDispatcher::DispatchEvent(event);
						break;
					}

					case QK_KEY_REPEAT: {
						MouseButtonRepeatEvent event(button, PosX, PosY);
						EventDispatcher::DispatchEvent(event);
						break;
					}

					case QK_KEY_RELEASED: {
						MouseButtonReleasedEvent event(button, PosX, PosY);
						EventDispatcher::DispatchEvent(event);
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
				float PosX = (float)GET_X_LPARAM(lParam), PosY = (float)GET_Y_LPARAM(lParam);

				switch (uMsg) {
					case WM_LBUTTONDBLCLK: button = QK_Key_LeftMouseBtn;	break;
					case WM_RBUTTONDBLCLK: button = QK_Key_RightMouseBtn;	break;
					case WM_MBUTTONDBLCLK: button = QK_Key_MidMouseBtn;		break;

					case WM_XBUTTONDBLCLK: 
						button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? QK_Key_ThumbForward : QK_Key_ThumbBack;
				}

				Input::UpdateMousePos(PosX, PosY);
				MouseButtonDblClickEvent event(button, PosX, PosY);
				EventDispatcher::DispatchEvent(event);

				return (LRESULT)0;
			}

			case WM_MOUSEMOVE: {
				float PosX = (float)GET_X_LPARAM(lParam), PosY = (float)GET_Y_LPARAM(lParam);
				float PrevX = Input::MouseCurrentX(), PrevY = Input::MouseCurrentY();

				if (windowNative->m_CursorLeftWindow) {
					Input::UpdateMousePos(PosX, PosY);
					windowNative->m_CursorLeftWindow = false;
					return (LRESULT)0;
				}

				if (PrevX == PosX && PrevY == PosY)
					return (LRESULT)0;

				MouseMovedEvent event(PrevX, PrevY, PosX, PosY);
				EventDispatcher::DispatchEvent(event);

				if (windowNative->m_CursorLocked) {
					POINT cursorPos = { static_cast<int>(PrevX), static_cast<int>(PrevY) };

					ClientToScreen(hwnd, &cursorPos);
					SetCursorPos(cursorPos.x, cursorPos.y);

					return (LRESULT)0;
				}

				Input::UpdateMousePos(PosX, PosY);

				return (LRESULT)0;
			}

			case WM_MOUSEWHEEL: {
				MouseScrolledEvent event(GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
				EventDispatcher::DispatchEvent(event);
				return (LRESULT)0;
			}

			case WM_MOUSELEAVE: {
				windowNative->m_CursorLeftWindow = true;
				return (LRESULT)0;
			}
			
			case WM_SIZE: {
				uint16_t width = LOWORD(lParam), height = HIWORD(lParam);

				window->m_Width  = width;
				window->m_Height = height;
				window->m_Maximized = (wParam == SIZE_MAXIMIZED);

				WindowResizeEvent event(width, height);
				EventDispatcher::DispatchEvent(event);

				return (LRESULT)0;
			}

			case WM_SIZING: {
				window->m_Maximized = false;
				return TRUE;
			}

			case WM_MOVE: {
				window->m_PosX = GET_X_LPARAM(lParam);
				window->m_PosY = GET_Y_LPARAM(lParam);

				WindowMoveEvent event(window->m_PosX, window->m_PosY);
				EventDispatcher::DispatchEvent(event);

				return (LRESULT)0;
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void WindowsWindow::AdjustWindowSizeForDPI(uint16_t& width, uint16_t& height) const {
		RECT rect = { 0, 0, width, height };

		QK_ASSERTEX(
			AdjustWindowRectExForDpi(&rect, m_WindowStyle, false, m_WindowExStyle, GetDpiForSystem()),
			"Failed to Adjust window rectangle for client area in dpi aware window!"
		);

		width  = static_cast<uint16_t>(rect.right  - rect.left);
		height = static_cast<uint16_t>(rect.bottom - rect.top);
	}

}


#endif // QK_PLATFORM_WINDOWS
