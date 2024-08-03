#pragma once

#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(); 
bool GLLogCALL(const char* function, const char* file, int line); 
