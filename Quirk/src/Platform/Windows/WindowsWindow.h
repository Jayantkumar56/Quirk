
#pragma once

#ifdef QK_PLATFORM_WINDOWS

#include "Core/Application/Window.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

	class WindowsWindow : public Window{
		struct WindowData {
			HWND WindowHandle;
			uint16_t WindWidth, WindHeight;
			uint16_t ClientWidth, ClientHeight;

			GraphicalContext* Context;
			std::function<void(Event&)> EventCallbackFn;

			std::wstring Title;
			std::wstring WindClassName;
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

		inline std::wstring GetWindowClassName() const { return m_Data.WindClassName; }

		virtual inline void* GetNativeWindow() override { return m_Data.WindowHandle; }
		virtual inline GraphicalContext* GetGraphicalContext() override { return m_Data.Context; }

		virtual inline void SetEventCallback(std::function<void(Event&)> fun) override { m_Data.EventCallbackFn = fun; }

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
