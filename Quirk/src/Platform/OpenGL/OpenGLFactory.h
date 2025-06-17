

#pragma once

#include "Core/RHI/Factory.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLGraphicalContext.h"
#include "OpenGLTexture2D.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexArray.h"
#include "OpenGLRenderCommand.h"
#include "OpenGLShader.h"


namespace Quirk::OpenGL {

    class Factory : public RHI::Factory {
    public:
        virtual Scope<RHI::GraphicalContext> CreateGraphicalContext(View<Window> window) const noexcept override {
            return CreateScope<OpenGL::GraphicalContext>(window);
        }

        virtual Scope<RHI::FrameBuffer> CreateFrameBuffer(const RHI::FrameBufferSpecification& spec) const noexcept override {
            return CreateScope<OpenGL::FrameBuffer>(spec);
        }

        virtual Ref<RHI::Texture2D> CreateTexture(BufferView dataBuffer, const RHI::TextureSpec& specification) const noexcept override {
            return CreateRef<OpenGL::Texture2D>(dataBuffer, specification);
        }

        virtual Ref<RHI::VertexBuffer> CreateVertexBuffer() const noexcept override {
            return CreateRef<OpenGL::VertexBuffer>();
        }

        virtual Ref<RHI::VertexBuffer> CreateVertexBuffer(BufferView buffer) const noexcept override {
            return CreateRef<OpenGL::VertexBuffer>(buffer);
        }

        virtual Ref<RHI::VertexBuffer> CreateVertexBuffer(uint32_t size) const noexcept override {
            return CreateRef<OpenGL::VertexBuffer>(size);
        }

        virtual Ref<RHI::IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) const noexcept override {
            return CreateRef<OpenGL::IndexBuffer>(indices, count);
        }

        virtual Ref<RHI::VertexArray> CreateVertexArray() const noexcept override {
            return CreateRef<OpenGL::VertexArray>();
        }

        virtual Scope<RHI::RenderCommand> CreateRenderCommandContext() const noexcept override {
            return CreateScope<OpenGL::RenderCommand>();
        }

        virtual Scope<RHI::Shader> CreateShader(ShaderSource shaderSrc) const noexcept override {
            return CreateScope<OpenGL::Shader>(shaderSrc);
        }

    };

}
