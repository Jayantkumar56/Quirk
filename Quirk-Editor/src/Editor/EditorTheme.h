

#pragma once

#include "Quirk.h"

namespace QuirkEditor {

	enum class ThemeName {
		DarkTheme = 0, LightTheme = 1
	};

	enum class ColorName {
		PopupBorder, DarkText, DropdownButton
	};

	class EditorTheme {
	public:
		inline void SetTheme(ThemeName theme) {
			switch (theme) {
				case ThemeName::DarkTheme:  SetDarkTheme();	 return;
				case ThemeName::LightTheme: SetLightTheme();  return;
			}
		}

		ImVec4 GetColor(ColorName color);

	private:
		void SetDarkTheme();
		void SetLightTheme();
	};

}

