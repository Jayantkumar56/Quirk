

#pragma once

#include "Core/RHI/Factory.h"

#include <string>
#include <unordered_map>


namespace Quirk {

	class ShaderLibrary {
	private:
		enum SahderType : uint8_t {
			None			= 0,
			VertexShader	= 1,
			FragmentShader	= 2,
			PixelShader		= 2,
			NoTypesOfShader	= 2
		};

	public:
		ShaderLibrary(ConstView<RHI::Factory> factory) noexcept :
				m_RHIFactory(factory)
		{}

		Ref<RHI::Shader> LoadShader(const std::string& filePath);
		Ref<RHI::Shader> LoadShader(const std::string& name, const std::string& filePath);
		Ref<RHI::Shader> LoadShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		Ref<RHI::Shader> GetShader(const std::string& name);

	private:
		Ref<RHI::Shader> LoadShaderFromFile(const std::string& filePath);
		SahderType ShaderTypeFromString(const std::string& type);

	private:
		ConstView<RHI::Factory> m_RHIFactory;
		std::unordered_map<std::string, Ref<RHI::Shader>> m_LoadedShaders;
	};

}
