

#pragma once

#include "Core/Input/Events.h"

namespace Quirk {

	class FrameBase {
		friend class FrameManager;

	public:
        virtual ~FrameBase() noexcept = default;

		virtual void OnImguiUiUpdate()     { }
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

    private:
		bool		m_Running = true;
		std::string m_Title;
	};

    template <typename T>
    concept FrameType = std::derived_from<T, FrameBase>;

}
