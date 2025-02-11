

#pragma once


namespace Quirk {

	constexpr uint16_t FontSizeLimit = 10000;

	// weight and size could be combined for keys in fonts map
	enum FontWeight : uint16_t {
		Regular		= 0,
		Medium		= 1,
		SemiBold	= 2,
		Bold		= 3,
		ExtraBold	= 4,
		Black		= 5,
		Count
	};

	class FontManager {
		// only a single instance of the class would be created by the FrameManager thus constructor is private
		friend class FrameManager;

	public:
		static inline ImFont* GetFont(FontWeight weight, uint16_t size) {
			return s_FontManager->GetFontHelper(weight, size);
		}

		static inline ImFont* GetFont(const std::string& fontName) {
			return s_FontManager->GetFontHelper(fontName);
		}

		static inline void SetFontFileToFontWeight(FontWeight weight, std::string_view filePath) {
			s_FontManager->SetFontFileToFontWeightHelper(weight, filePath);
		}

		static inline void LoadFont(FontWeight weight, uint16_t size, const char* name = nullptr) {
			s_FontManager->LoadFontHelper(weight, size, name);
		}

	private:
		FontManager() : m_FontAtlas(new ImFontAtlas()) { s_FontManager = this; }
		~FontManager()  { delete m_FontAtlas; }

		// returns the font with provided weight and size (if loaded with respective weight and size before calling)
		// should not be called in any **::ImGuiUiUpdate(), when font is not preloaded
		// since io.Fonts->AddFontFromFileTTF could not be called between ImGui::NewFrame() and ImGui::EndFrame/Render()
		ImFont* GetFontHelper(FontWeight weight, uint16_t size) {
			int key = (weight * FontSizeLimit) + size;

			if (!m_Fonts.contains(key)) {
				m_Fonts[key] = m_FontAtlas->AddFontFromFileTTF(m_FontFilePath[weight].data(), static_cast<float>(size));
			}

			return m_Fonts[key];
		}

		// returns the font with provided name (if loaded with respective name before calling)
		ImFont* GetFontHelper(const std::string& fontName) {
			QK_ASSERT(m_FontsWithName.contains(fontName), "Requested font with name %s do not exist", fontName);
			return m_FontsWithName[fontName];
		}

		// sets a particular font file path to a particular FontWeight
		// this file will later be use to load font of that particular FontWeight
		void SetFontFileToFontWeightHelper(FontWeight weight, std::string_view filePath) {
			m_FontFilePath[weight] = filePath;
		}

		// loads font with weight and size name is optional if provided then the font could be retreived by name also
		inline void LoadFontHelper(FontWeight weight, uint16_t size, const char* name = nullptr) {
			int key      = (weight * FontSizeLimit) + size;
			m_Fonts[key] = m_FontAtlas->AddFontFromFileTTF(m_FontFilePath[weight].data(), (float)size);

			if (name != nullptr) {
				m_FontsWithName[name] = m_Fonts[key];
			}
		}

	private:
		// a common ImFontAtlas would be shared in the whole Application 
		ImFontAtlas* m_FontAtlas;

		std::unordered_map<uint16_t, ImFont*>           m_Fonts;
		std::unordered_map<std::string, ImFont*>        m_FontsWithName;
		std::array<std::string_view, FontWeight::Count> m_FontFilePath;

		// only a single instance of the class would be created by the FrameManager
		static inline FontManager* s_FontManager;
	};

}
