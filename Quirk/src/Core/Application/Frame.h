

#pragma once

#include "Window.h"
#include "Core/Input/Events.h"
#include "Core/Imgui/ImguiUI.h"
#include "Core/Renderer/GraphicalContext.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"
#include "Core/Renderer/RenderCommands.h"

#include <utility>

namespace Quirk {

	class Frame;

	class TitleBar {
		friend class FrameManager;
		friend class Frame;

	public:
		TitleBar()          = default;
		virtual ~TitleBar() = default;

		virtual void OnImguiUiUpdate()     = 0;
		virtual bool OnEvent(Event& event) = 0;

		Window& GetWindow() noexcept;
		inline Frame* GetParentFrame() noexcept { return m_ParentFrame; }

		inline void SetCursorOverMinimiseButton(bool toggle) noexcept { GetWindow().SetCursorOverMinimiseButton(toggle); }
		inline void SetCursorOverMaximiseButton(bool toggle) noexcept { GetWindow().SetCursorOverMaximiseButton(toggle); }
		inline void SetCursorOverCloseButton(bool toggle)    noexcept { GetWindow().SetCursorOverCloseButton(toggle);    }

	private:
		inline void OnUiUpdate() {
			if (ImGui::BeginMainMenuBar()) {
				OnImguiUiUpdate();

				// telling window if it can move with cursor (when dragging titlebar with mouse)
				bool itemNotHovered = ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered();
				GetWindow().SetCanMoveWithCursor(itemNotHovered);

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

		Window& GetWindow() noexcept;
		inline Frame* GetParentFrame() noexcept { return m_ParentFrame; }

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
		void UpdateFrames();

		// right now HandleEvent called only when window is updated so no need to set context here
		// as window is updated only after setting the proper current contexts

		bool HandleEvent(Event& event);

		template<FrameType T, typename ...Args>
		inline void AddFrame(Args&& ...args) {
			m_Frames.push_back(static_cast<Frame*>(new T(std::forward<Args>(args)...)));
		}

		/*inline void RemoveFrame(Frame* layer) {

		}*/

	private:
		std::vector<Frame*> m_Frames;
	};

}
