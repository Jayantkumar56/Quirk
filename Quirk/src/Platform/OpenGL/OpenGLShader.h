

#pragma once

#include "Core/RHI/Shader.h"


namespace Quirk::OpenGL {

	class Shader final : public RHI::Shader {
	public:
		Shader(ShaderSource shaderSrc);
		~Shader() noexcept;

		virtual void Bind()   const noexcept override;
		virtual void Unbind() const noexcept override;

		virtual void UploadUniform(std::string_view name, const glm::mat4& matrix)             const noexcept override;
		virtual void UploadUniform(std::string_view name, const glm::vec3& vec)                const noexcept override;
		virtual void UploadUniform(std::string_view name, const glm::vec4& vec)                const noexcept override;
		virtual void UploadUniform(std::string_view name, const int32_t* data, uint32_t count) const noexcept override;
		virtual void UploadUniform(std::string_view name, const float* data, uint32_t count)   const noexcept override;

	private:
		uint32_t m_RendererId;
	};

}
