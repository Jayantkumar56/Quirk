

#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "Core/Core.h"


namespace Quirk {

	class Shader {
	public:
		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string shaderSourcesArray[], uint8_t size);

	public:
		~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniform(const std::string& name, const glm::mat4& matrix)             = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec3& vec)                = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec4& vec)                = 0;
		virtual void UploadUniform(const std::string& name, const int32_t* data, uint32_t count) = 0;
		virtual void UploadUniform(const std::string& name, const float* data, uint32_t count)   = 0;
	};

	class ShaderLibrary {
	public:
		enum SahderType : uint8_t {
			None			= 0,
			VertexShader	= 1,
			FragmentShader	= 2,
			PixelShader		= 2,
			NoTypesOfShader	= 2
		};

	public:
		static Ref<Shader> LoadShader(const std::string& filePath);
		static Ref<Shader> LoadShader(const std::string& name, const std::string& filePath);
		static Ref<Shader> LoadShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		static Ref<Shader> GetShader(const std::string& name);

	private:
		static Ref<Shader> LoadShaderFromFile(const std::string& filePath);
		static SahderType ShaderTypeFromString(const std::string& type);

	private:
		static std::unordered_map<std::string, Ref<Shader>> m_LoadedShaders;
	};

}
