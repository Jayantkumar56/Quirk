

#include "Qkpch.h"

#ifdef QK_PLATFORM_WINDOWS
#include "Core/Application/Application.h"
#include "Core/Input/Input.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Quirk {

	void Cursor::ShowCursor() {
		while (::ShowCursor(TRUE) < 0);
	}

	void Cursor::HideCursor(){
		while (::ShowCursor(FALSE) > -1);
	}

	void Cursor::LockCursor() {
		Application::Get().GetWindow().SetCursorLocked(true);
	}

	void Cursor::UnlockCursor() {
		Application::Get().GetWindow().SetCursorLocked(false);
	}

	void Cursor::SetCursorPosition(float x, float y){
		SetCursorPos(static_cast<int>(x), static_cast<int>(y));
		Input::UpdateMousePos(x, y);
	}

	void Cursor::PlaceAtCenter() {
		auto& window = Application::Get().GetWindow();

		int x = window.GetWindPosX() + window.GetWidth() / 2;
		int y = window.GetWindPosY() + window.GetHeight() / 2;

		SetCursorPos(x, y);
		Input::UpdateMousePos(static_cast<float>(x), static_cast<float>(y));
	}

}


#endif // QK_PLATFORM_WINDOWS

