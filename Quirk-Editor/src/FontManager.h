

#pragma once


namespace Quirk {

	// each weight are seperated by 1000, so weight and size could be combined for keys in fonts map
	enum FontWeight : uint16_t {
		Regular		= 0, 
		Medium		= 1000, 
		SemiBold	= 2000, 
		Bold		= 3000, 
		ExtraBold	= 4000, 
		Black		= 5000
	};

	class FontManager {
	public:
		FontManager() = delete;
		~FontManager() = delete;

		static void LoadFonts();
		static ImFont* GetFont(FontWeight weight, uint16_t size);
		static ImFont* GetFont(const std::string& fontName);

	private:
		static inline void LoadFont(ImGuiIO& io, FontWeight weight, uint16_t size, const char* name = nullptr) {
			s_Fonts[weight + size] = io.Fonts->AddFontFromFileTTF(GetFontPathWithWeight(weight).data(), static_cast<float>(size));

			if (name != nullptr) {
				s_FontsWithName[name] = s_Fonts[weight + size];
			}
		}

		static std::string_view GetFontPathWithWeight(FontWeight weight);
		static std::unordered_map<uint16_t, ImFont*> s_Fonts;
		static std::unordered_map<std::string, ImFont*> s_FontsWithName;
	};

}
