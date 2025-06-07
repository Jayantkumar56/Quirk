

#pragma once

#include "Core/Frame/ImguiUI.h"
#include "Core/Utility/View.h"
#include "FontManager.h"
#include "FrameInitContext.h"

namespace Quirk::Internals {

    class ImguiContextManager {
    public:
        ImguiContextManager(const FrameInitContext& initContext, auto& frame) :
                m_GraphicalContext ( frame.GetGraphicalContext()                               ),
                m_ImguiContext     ( CreateImguiContext(frame.GetWindow(), m_GraphicalContext) ),
                m_FontManager      ( ImGui::GetIO().Fonts                                      )
        {}

        inline void MakeImguiContextCurrent() noexcept {
            m_ImguiContext.MakeImguiContextCurrent();
        }

        inline ImguiContext& GetImguiContext() noexcept {
            return m_ImguiContext;
        }

        inline View<FontManager> GetFontManager() noexcept {
            return &m_FontManager;
        }

        // will get called between Imgui::Begin() and Imgui::End()
        virtual void OnImguiUiUpdate() = 0;

    private:
        inline ImguiContext CreateImguiContext(View<Window> window, View<GraphicalContext> graphicalContext) {
            ImguiContext context;
            context.Init(window, graphicalContext);
            return context;
        }

    private:
        // the graphical context on which imgui context is created 
        // must live longer than the imgui context 
        View<GraphicalContext> m_GraphicalContext;

        ImguiContext m_ImguiContext;
        FontManager  m_FontManager;
    };

}
