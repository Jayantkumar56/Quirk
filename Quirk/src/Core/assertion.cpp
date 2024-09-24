
#include "Qkpch.h"
#include"assertion.h"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[Opengl Error!] (" << error << ") : " << function << " " << file << " " << line << std::endl;
		return false;
	}

	return true;
}

