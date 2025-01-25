

#pragma once

#include "Window.h"
#include "Core/Input/Events.h"
#include "Core/Imgui/ImguiUI.h"
#include "Core/Renderer/GraphicalContext.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"

#include <utility>

namespace Quirk {

	class Frame;

	class TitleBar {
		friend class Frame;
		friend class FrameManager;

	public:
		TitleBar() = default;
		virtual ~TitleBar() = default;

		virtual void OnImguiUiUpdate() = 0;
		virtual bool OnEvent(Event& event) = 0;

		Window& GetWindow();
		inline Frame* GetParentFrame() { return m_ParentFrame; }

	private:
		inline void OnUiUpdate() {
			if (ImGui::BeginMainMenuBar()) {
				OnImguiUiUpdate();

				// telling window if it can move with cursor (when dragging titlebar with mouse)
				bool itemNotHovered = ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered();
				GetWindow().SetMoveWithCursor(itemNotHovered);

				ImGui::EndMainMenuBar();
			}
		}

	private:
		// to communicate with the parent Frame obj which manages this titlebar
		Frame* m_ParentFrame = nullptr;
	};

	template <typename T>
	concept TitleBarType = std::is_base_of<TitleBar, T>::value;

	class Panel {
		friend class Frame;

	public:
		Panel() = default;
		virtual ~Panel() = default;

		virtual void OnUpdate()            = 0;
		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		Window& GetWindow();
		inline Frame* GetParentFrame() { return m_ParentFrame; }

	private:
		// to communicate with the parent Frame obj which manages this panel
		Frame* m_ParentFrame = nullptr;
	};

	template <typename T>
	concept PanelType = std::is_base_of<Panel, T>::value;

	class Frame {
		friend class FrameManager;

	public:
		Frame(WindowSpecification& spec) :
				m_Window(spec),
				m_Context(std::move(GraphicalContext::Create(m_Window))),
				m_Title(std::move(spec.Title)),
				m_TitleBar(nullptr)
		{
			m_ImguiUI.Init(m_Window, m_Context);
			SetVSync(spec.VSyncOn);
		}

		virtual ~Frame() { 
			for (size_t i = 0; i < m_Panels.size(); ++i)
				delete m_Panels[i];

			if(m_Context != nullptr)
				m_Context->DestroyContext(m_Window);
		}

		// deleted both copy constructor and copy assignment, to make it non copyable
		Frame(Frame& other)					 = delete;
		Frame& operator=(const Frame& other) = delete;

		virtual void OnAttach()            = 0;
		virtual void OnDetach()            = 0;
		virtual void OnUpdate()            = 0;
		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		inline void SwapBuffer() const	     { m_Context->SwapBuffer();     }
		inline void SetVSync(int toggle)     { m_Context->SetVSync(toggle); }
		inline Window& GetWindow()		     { return m_Window;             }
		inline const std::string& GetTitle() { return m_Title;              }

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
		Window      m_Window;
		ImguiUI     m_ImguiUI;
		std::string m_Title;
		Scope<GraphicalContext> m_Context;

		TitleBar*           m_TitleBar;
		std::vector<Panel*> m_Panels;
	};

	template <typename T>
	concept FrameType = std::is_base_of<Frame, T>::value;

	class FrameManager {
	public:
		inline void UpdateFrames() {
			for (auto& frame : m_Frames) {
				frame->m_Window.OnUpdate();
				frame->OnUpdate();

				for (auto& panel : frame->m_Panels)
					panel->OnUpdate();
			}
		}

		inline void UpdateImguiUi() {
			for (auto& frame : m_Frames) {
				frame->m_ImguiUI.Begin();

				frame->OnImguiUiUpdate();
				frame->m_TitleBar->OnUiUpdate();

				for (auto& panel : frame->m_Panels)
					panel->OnImguiUiUpdate();

				frame->m_ImguiUI.End(frame->m_Context);

				frame->SwapBuffer();
			}
		}

		inline bool HandleEvent(Event& event) {
			for (auto frame : m_Frames) {
				frame->OnEvent(event);
				frame->m_TitleBar->OnEvent(event);

				for (auto& panel : frame->m_Panels)
					panel->OnEvent(event);
			}

			return false;
		}

		template<FrameType T, typename ...Args>
		inline void AddFrame(Args&& ...args) {
			T* frame = new T(std::forward<Args>(args)...);
			m_Frames.push_back(static_cast<Frame*>(frame));
		}

		/*inline void RemoveFrame(Frame* layer) {

		}*/

	private:
		std::vector<Frame*> m_Frames;
	};

}
