

#pragma once

#include "Core/Input/Events.h"

namespace Quirk {

    struct FrameContext;
    class  FrameManager;
}

namespace Quirk::Internals {

	class FrameBase {
		friend class ::Quirk::FrameManager;

	public:
        virtual ~FrameBase() noexcept = default;

        // called once when the frame is being created (after the constructor is called)
        virtual void Init()      noexcept = 0;

        // called once when the frame is being deleted (before the destructor is called)
        virtual void Terminate() noexcept = 0;

		virtual void OnUpdate()            { }
        // will get called between Imgui::Begin() and Imgui::End()
        //virtual void OnImguiUiUpdate() = 0;
		virtual bool OnEvent(Event& event) { return false; }

		// since frames are managed by the frame manager 
        // thus just by setting m_Running to false
		// will make the frame manager close this frame
		inline void CloseFrame() noexcept { m_Running = false; }

		inline std::string_view GetTitle() const noexcept { return m_Title; }

        virtual void MakeContextCurrent() = 0;

    protected:
        virtual void UpdateFrame()             = 0;
        virtual bool HandleEvent(Event& event) = 0;
        virtual void GetFrameContext(FrameContext& outFrameContext) = 0;

    private:
		bool		m_Running = true;
		std::string m_Title;
	};

}

namespace Quirk {

    template <typename T>
    concept FrameType = std::derived_from<T, Internals::FrameBase>;

}
