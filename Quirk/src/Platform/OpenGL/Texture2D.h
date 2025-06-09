

#pragma once

#include "Core/RHI/Texture2D.h"


namespace Quirk::OpenGL {

    int TextureWrapToOpenglWrap      ( RHI::TextureWrap   wrap   );
    int TexutureFilterToOpenglFilter ( RHI::TextureFilter filter );

    class Texture2D final : public RHI::Texture2D {
    public:
        Texture2D(BufferView dataBuffer, const RHI::TextureSpec& specification) noexcept;
        ~Texture2D() noexcept;

        virtual uint32_t GetRendererId()         const noexcept override { return m_RendererId; }
        virtual void     Bind(uint32_t slot = 0) const noexcept override;

        virtual void SetData(
            BufferView data, 
            uint32_t   xOffset,
            uint32_t   yOffset,
            uint32_t   width,
            uint32_t   height 
        ) noexcept override;

    private:
        virtual bool Equal(const RHI::Texture2D& other) const noexcept override;

    private:
        uint32_t m_RendererId;
    };

}
