

#include "Qkpch.h"

#include "OpenGLRenderCommand.h"

#include "glad/glad.h"


namespace Quirk::OpenGL {

	void RenderCommand::SetClearColor(const glm::vec4& color) const noexcept {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::Clear(bool color, bool depth, bool stencil) const noexcept {
		GLbitfield mask = 0;
		mask |= color   ? GL_COLOR_BUFFER_BIT   : 0;
		mask |= depth   ? GL_DEPTH_BUFFER_BIT   : 0;
		mask |= stencil ? GL_STENCIL_BUFFER_BIT : 0;

		glClear(mask);
	}

	void RenderCommand::SetViewport(uint32_t width, uint32_t height) const noexcept {
		glViewport(0, 0, width, height);
	}

	void RenderCommand::EnableBlending() const noexcept {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RenderCommand::EnableDepthTesting() const noexcept {
		glEnable(GL_DEPTH_TEST);
	}

	void RenderCommand::SetCullMode(RHI::CullMode mode) const noexcept {
		if (mode == RHI::CullMode::None) {
			glDisable(GL_CULL_FACE);
		}
		else {
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(mode == RHI::CullMode::Front ? GL_FRONT : GL_BACK);
		}
	}

	void RenderCommand::DrawIndexed(Ref<RHI::VertexArray> vertexArray, uint32_t indexCount) const noexcept {
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

	void RenderCommand::DrawIndexedInstanced(Ref<RHI::VertexArray> vertexArray, uint32_t instanceCount) const noexcept {
		// TODO: yet to be created
		QK_ASSERT(false, "Not yet created");
	}

	void RenderCommand::DrawVertices(uint32_t vertexCount) const noexcept {
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

}
