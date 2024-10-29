

#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS

// to provide imgui with the event data
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "Core/Core.h"
#include "WindowsWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <WindowsX.h>

namespace Quirk {

	HINSTANCE Window::s_HInstance = nullptr;

    void Window::Init(HINSTANCE hInstance) {
		QK_CORE_ASSERT(hInstance, "Didn't got initial HINSTANCE from windows api!");
		s_HInstance = hInstance;

		// to maintin uniformity in window size among different dpi monitors
		QK_CORE_ASSERTEX(
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2), 
			"Unable to set Dpi Awareness to DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2"
		);
    }

	Window::Window(const std::wstring& title, uint16_t width, uint16_t height, RendererAPI::API rendererAPI) :
			m_Data({ 
				nullptr, 
				WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE,
				WS_EX_ACCEPTFILES,
				width, height, 
				width, height,
				0, 0,
				nullptr,
				title, 
				L"Quirk",
				false,
				false
			}),
			m_Context(GraphicalContext::Create(rendererAPI))
	{
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
		wc.lpszClassName	= m_Data.WindClassName.c_str();
		wc.hIconSm			= 0;

		QK_CORE_ASSERTEX(RegisterClassExW(&wc), "Failed to Register the window class! {0}", GetLastError());

		AdjustWindowArea();

		int posX = ((GetSystemMetrics(SM_CXSCREEN) - m_Data.WindWidth) / 2);
		int posY = ((GetSystemMetrics(SM_CYSCREEN) - m_Data.WindHeight) / 2);
		
		m_Data.WindowHandle = CreateWindowExW(
			m_Data.WindowExStyle,					// The window accepts drag-drop files.
			m_Data.WindClassName.c_str(),			// Window class
			m_Data.Title.c_str(),					// Window text
			m_Data.WindowStyle,						// Window style
			posX, posY,								// Postion of window on the screen
			m_Data.WindWidth, m_Data.WindHeight,	// height and width of the window
			NULL,									// Parent window    
			NULL,									// Menu
			s_HInstance,							// Instance handle
			NULL									// Additional application data
		);

		QK_CORE_ASSERT(m_Data.WindowHandle, "Failed to create Window handle!");

		// putting this Window pointer into created HWND
		SetPropW(m_Data.WindowHandle, L"wndptr", this);

		m_Context->Init(*this);

		// Updating window client area pos
		POINT pos = { 0, 0 };
		ClientToScreen(m_Data.WindowHandle, &pos);

		m_Data.PosX = pos.x;
		m_Data.PosY = pos.y;

		SetActiveWindow(m_Data.WindowHandle);

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		QK_CORE_ASSERT(SUCCEEDED(hr), "Unable to Initialize COM!");
	}

	Window::~Window() {
		CoUninitialize();

		m_Context->DestroyContext(*this);
		delete m_Context;
		DestroyWindow(m_Data.WindowHandle);
	}

	void Window::OnUpdate() {
		MSG msg;
		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::SetVSync(bool toggle) {
		if (m_Data.IsVSyncOn && !toggle) {
			m_Context->SetVSync(0);
		}
		else if (!m_Data.IsVSyncOn && toggle) {
			m_Context->SetVSync(1);
		}

		m_Data.IsVSyncOn = toggle;
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Window* window = (Window*)GetPropW(hwnd, L"wndptr");
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		if (!window) return DefWindowProc(hwnd, uMsg, wParam, lParam);

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
				window->m_Data.EventCallbackFn(event);

				return (LRESULT)0;
			}

			case WM_MOUSEMOVE: {
				float PosX = (float)GET_X_LPARAM(lParam), PosY = (float)GET_Y_LPARAM(lParam);
				float PrevX = Input::MouseCurrentX(), PrevY = Input::MouseCurrentY();

				if (window->m_Data.CursorLeftWindow) {
					Input::UpdateMousePos(PosX, PosY);
					window->m_Data.CursorLeftWindow = false;
					return (LRESULT)0;
				}

				if (PrevX == PosX && PrevY == PosY)
					return (LRESULT)0;

				MouseMovedEvent event(PrevX, PrevY, PosX, PosY);
				window->m_Data.EventCallbackFn(event);

				if (window->m_Data.CursorLocked) {
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
				window->m_Data.EventCallbackFn(event);
				return (LRESULT)0;
			}

			case WM_MOUSELEAVE: {
				window->m_Data.CursorLeftWindow = true;
				return (LRESULT)0;
			}
			
			case WM_SIZE: {
				uint16_t width = LOWORD(lParam), height = HIWORD(lParam);

				window->m_Data.ClientWidth = width;
				window->m_Data.ClientHeight = height;

				window->AdjustWindowArea();

				WindowResizeEvent event(width, height);
				window->m_Data.EventCallbackFn(event);

				return (LRESULT)0;
			}

			case WM_MOVE: {
				if(!window) return (LRESULT)0;

				window->m_Data.PosX = static_cast<int32_t>GET_X_LPARAM(lParam);
				window->m_Data.PosY = static_cast<int32_t>(GET_Y_LPARAM(lParam));

				WindowMoveEvent event(window->m_Data.PosX, window->m_Data.PosY);
				window->m_Data.EventCallbackFn(event);

				return (LRESULT)0;
			}
		}
		 
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void Window::AdjustWindowArea() {
		RECT rect = { 0, 0, m_Data.WindWidth, m_Data.WindHeight };

		QK_ASSERTEX(
			AdjustWindowRectExForDpi(&rect, m_Data.WindowStyle, false, m_Data.WindowExStyle, GetDpiForSystem()),
			"Failed to Adjust window rectangle for client area in dpi aware window!"
		);

		m_Data.WindWidth = static_cast<uint16_t>(rect.right - rect.left);
		m_Data.WindHeight = static_cast<uint16_t>(rect.bottom - rect.top);
	}

}


#endif // QK_PLATFORM_WINDOWS
