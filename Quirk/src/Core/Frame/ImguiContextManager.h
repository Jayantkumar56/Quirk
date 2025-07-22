

#pragma once

#include "Core/Frame/ImguiUI.h"
#include "Core/Utility/View.h"
#include "FontManager.h"
#include "FrameInitContext.h"


namespace Quirk::Internals {

    class ImguiContextManager {
    public:
        ImguiContextManager(const FrameInitContext& initContext, auto& frame) noexcept :
                m_GraphicalContext ( frame.GetGraphicalContext()           ),
                m_ImguiContext     ( frame.GetWindow(), m_GraphicalContext ),
                m_FontManager      ( ImGui::GetIO().Fonts                  )
        {}

        inline void                   MakeImguiContextCurrent()        noexcept { m_ImguiContext.MakeImguiContextCurrent(); }
        inline View<ImguiContext>     GetImguiContext()                noexcept { return &m_ImguiContext;                   }
        inline View<FontManager>      GetFontManager()                 noexcept { return &m_FontManager;                    }
        inline ConstView<FontManager> GetFontManagerForRead()    const noexcept { return &m_FontManager;                    }

        // will get called between Imgui::Begin() and Imgui::End()
        virtual void OnImguiUiUpdate() = 0;

    private:
        // the graphical context on which imgui context is created 
        // must live longer than the imgui context 
        ConstView<RHI::GraphicalContext> m_GraphicalContext;

        ImguiContext m_ImguiContext;
        FontManager  m_FontManager;
    };

}
