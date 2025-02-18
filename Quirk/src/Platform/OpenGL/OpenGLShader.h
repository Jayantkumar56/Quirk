
#pragma once

#include "Core/Renderer/RendererPrimitives/Shader.h"

namespace Quirk {

	class OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string shaderSourcesArray[], uint8_t size);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniform(const std::string& name, const glm::mat4& matrix)             override;
		virtual void UploadUniform(const std::string& name, const glm::vec3& vec)                override;
		virtual void UploadUniform(const std::string& name, const glm::vec4& vec)                override;
		virtual void UploadUniform(const std::string& name, const int32_t* data, uint32_t count) override;
		virtual void UploadUniform(const std::string& name, const float* data, uint32_t count)   override;

	private:
		static unsigned int ShaderTypeToGLShaderType(uint8_t type);
		static unsigned int CompileShader(unsigned int glType, const char* source);

	private:
		uint32_t m_RendererId;
	};

}
