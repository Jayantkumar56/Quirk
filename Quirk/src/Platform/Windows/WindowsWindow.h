
#pragma once

#ifdef QK_PLATFORM_WINDOWS

#include "Core/Window.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

	class WindowsWindow : public Window{
		struct WindowData {
			HWND WindowHandle;
			uint16_t WindWidth, WindHeight;
			uint16_t ClientWidth, ClientHeight;

			std::function<void(Event&)> EventCallbackFn;
		};

	public:
		static void Init(HINSTANCE hInstance);

	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		virtual void OnUpdate();

		virtual inline uint16_t GetWidth() const override { return m_Data.ClientWidth; }
		virtual inline uint16_t GetHeight() const override { return m_Data.ClientHeight; }
		virtual inline uint16_t GetWindWidth() const override { return m_Data.WindWidth; }
		virtual inline uint16_t GetWindHeight() const override { return m_Data.WindHeight; }

		virtual inline void* GetNativeWindow() override { return m_Data.WindowHandle; }
		virtual inline void SetEventCallback(std::function<void(Event&)> fun) override { m_Data.EventCallbackFn = fun; }

	private:
		static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		void AdjustClientArea(DWORD windowStyle, DWORD windowExStyle);

	private:
		static HINSTANCE s_HInstance;

	private:
		std::wstring Title;
		WindowData m_Data;
		GraphicalContext* m_Context;
	};

}


#endif // QK_PLATFORM_WINDOWS
