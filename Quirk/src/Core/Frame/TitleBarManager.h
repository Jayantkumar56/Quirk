

#pragma once

#include "TitleBar.h"
#include "FrameInitContext.h"


namespace Quirk::Internals {

    class TitleBarManager {
    public:
        TitleBarManager(const FrameInitContext& initContext, auto& frame) noexcept {}

        template<TitleBarType T, typename ...Args>
        void SetTitleBar(auto* frame, Args&& ... args) {
            // TODO: refactor it for proper error logging and checking

            if constexpr (std::is_constructible_v<T, decltype(frame), Args&&...>) {
                m_TitleBar = Scope<TitleBar>(static_cast<TitleBar*>(new T(frame, std::forward<Args>(args)...)));
            }
            else {
                m_TitleBar = Scope<TitleBar>(static_cast<TitleBar*>(new T(std::forward<Args>(args)...)));
            }

            m_TitleBar->m_Window = frame->GetWindow();
        }

    protected:
        inline void UpdateTitleBarUI() {
            m_TitleBar->OnUiUpdate();
        }

        inline bool TitleBarHandleEvents(Event& event) {
            return m_TitleBar->OnEvent(event);
        }

    private:
        Scope<TitleBar> m_TitleBar;
    };

}
