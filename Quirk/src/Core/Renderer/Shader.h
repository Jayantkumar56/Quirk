

#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Quirk {

	class Shader {
	public:
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	public:
		~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniform(const std::string& name, const glm::mat4& matrix) = 0;
	};

}
