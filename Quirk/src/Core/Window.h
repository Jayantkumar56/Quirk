
#pragma once

#include <string>


namespace Quirk {

	struct WindowProps {
		std::wstring Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(std::wstring title = L"Quirk Engine!!!!", unsigned int width = 1200, unsigned int height = 800) :
				Title(title), 
				Width(width), 
				Height(height) 
		{}
	};

	class  Window {
	public:
		static Window* Create(const WindowProps& props = WindowProps());

	public:
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void* GetNativeWindow() = 0;

	};

}
