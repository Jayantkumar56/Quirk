

#pragma once
#include "Core/Frame/GraphicalContext.h"
#include "Window.h"
#include "Core/Input/Events.h"

namespace Quirk {

	class FrameManager;
	class Frame;

	class FrameBase {
		friend class FrameManager;
		friend class Frame;

	public:
		FrameBase(WindowSpecification& spec) :
			m_Window(spec),
			m_Context(GraphicalContext::Create(m_Window)),
			m_Title(std::move(spec.Title))
		{
			m_Context->SetVSync(spec.VSyncOn);
		}

		virtual ~FrameBase() {
			if (m_Context != nullptr) {
				m_Context->DestroyContext(m_Window);
				delete m_Context;
			}
		}

		// deleted both copy constructor and copy assignment, to make it non copyable
		FrameBase(FrameBase& other)                  = delete;
		FrameBase& operator=(const FrameBase& other) = delete;

		virtual void OnUpdate()            { }
		virtual void OnImguiUiUpdate()     { }
		virtual bool OnEvent(Event& event) { return false; }

		// since frames are managed by the frame manager thus by just setting m_Running to false
		// will make the frame manager close this frame

		inline void CloseFrame() noexcept { m_Running = false; }

		inline void    SwapBuffer() const               { m_Context->SwapBuffer();     }
		inline void    SetVSync(int toggle)             { m_Context->SetVSync(toggle); }
		inline Window& GetWindow()		       noexcept { return m_Window;             }
		inline const   std::string& GetTitle() noexcept { return m_Title;              }

	private:
		bool		m_Running = true;
		Window      m_Window;
		std::string m_Title;

		GraphicalContext* m_Context;
	};

}
