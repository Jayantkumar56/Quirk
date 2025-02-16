

#pragma once

#include "Core/Frame/GraphicalContext.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"
#include "Core/Renderer/RenderCommands.h"

#include "TitleBar.h"
#include "Panel.h"
#include "FontManager.h"

#include <utility>

namespace Quirk {

	class Frame {
		friend class FrameManager;

	public:
		Frame(WindowSpecification& spec) :
				m_Window(spec),
				m_Context(GraphicalContext::Create(m_Window)),
				m_Title(std::move(spec.Title)),
				m_TitleBar(nullptr)
		{
			m_ImguiUI.Init(m_Window, m_Context);
			SetVSync(spec.VSyncOn);
		}

		virtual ~Frame() {
			for (size_t i = 0; i < m_Panels.size(); ++i)
				delete m_Panels[i];

			if(m_Context != nullptr) {
				m_Context->DestroyContext(m_Window);
				delete m_Context;
			}
		}

		// deleted both copy constructor and copy assignment, to make it non copyable
		Frame(Frame& other)					 = delete;
		Frame& operator=(const Frame& other) = delete;

		virtual void OnUpdate()            = 0;
		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		// since frames are managed by the frame manager thus by just setting m_Running to false
		// will make the frame manager close this frame

		inline void CloseFrame() noexcept { m_Running = false; }
		inline void MakeContextCurrent() noexcept { 
			m_Context->MakeContextCurrent();
			m_ImguiUI.MakeImguiUIContextCurrent();
		}

		inline void SwapBuffer() const	     { m_Context->SwapBuffer();     }
		inline void SetVSync(int toggle)     { m_Context->SetVSync(toggle); }
		inline Window& GetWindow()		     noexcept { return m_Window;    }
		inline const std::string& GetTitle() noexcept { return m_Title;     }

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
		bool		m_Running = true;
		Window      m_Window;
		std::string m_Title;

		GraphicalContext* m_Context;
		ImguiUI           m_ImguiUI;

		TitleBar*           m_TitleBar;
		std::vector<Panel*> m_Panels;
	};

	template <typename T>
	concept FrameType = std::is_base_of<Frame, T>::value;

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
