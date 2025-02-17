

#pragma once

#include "TitleBar.h"
#include "Panel.h"
#include "FrameBase.h"
#include "FontManager.h"

namespace Quirk {

	class Frame : public FrameBase {
		friend class FrameManager;

	public:
		Frame(WindowSpecification& spec) : m_TitleBar(nullptr), FrameBase(spec) {
			m_ImguiUI.Init(m_Window, m_Context);
		}

		virtual ~Frame() {
			for (size_t i = 0; i < m_Panels.size(); ++i)
				delete m_Panels[i];		
		}

		inline void MakeContextCurrent() noexcept {
			m_Context->MakeContextCurrent();
			m_ImguiUI.MakeImguiUIContextCurrent();
		}
		
		// lifetime of the panel is managed by the frame
		template<PanelType P, typename ...Args>
		inline void AddPanel(Args&& ... args) {
			P* panel = new P(std::forward<Args>(args)...);
			panel->m_ParentFrame = this;
			m_Panels.push_back(static_cast<Panel*>(panel));
		}

		// lifetime of the titlebar is managed by the frame
		template<TitleBarType T, typename ...Args>
		inline void SetTitleBar(Args&& ... args) {
			m_TitleBar = static_cast<T*>(new T(std::forward<Args>(args)...));
			m_TitleBar->m_ParentFrame = this;
		}

	private:
		ImguiUI m_ImguiUI;

		TitleBar*           m_TitleBar;
		std::vector<Panel*> m_Panels;
	};

	template <typename T>
	concept FrameType = std::derived_from<T, Frame>;

	class FrameManager {
	public:
		~FrameManager() {
			for (auto frame : m_Frames)
				delete frame;
		}

		void UpdateFrames();

		// right now HandleEvent called only when window is updated so no need to set context here
		// as window is updated only after setting the proper current contexts

		bool HandleEvent(Event& event);

		template<FrameType T, typename ...Args>
		inline T* AddFrame(Args&& ...args) {
			T* frame = new T(std::forward<Args>(args)...);
			m_Frames.push_back(static_cast<Frame*>(frame));
			return frame;
		}

		/*inline void RemoveFrame(Frame* layer) {

		}*/

		ImFontAtlas* GetFontAtlas() { return m_FontManager.m_FontAtlas; }

	private:
		std::vector<Frame*> m_Frames;
		FontManager m_FontManager;
	};

}
