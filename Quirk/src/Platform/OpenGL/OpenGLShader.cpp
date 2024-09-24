
#include "Qkpch.h"
#include "glad/glad.h"
#include "OpenGLShader.h"
#include "Core/Core.h"
#include <glm/gtc/type_ptr.hpp>

namespace Quirk {

	unsigned int OpenGLShader::ShaderTypeToGLShaderType(uint8_t type) {
		switch (type) {
			case ShaderLibrary::VertexShader:	return GL_VERTEX_SHADER;
			case ShaderLibrary::FragmentShader:	return GL_FRAGMENT_SHADER;
		}

		QK_CORE_ASSERT(false, "Invalid Shader type");
		return 0;
	}

	unsigned int OpenGLShader::CompileShader(unsigned int glType, const char* source) {
		unsigned int shader = glCreateShader(glType);

		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader);

			QK_CORE_ERROR("{0}", infoLog.data());
			QK_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return 0;
		}

		return shader;
	}

	OpenGLShader::OpenGLShader(const std::string shaderSourcesArray[], uint8_t size) {
		GLuint shader[ShaderLibrary::SahderType::NoTypesOfShader] = {0};

		for (uint8_t i = 0; i < size; ++i) {
			if (shaderSourcesArray[i].size() == 0)
				continue;

			shader[i] = CompileShader(ShaderTypeToGLShaderType(i + 1), shaderSourcesArray[i].c_str());
		}

		m_RendererId = glCreateProgram();
		GLuint program = m_RendererId;

		// Attach our shaders to our program
		for (uint8_t i = 0; i < size; ++i) {
			if(shader[i] != 0)
				glAttachShader(program, shader[i]);
		}
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (uint8_t i = 0; i < size; ++i) {
				if (shader[i] != 0)
					glDeleteShader(shader[i]);
			}

			QK_CORE_ERROR("{0}", infoLog.data());
			QK_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (uint8_t i = 0; i < size; ++i) {
			if (shader[i] != 0)
				glDetachShader(program, shader[i]);
		}
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererId = glCreateProgram();
		GLuint program = m_RendererId;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			QK_CORE_ERROR("{0}", infoLog.data());
			QK_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererId);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererId);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

    void OpenGLShader::UploadUniform(const std::string& name, const int32_t* data, uint32_t count) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1iv(location, count, data);
    }

}
