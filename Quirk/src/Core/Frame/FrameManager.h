

#pragma once

#include "Core/Core.h"
#include "FrameBase.h"
#include "FontManager.h"

#include <vector>
#include <string_view>

namespace Quirk {

    class FrameManager {
    public:
        ~FrameManager() {
            for (auto& frame : m_Frames) {
                frame->Terminate();
            }
        }

        template<FrameType T, typename ...Args>
        inline T* AddFrame(Args&& ...args) {
            T* frame = new T(std::forward<Args>(args)...);
            frame->Init();
            m_Frames.emplace_back(static_cast<Internals::FrameBase*>(frame));
            return frame;
        }

        inline void RemoveFrame(std::string_view frameName) {
            for (auto& frame : m_Frames) {
                if (frame->m_Title == frameName) {
                    frame->CloseFrame();
                    return;
                }
            }

            QK_WARN("Trying to remove {0} frame which doesn't exist in the manager!", frameName);
        }

        inline void UpdateFrames() {
            // if there are no frames in the application we can terminate the Application
            if (!m_Frames.size()) {
                WindowCloseEvent e;
                EventDispatcher::DispatchEvent(e);
            }

            for (size_t i = 0; i < m_Frames.size(); ++i) {
                if (!m_Frames[i]->m_Running) {
                    // on destruction titlebar and panel deletes some gpu resources 
                    // thus correct context must be set before deleting any resource
                    m_Frames[i]->MakeContextCurrent();
                    m_Frames[i]->Terminate();
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
        std::vector<Scope<Internals::FrameBase>> m_Frames;
    };

}

