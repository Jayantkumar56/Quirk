

#pragma once


#include "Core/Assets/RenderAssets/Texture.h"


namespace QuirkEditor {

    class EditorResourceManager {
    public:
        EditorResourceManager() {
            m_IconRemove = Quirk::Texture2D::Create("assets/Images/remove.png");
            m_IconUpload = Quirk::Texture2D::Create("assets/Images/uploadImage.png");
        }

        inline ImTextureID GetIconRemove() noexcept { return (ImTextureID)(intptr_t)m_IconRemove->GetRendererId(); }
        inline ImTextureID GetIconUpload() noexcept { return (ImTextureID)(intptr_t)m_IconUpload->GetRendererId(); }

    private:
        Quirk::Ref<Quirk::Texture2D> m_IconRemove;
        Quirk::Ref<Quirk::Texture2D> m_IconUpload;
    };

}

