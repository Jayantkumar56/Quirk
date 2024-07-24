
#pragma once

#include "Core/Renderer/Shader.h"

namespace Quirk {

	class OpenGLShder : public Shader{
	public:
		OpenGLShder(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShder();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t m_RendererId;
	};

}
