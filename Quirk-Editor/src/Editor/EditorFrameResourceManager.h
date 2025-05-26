

#pragma once


#include "Core/Assets/RenderAssets/Texture.h"

#include "imgui.h"


namespace QuirkEditor {

    class EditorFrameResourceManager {
    public:
        EditorFrameResourceManager() {
            InitSceneViewportPanelIcons();
            InitInspectorPanelIcons();
            InitContentBrowserPanelIcons();
        }

        // ---------------------------------------------------------------------------------------------------------------------
        // Inspector Panel Icons

        inline ImTextureID GetIconPlay()  const noexcept { return (ImTextureID)(intptr_t)m_IconPlay->GetRendererId();  }
        inline ImTextureID GetIconPause() const noexcept { return (ImTextureID)(intptr_t)m_IconPause->GetRendererId(); }

        // ---------------------------------------------------------------------------------------------------------------------
        // Inspector Panel Icons

        inline ImTextureID GetIconRemove() const noexcept { return (ImTextureID)(intptr_t)m_IconRemove->GetRendererId(); }
        inline ImTextureID GetIconUpload() const noexcept { return (ImTextureID)(intptr_t)m_IconUpload->GetRendererId(); }

        // ---------------------------------------------------------------------------------------------------------------------
        // Content Browser Panel

        inline ImTextureID GetIconFile()     const noexcept { return (ImTextureID)(intptr_t)m_IconFile->GetRendererId();     }
        inline ImTextureID GetIconFolder()   const noexcept { return (ImTextureID)(intptr_t)m_IconFolder->GetRendererId();   }
        inline ImTextureID GetIconBackward() const noexcept { return (ImTextureID)(intptr_t)m_IconBackward->GetRendererId(); }
        inline ImTextureID GetIconForward()  const noexcept { return (ImTextureID)(intptr_t)m_IconForward->GetRendererId();  }
        inline ImTextureID GetIconRefresh()  const noexcept { return (ImTextureID)(intptr_t)m_IconRefresh->GetRendererId();  }

    private:
        void InitSceneViewportPanelIcons() {
            m_IconPlay  = Quirk::TextureImporter::CreateFromImage( "assets/Images/play.png"  );
            m_IconPause = Quirk::TextureImporter::CreateFromImage( "assets/Images/pause.png" );
        }

        void InitInspectorPanelIcons() {
            m_IconRemove = Quirk::TextureImporter::CreateFromImage( "assets/Images/remove.png"      );
            m_IconUpload = Quirk::TextureImporter::CreateFromImage( "assets/Images/uploadImage.png" );
        }

        void InitContentBrowserPanelIcons() {
			m_IconFile     = Quirk::TextureImporter::CreateFromImage( "assets/Images/file.png"           );
            m_IconFolder   = Quirk::TextureImporter::CreateFromImage( "assets/Images/folder.png"         );
            m_IconBackward = Quirk::TextureImporter::CreateFromImage( "assets/Images/backward_arrow.png" );
			m_IconForward  = Quirk::TextureImporter::CreateFromImage( "assets/Images/forward_arrow.png"  );
			m_IconRefresh  = Quirk::TextureImporter::CreateFromImage( "assets/Images/refresh.png"        );
        }

    private:
        // -------------------------------------------
        // Scene ViewPort Icons

        Quirk::Ref<Quirk::Texture2D> m_IconPlay;
        Quirk::Ref<Quirk::Texture2D> m_IconPause;

        // -------------------------------------------
        // Inspector Panel Icons

        Quirk::Ref<Quirk::Texture2D> m_IconRemove;
        Quirk::Ref<Quirk::Texture2D> m_IconUpload;

        // -------------------------------------------
        // Content Browser Panel

        Quirk::Ref<Quirk::Texture2D> m_IconFile;
        Quirk::Ref<Quirk::Texture2D> m_IconFolder;
        Quirk::Ref<Quirk::Texture2D> m_IconBackward;
        Quirk::Ref<Quirk::Texture2D> m_IconForward;
        Quirk::Ref<Quirk::Texture2D> m_IconRefresh;
    };

}

