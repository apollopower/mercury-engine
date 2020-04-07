#pragma once

#include <GL/glew.h>

// stops the debugger in case of an invalid assertion
#define ASSERT(x) if (!(x)) __debugbreak();

// Macro to call on GL functions to check for errors
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
