

#include "Quirk.h"
#include "FontManager.h"


namespace Quirk {

	std::unordered_map<uint16_t, ImFont*> FontManager::s_Fonts;
	std::unordered_map<std::string, ImFont*> FontManager::s_FontsWithName;

	std::string_view FontManager::GetFontPathWithWeight(FontWeight weight){
		switch (weight) {
			case Quirk::Regular:	return "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Regular.ttf";
			case Quirk::Medium:		return "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Medium.ttf";
			case Quirk::SemiBold:	return "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-SemiBold.ttf";
			case Quirk::Bold:		return "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Bold.ttf";
			case Quirk::ExtraBold:	return "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-ExtraBold.ttf";
			case Quirk::Black:		return "assets/Fonts/Schibsted_Grotesk/static/SchibstedGrotesk-Black.ttf";
		}

		QK_ASSERT(false, "Invalid Font Weight specified!");
		return "ERROR: Invalid Font Weight Selected!";
	}

	void FontManager::LoadFonts() {
		ImGuiIO& io = ImGui::GetIO();

		LoadFont(io, FontWeight::Regular, 22);
		LoadFont(io, FontWeight::Bold, 18);
		LoadFont(io, FontWeight::Regular, 16, "DragFloatValue");
		LoadFont(io, FontWeight::SemiBold, 20, "PropertyLabel");
		LoadFont(io, FontWeight::SemiBold, 22, "ComponentTreeNode");
	}

	ImFont* FontManager::GetFont(FontWeight weight, uint16_t size) {
		if (!s_Fonts.contains(weight + size)) {
			ImGuiIO& io = ImGui::GetIO();
			s_Fonts[weight + size] = io.Fonts->AddFontFromFileTTF(GetFontPathWithWeight(weight).data(), static_cast<float>(size));
		}

		return s_Fonts[weight + size];
	}

	ImFont* FontManager::GetFont(const std::string& fontName) {
		QK_ASSERT(s_FontsWithName.contains(fontName), "Required font with name %s do not exist", fontName);

		return s_FontsWithName[fontName];
	}

}
