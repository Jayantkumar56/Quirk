

#pragma once

#include"glad/glad.h"
#include<iostream>

// to check which particular gl function is causing error
// error checking code start here...................

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


// error checking code ends here....................

