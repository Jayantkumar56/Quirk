

#pragma once

#include "Core/Imgui/ImguiUI.h"
#include "Core/Utility/View.h"

namespace Quirk {

    class ImguiContextManager {
    public:
        ImguiContextManager(auto& frame) :
                m_GraphicalContext(frame.GetGraphicalContext())
        {
            m_ImguiUI.Init(frame.GetWindow(), m_GraphicalContext.Get());
        }

        inline void MakeImguiContextCurrent() noexcept {
            m_ImguiUI.MakeImguiUIContextCurrent();
        }

        inline ImguiUI& GetImguiContext() noexcept {
            return m_ImguiUI;
        }

    private:
        ImguiUI m_ImguiUI;

        // the graphical context on which imgui context is created 
        // must live longer than the imgui context (even for proper functioning)
        Quirk::View<GraphicalContext> m_GraphicalContext;
    };

}
