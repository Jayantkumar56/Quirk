

#include "Qkpch.h"

#include "Core/Core.h"
#include "OpenGLShader.h"

#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>


namespace Quirk::OpenGL {

	static GLuint CompileShader(GLenum glType, const char* source) noexcept;

	Shader::Shader(ShaderSource shaderSrc) {
		GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER, shaderSrc.VertexShader.data());
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, shaderSrc.FragmentShader.data());

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

			QK_CORE_ERROR("Shader link failure with error {0}", infoLog.data());
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader() noexcept {
		glDeleteProgram(m_RendererId);
	}

	void Shader::Bind() const noexcept {
		glUseProgram(m_RendererId);
	}

	void Shader::Unbind() const noexcept {
		glUseProgram(0);
	}

	void Shader::UploadUniform(std::string_view name, const glm::mat4& matrix) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniform(std::string_view name, const glm::vec3& vec) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform3fv(location, 1, glm::value_ptr(vec));
	}

	void Shader::UploadUniform(std::string_view name, const glm::vec4& vec) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform4fv(location, 1, glm::value_ptr(vec));
	}

	void Shader::UploadUniform(std::string_view name, const int32_t* data, uint32_t count) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform1iv(location, count, data);
	}

	void Shader::UploadUniform(std::string_view name, const float* data, uint32_t count) const noexcept {
		GLint location = glGetUniformLocation(m_RendererId, name.data());
		glUniform1fv(location, count, data);
	}


	static GLuint CompileShader(GLenum glType, const char* source) noexcept {
		GLuint shader = glCreateShader(glType);

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

			QK_CORE_ERROR("Shader compilation failure with error {0}", infoLog.data());
			return 0;
		}

		return shader;
	}

}
