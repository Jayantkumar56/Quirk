
#pragma once

#include "Core/Renderer/Shader.h"

namespace Quirk {

	class OpenGLShder : public Shader{
	public:
		OpenGLShder(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShder();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniform(const std::string& name, const glm::mat4& matrix) override;

	private:
		uint32_t m_RendererId;
	};

}
