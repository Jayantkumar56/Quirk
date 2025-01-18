

#pragma once

#include "Window.h"
#include "Core/Input/Events.h"
#include "Core/Imgui/ImguiUI.h"
#include "Core/Renderer/GraphicalContext.h"

#include <utility>

namespace Quirk {

	class Frame;

	class Panel {
		friend class Frame;

	public:
		Panel() = default;
		~Panel() {};

		virtual void OnUpdate()            = 0;
		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		inline const Frame* GetParentFrame() const { return m_ParentFrame; }

	private:
		Frame* m_ParentFrame = nullptr;
	};

	class Frame {
		friend class FrameManager;

	public:
		Frame(const WindowSpecification& spec) :
				m_Window(spec),
				m_Context(std::move(GraphicalContext::Create(m_Window)))
		{
			m_ImguiUI.Init(m_Window, m_Context);
		}

		~Frame() { 
			for (size_t i = 0; i < m_Panels.size(); ++i)
				delete m_Panels[i];

			if(m_Context != nullptr)
				m_Context->DestroyContext(m_Window); 
		}

		// deleted both copy constructor and copy assignment, to make it non copyable
		Frame(Frame& other)					 = delete;
		Frame& operator=(const Frame& other) = delete;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual void OnUpdate()            = 0;
		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		inline void SwapBuffer() const	   { m_Context->SwapBuffer();     }
		inline Window& GetWindow()		   { return m_Window;             }
		inline void SetVSync(int toggle)   { m_Context->SetVSync(toggle); }

		template<typename T, typename ...Args>
		inline void AddPanel(Args&& ... args) {
			T* panel = new T(std::forward<Args>(args)...);
			panel->m_ParentFrame = this;
			m_Panels.push_back(panel);
		}

	private:
		Window  m_Window;
		ImguiUI m_ImguiUI;
		Scope<GraphicalContext> m_Context;
		std::vector<Panel*> m_Panels;
	};

	class FrameManager {
	public:
		inline void UpdateFrames() {
			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->m_Window.OnUpdate();
				m_Frames[i]->OnUpdate();

				for (size_t j = 0; j < m_Frames[i]->m_Panels.size(); ++j)
					m_Frames[i]->m_Panels[j]->OnUpdate();
			}
		}

		inline void UpdateImguiUi() {
			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->m_ImguiUI.Begin();

				m_Frames[i]->OnImguiUiUpdate();

				for (size_t j = 0; j < m_Frames[i]->m_Panels.size(); ++j)
					m_Frames[i]->m_Panels[j]->OnImguiUiUpdate();

				m_Frames[i]->m_ImguiUI.End(m_Frames[i]->m_Context);
			}

			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->SwapBuffer();
			}
		}

		inline bool HandleEvent(Event& event) {
			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->OnEvent(event);

				for (size_t j = 0; j < m_Frames[i]->m_Panels.size(); ++j)
					m_Frames[i]->m_Panels[j]->OnEvent(event);
			}

			return false;
		}

		inline void AddFrame(Frame* frame) {
			m_Frames.push_back(frame);
		}

		/*inline void RemoveFrame(Frame* layer) {

		}*/

	private:
		std::vector<Frame*> m_Frames;
	};

}
