

#pragma once

#include "Quirk.h"

namespace Quirk {

	enum class ThemeName {
		DarkTheme = 0, LightTheme = 1
	};

	class Theme {
	public:
		Theme() = delete;
		~Theme() = delete;

		static inline void SetTheme(ThemeName theme) {
			switch (theme) {
				case Quirk::ThemeName::DarkTheme:		SetDarkTheme();			return;
				case Quirk::ThemeName::LightTheme:		SetLightTheme();		return;
			}
		}

	private:
		static void SetDarkTheme();
		static void SetLightTheme();
	};

}

