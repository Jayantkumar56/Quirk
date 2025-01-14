

#pragma once

#include "Window.h"
#include "Core/Input/Events.h"
#include "Core/Imgui/ImguiUI.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

	class Frame {
		friend class FrameManager;

	public:
		Frame(const WindowSpecification& spec) :
				m_Window(spec),
				m_Context(std::move(GraphicalContext::Create(m_Window)))
		{
			m_ImguiUI.Init(m_Window, m_Context);
		}

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual void OnUpdate()            = 0;
		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		inline void SwapBuffer() const { m_Context->SwapBuffer(); }
		inline Window& GetWindow()     { return m_Window;         }

	private:
		Window  m_Window;
		ImguiUI m_ImguiUI;
		Ref<GraphicalContext> m_Context;
	};

	class FrameManager {
	public:
		inline void UpdateFrames() {
			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->m_Window.OnUpdate();
				m_Frames[i]->OnUpdate();
			}
		}

		inline void UpdateImguiUiFrames() {
			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->m_ImguiUI.Begin();
				m_Frames[i]->OnImguiUiUpdate();
				m_Frames[i]->m_ImguiUI.End(m_Frames[i]->m_Context);
			}

			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->SwapBuffer();
			}
		}

		inline bool HandleEvent(Event& event) {
			for (size_t i = 0; i < m_Frames.size(); ++i) {
				m_Frames[i]->OnEvent(event);
			}

			return false;
		}

		inline void AddFrame(Frame* frame) {
			m_Frames.push_back(frame);
		}

		/*inline void RemoveFrame(Frame* layer) {

		}*/

		// temporary function
		inline Window& GetWindow() {
			return m_Frames[0]->GetWindow();
		}

	private:
		std::vector<Frame*> m_Frames;
	};

}
