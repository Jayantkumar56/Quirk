
#pragma once

#ifdef QK_PLATFORM_WINDOWS

namespace Quirk {

	// forward declaration of graphical context and event class
	class GraphicalContext;
	class Event;

	class Window{
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
		Window(const std::wstring title, uint16_t width, uint16_t height);
		~Window();

		void OnUpdate();

		inline uint16_t GetWidth() const { return m_Data.ClientWidth; }
		inline uint16_t GetHeight() const { return m_Data.ClientHeight; }
		inline uint16_t GetWindWidth() const { return m_Data.WindWidth; }
		inline uint16_t GetWindHeight() const { return m_Data.WindHeight; }

		inline std::wstring GetWindowClassName() const { return m_Data.WindClassName; }

		inline void* GetNativeWindow() { return m_Data.WindowHandle; }
		inline GraphicalContext* GetGraphicalContext() { return m_Data.Context; }

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
