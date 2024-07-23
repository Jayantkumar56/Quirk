
#pragma once

#ifdef QK_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Core/Input/Input.h"
#include "Core/Input/Events.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Input/KeyboardEvents.h"
#include "Core/Input/ApplicationEvents.h"

namespace Quirk {

	class Window{
		struct WindowData {
			HWND WindowHandle;
			uint16_t WindWidth, WindHeight;
			uint16_t ClientWidth, ClientHeight;
			std::function<void(Event&)> EventCallbackFn;
			std::wstring Title;
			std::wstring WindClassName;
		};

	public:
		static void Init(HINSTANCE hInstance);

	public:
		Window(const std::wstring title, uint16_t width, uint16_t height);
		~Window() = default;

		void OnUpdate();

		inline uint16_t GetWidth() const { return m_Data.ClientWidth; }
		inline uint16_t GetHeight() const { return m_Data.ClientHeight; }
		inline uint16_t GetWindWidth() const { return m_Data.WindWidth; }
		inline uint16_t GetWindHeight() const { return m_Data.WindHeight; }

		inline std::wstring GetWindowClassName() const { return m_Data.WindClassName; }

		inline void* GetNativeWindow() { return m_Data.WindowHandle; }
		inline void SetEventCallback(std::function<void(Event&)> fun) { m_Data.EventCallbackFn = fun; }

	private:
		static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		void AdjustClientArea(DWORD windowStyle, DWORD windowExStyle);

	private:
		static HINSTANCE s_HInstance;

	private:
		WindowData m_Data;
	};

}


#endif // QK_PLATFORM_WINDOWS
