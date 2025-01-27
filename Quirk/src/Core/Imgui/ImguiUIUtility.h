

#pragma once

#include "imgui.h"
#include "imgui_internal.h"

namespace Quirk { namespace ImguiUIUtility {
	
	void Text(const char* text, ImFont* font);

	void TextCentered(const char* text);

	void TextWrappedCentered(const char* text);

	bool DrawFloat3(const char* label, float* data, float resetValue = 0.0f, float speed = 0.1f, float labelColumnWidth = 100.0f,
					ImFont* lableFont  = ImGui::GetIO().Fonts->Fonts[0],  ImFont* buttonFont = ImGui::GetIO().Fonts->Fonts[0],
					ImFont* valuesFont = ImGui::GetIO().Fonts->Fonts[0]);

}}

