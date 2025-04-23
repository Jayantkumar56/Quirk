

#pragma once
#include "Core/Frame/GraphicalContext.h"
#include "Window.h"
#include "Core/Input/Events.h"

namespace Quirk {

	class FrameBase {
		friend class FrameManager;

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
			}
		}

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

        virtual void MakeContextCurrent() = 0;

    protected:
        virtual void UpdateFrame()   = 0;
        virtual void UpdateFrameUI() = 0;
        virtual bool HandleEvent(Event& event) = 0;

        inline GraphicalContext* GetGraphicalContext() noexcept { return m_Context.get(); }

    private:
		bool		m_Running = true;
		Window      m_Window;
		std::string m_Title;

        Scope<GraphicalContext> m_Context;
	};

    template <typename T>
    concept FrameType = std::derived_from<T, FrameBase>;

}
