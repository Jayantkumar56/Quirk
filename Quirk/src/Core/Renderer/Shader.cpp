

#include "Qkpch.h"
#include <fstream>

#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Quirk {
	//////////////////////   Shader   ///////////////////////////////////////////////////////////////////////////////////////////////////
	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		RendererAPI::API currentAPI = Renderer::GetAPI();

		switch (currentAPI) {
			case Quirk::RendererAPI::API::None:		QK_CORE_ASSERT(false, "RendererAPI::API::None is not supported right now!"); return nullptr;
			case Quirk::RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
		}

		QK_CORE_ASSERT(false, "Invalid Renderer API Selected!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string shaderSourcesArray[], uint8_t size) {
		RendererAPI::API currentAPI = Renderer::GetAPI();

		switch (currentAPI) {
			case Quirk::RendererAPI::API::None:		QK_CORE_ASSERT(false, "RendererAPI::API::None is not supported right now!"); return nullptr;
			case Quirk::RendererAPI::API::OpenGL:	return std::make_shared<OpenGLShader>(shaderSourcesArray, size);
		}

		QK_CORE_ASSERT(false, "Invalid Renderer API Selected!");
		return nullptr;
	}

	/////////////   ShaderLibrary   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::m_LoadedShaders;

	Ref<Shader> ShaderLibrary::LoadShader(const std::string& filePath) {
		// Extract name from file
		size_t lastSlash = filePath.find_last_of("/\\");
		lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;

		size_t lastDot = filePath.rfind('.');
		size_t count = (lastDot == std::string::npos) ? (filePath.size() - lastSlash) : (lastDot - lastSlash);

		std::string name = filePath.substr(lastSlash, count);
		QK_CORE_ASSERT(!m_LoadedShaders.contains(name), "Shader named {0} already exists!", name);

		m_LoadedShaders[name] = LoadShaderFromFile(filePath);
		return m_LoadedShaders[name];
	}

	Ref<Shader> ShaderLibrary::LoadShader(const std::string& name, const std::string& filePath) {
		QK_CORE_ASSERT(!m_LoadedShaders.contains(name), "Shader named {0} already exists!", name);

		m_LoadedShaders[name] = LoadShaderFromFile(filePath);
		return m_LoadedShaders[name];
	}

	Ref<Shader> ShaderLibrary::LoadShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		QK_CORE_ASSERT(!m_LoadedShaders.contains(name), "Shader named {0} already exists!", name);

		m_LoadedShaders[name] = Shader::Create(vertexSrc, fragmentSrc);
		return m_LoadedShaders[name];
	}

	Ref<Shader> ShaderLibrary::GetShader(const std::string& name) {
		QK_CORE_ASSERT(m_LoadedShaders.contains(name), "Shader named {0} isn't loaded yet", name);
		return m_LoadedShaders[name];
	}

	Ref<Shader> ShaderLibrary::LoadShaderFromFile(const std::string& filePath) {
		std::string source;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);

		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();

			if (size != -1) {
				source.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&source[0], size);
			}
			else {
				QK_CORE_ERROR("Could not read from file '{0}'", filePath);
			}
		}
		else {
			QK_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		// array of all types of sahders present in the source file
		std::string shaderSources[SahderType::NoTypesOfShader];

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken);

		while (pos != std::string::npos) {
			size_t begin = source.find_first_not_of(" \t\n\r", pos + typeTokenLength + 1);
			QK_CORE_ASSERT(begin != std::string::npos, "Syntax error in the shader!");

			size_t end = source.find_first_of(" \t\n\r", begin);
			QK_CORE_ASSERT(end != std::string::npos, "Empty shader source provided!");

			std::string type = source.substr(begin, end - begin);
			uint8_t shaderType = ShaderTypeFromString(type);

			size_t shaderStart = source.find_first_of("#version", end);
			QK_CORE_ASSERT(shaderStart != std::string::npos, "Syntax error");

			pos = source.find(typeToken, shaderStart);
			shaderSources[shaderType -1] = (pos == std::string::npos) ? source.substr(shaderStart) : source.substr(shaderStart, pos - shaderStart);
		}

		return Shader::Create(shaderSources, SahderType::NoTypesOfShader);
	}

	ShaderLibrary::SahderType ShaderLibrary::ShaderTypeFromString(const std::string& type) {
		if (type == "vertex" || type == "Vertex") {
			return ShaderLibrary::SahderType::VertexShader;
		}
		
		if (type == "fragment" || type == "Fragment" || type == "pixel" || type == "Pixel") {
			return ShaderLibrary::SahderType::FragmentShader;
		}

		QK_CORE_ASSERT(false, "Invalid shader type specified");
		return ShaderLibrary::SahderType::None;
	}

}
