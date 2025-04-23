

#pragma once

#include "Core/Core.h"
#include "FrameBase.h"
#include "FontManager.h"
#include "Core/Renderer/RenderCommands.h"

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

        inline FrameBase*   GetCurrentFrame()    { return m_CurrentFrame;            }
        inline ImFontAtlas* GetFontAtlas() const { return m_FontManager.m_FontAtlas; }

        template<FrameType T>
        inline T* GetCurrentFrameAs() { return static_cast<T*>(m_CurrentFrame); }

        void UpdateFrames() {
            // if there are no frames in the application we can terminate the Application
            if (!m_Frames.size()) {
                WindowCloseEvent e;
                EventDispatcher::DispatchEvent(e);
            }

            for (size_t i = 0; i < m_Frames.size(); ++i) {
                m_CurrentFrame = m_Frames[i].get();

                // setting graphical and imgui context for currrent frame
                m_Frames[i]->MakeContextCurrent();

                // if frame->m_Runing is set false that means the frame should be deleted
                // decremented i because the (i+1)th element will now be (i)th element which needs to be processed
                // continued this iteration as there might be no next element in the vector
                if (!m_Frames[i]->m_Running) {
                    m_Frames.erase(m_Frames.begin() + i--);
                    continue;
                }

                // clearing the backbuffer
                RenderCommands::Clear();

                m_Frames[i]->UpdateFrame();
                m_Frames[i]->UpdateFrameUI();
                m_Frames[i]->SwapBuffer();
            }
        }

        // right now HandleEvent called only when window is updated so no need to set context here
        // as window is updated only after setting the proper current contexts

        bool HandleEvent(Event& event) {
            for (auto& frame : m_Frames) {
                m_CurrentFrame = frame.get();
                frame->OnEvent(event);
            }

            return false;
        }

    private:
        FrameBase* m_CurrentFrame;
        std::vector<Ref<FrameBase>> m_Frames;
        FontManager m_FontManager;
    };

}

