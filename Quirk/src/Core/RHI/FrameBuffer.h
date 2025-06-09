

#pragma once

#include "Core/Core.h"
#include "Core/RHI/Texture2D.h"


namespace Quirk::RHI {

    enum class FrameBufferTextureType : uint8_t {
        None = 0,

        // color attachments
        RGBA_8,
        RGB_8,
        RED_INTEGER,

        // depth and stencile attachments
        DEPTH_24_STENCIL_8
    };

    static inline bool IsFrameBufferColorAttachment(FrameBufferTextureType type) {
        bool isColorType = type == FrameBufferTextureType::RGBA_8;
        isColorType = isColorType || (type == FrameBufferTextureType::RGB_8);
        isColorType = isColorType || (type == FrameBufferTextureType::RED_INTEGER);
        return isColorType;
    }

    static inline bool IsFrameBufferDepthAttachment(FrameBufferTextureType type) {
        bool isDepthType = type == FrameBufferTextureType::DEPTH_24_STENCIL_8;
        return isDepthType;
    }

    union ClearAttachmentData {
        float RGBA[4];
        int	  RedInteger;
        float DepthValue;
    };

    struct FrameBufferAttachmentSpecification {
        FrameBufferTextureType Type      = FrameBufferTextureType::None;
        ClearAttachmentData    ClearData = { 0.0f, 0.0f, 0.0f, 1.0f };
        TextureWrap			   WrapMode  = TextureWrap::Repeat;
        TextureFilter MinificationFilter = TextureFilter::Linear;
        TextureFilter MagficationFilter  = TextureFilter::Linear;
    };

    struct FrameBufferSpecification {
        uint32_t Width = 0, Height = 0;
    };

    class FrameBuffer {
    public:
        virtual ~FrameBuffer() noexcept = default;

        virtual void Bind()   noexcept = 0;
        virtual void Unbind() noexcept = 0;

        virtual void ClearAttachments() = 0;
        virtual void SetAttachments(std::initializer_list<FrameBufferAttachmentSpecification> attachmentsSpec) = 0;

        virtual uint32_t GetDepthStencilAttachment()      const noexcept = 0;
        virtual uint32_t GetColorAttachment(size_t index) const noexcept = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual void GetColorPixelData(size_t index, int x, int y, int width, int height, void* outputData, int size) = 0;
    };

}
