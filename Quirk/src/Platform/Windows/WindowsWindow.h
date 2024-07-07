
#pragma once

#ifdef QK_PLATFORM_WINDOWS

#include <bitset>

namespace Quirk {

	class WindowsWindow : public Window{
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
		};

	public:
		static void Init(HINSTANCE hInstance);

	public:
		WindowsWindow(const WindowProps& props);

		virtual void OnUpdate();

		inline virtual unsigned int GetWidth() const override { return m_Data.Width; }
		inline virtual unsigned int GetHeight() const override { return m_Data.Height; }

		inline virtual void* GetNativeWindow() override { return m_WindowHandle; }

	private:
		static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		static HINSTANCE s_HInstance;

		static std::bitset<256> s_KeyPressed;
		static std::bitset<256> s_KeyRepeat;

	private:
		HWND m_WindowHandle;
		WindowData m_Data;

	};

}


#endif // QK_PLATFORM_WINDOWS
