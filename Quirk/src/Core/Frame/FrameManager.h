

#pragma once

#include "Core/Core.h"
#include "FrameBase.h"
#include "FontManager.h"

#include <vector>
#include <string_view>

namespace Quirk {

    class FrameManager {
    public:
        template<FrameType T, typename ...Args>
        inline T* AddFrame(Args&& ...args) {
            T* frame = new T(std::forward<Args>(args)...);
            m_Frames.emplace_back(static_cast<FrameBase*>(frame));
            return frame;
        }

        inline void RemoveFrame(std::string_view frameName) {
            for (size_t i = 0; i < m_Frames.size(); ++i) {
                if (m_Frames[i]->m_Title == frameName) {
                    m_Frames.erase(m_Frames.begin() + i);
                }
            }

            QK_WARN("Trying to remove {0} frame which doesn't exist in the manager!", frameName);
        }

        inline ImFontAtlas* GetFontAtlas() const { return m_FontManager.m_FontAtlas; }

        inline void UpdateFrames() {
            // if there are no frames in the application we can terminate the Application
            if (!m_Frames.size()) {
                WindowCloseEvent e;
                EventDispatcher::DispatchEvent(e);
            }

            for (size_t i = 0; i < m_Frames.size(); ++i) {
                // setting graphical and imgui context for currrent frame
                m_Frames[i]->MakeContextCurrent();

                // if frame->m_Runing is set false that means the frame should be deleted
                // decremented i because the (i+1)th element will now be (i)th element which needs to be processed
                // continued this iteration as there might be no next element in the vector
                if (!m_Frames[i]->m_Running) {
                    m_Frames.erase(m_Frames.begin() + i--);
                    continue;
                }


                m_Frames[i]->UpdateFrame();
            }
        }

        // right now HandleEvent called only when window is updated so no need to set context here
        // as window is updated only after setting the proper current contexts

        inline bool HandleEvent(Event& event) {
            for (auto& frame : m_Frames) {
                frame->OnEvent(event);
            }

            return false;
        }

    private:
        FontManager m_FontManager;
        std::vector<Scope<FrameBase>> m_Frames;
    };

}

