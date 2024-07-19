
#pragma once

#include "Core/Renderer/GraphicalContext.h"

#include <string>
#include "Core/Input/Events.h"
#include "Core/Input/Input.h"

#include "Core/Input/KeyCodes.h"
#include "Core/Input/MouseEvents.h"
#include "Core/Input/KeyboardEvents.h"
#include "Core/Input/ApplicationEvents.h"

namespace Quirk {

	struct WindowProps {
		std::wstring Title;
		uint16_t Width;
		uint16_t Height;

		WindowProps(std::wstring title = L"Quirk Engine!!!!", uint16_t width = 1200, uint16_t height = 800) :
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

		virtual inline uint16_t GetWidth() const = 0;
		virtual inline uint16_t GetHeight() const = 0;

		virtual inline uint16_t GetWindWidth() const = 0;
		virtual inline uint16_t GetWindHeight() const = 0;

		virtual inline void* GetNativeWindow() = 0;
		virtual inline GraphicalContext* GetGraphicalContext() = 0;
		virtual inline void SetEventCallback(std::function<void(Event&)> fun) = 0;
	};

}
