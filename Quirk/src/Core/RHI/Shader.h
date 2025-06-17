

#pragma once

#include "glm/glm.hpp"

#include <string_view>


namespace Quirk {

	struct ShaderSource {
		std::string_view VertexShader;
		std::string_view FragmentShader;
	};

}

namespace Quirk::RHI {

	class Shader {
	public:
		virtual ~Shader() noexcept = default;

		virtual void Bind()   const noexcept = 0;
		virtual void Unbind() const noexcept = 0;

		virtual void UploadUniform(std::string_view name, const glm::mat4& matrix)             const noexcept = 0;
		virtual void UploadUniform(std::string_view name, const glm::vec3& vec)                const noexcept = 0;
		virtual void UploadUniform(std::string_view name, const glm::vec4& vec)                const noexcept = 0;
		virtual void UploadUniform(std::string_view name, const int32_t* data, uint32_t count) const noexcept = 0;
		virtual void UploadUniform(std::string_view name, const float* data, uint32_t count)   const noexcept = 0;
	};

}
